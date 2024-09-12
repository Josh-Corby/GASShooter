// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/GSEquipmentManagerComponent.h"

#include "AbilitySystem/GSAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/ActorChannel.h"
#include "Equipment/GSEquipmentDefinition.h"
#include "Equipment/GSEquipmentInstance.h"
#include "GSGameplayTags.h"
#include "Inventory/GSInventoryItemInstance.h"
#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////
// FGSAppliedEquipmentEntry

FString FGSAppliedEquipmentEntry::GetDebugString() const
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(EquipmentDefinition.Get()));
}

void FGSAppliedEquipmentEntry::AddGrantedHandle(FGameplayAbilitySpecHandle Handle)
{
	GrantedHandles.AddAbilitySpecHandle(Handle);
}

void FGSAppliedEquipmentEntry::RemoveGrantedHandle(UGSAbilitySystemComponent* GSASC, FGameplayAbilitySpecHandle Handle)
{
	GrantedHandles.RemoveAbilitySpecHandle(GSASC, Handle);
}

FGSAbilitySet_GrantedHandles* FGSAppliedEquipmentEntry::GetGrantedHandles()
{
	return &GrantedHandles;
}

//////////////////////////////////////////////////////////////////////
// FGSEquipmentList

void FGSEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		const FGSAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
	}
}

void FGSEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FGSAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FGSEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	// 	for (int32 Index : ChangedIndices)
	// 	{
	// 		const FGameplayTagStack& Stack = Stacks[Index];
	// 		TagToCountMap[Stack.Tag] = Stack.StackCount;
	// 	}
}

UGSAbilitySystemComponent* FGSEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UGSAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UGSEquipmentInstance* FGSEquipmentList::AddEntry(UObject* Instigator, TSubclassOf<UGSEquipmentDefinition> EquipmentDefinition)
{
	UGSEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());

	const UGSEquipmentDefinition* EquipmentCDO = GetDefault<UGSEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UGSEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UGSEquipmentInstance::StaticClass();
	}

	FGSAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UGSEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);  //@TODO: Using the actor instead of component as the outer due to UE-127172
	Result = NewEntry.Instance;
	Result->SetInstigator(Instigator);

	if (UGSAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (TObjectPtr<const UGSAbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
		}

		Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

		MarkItemDirty(NewEntry);

		if (const UGSRangedWeaponDefinition* RangedWeaponDef = Cast<UGSRangedWeaponDefinition>(EquipmentCDO))
		{
			FGSGameplayTags GameplayTags = FGSGameplayTags::Get();
			UGSInventoryItemInstance* ItemInstance = Cast<UGSInventoryItemInstance>(Result->GetInstigator());
			int32 CurrentFireTypeIndex = ItemInstance->GetStatTagStackCount(GameplayTags.Weapon_FireType_Index);

			const FGSAbilitySet_GameplayAbility& AbilityToGrant = RangedWeaponDef->WeaponFireTypes[CurrentFireTypeIndex];
			ASC->GiveGSAbility(AbilityToGrant, &NewEntry.GrantedHandles, Result);
		}

		return Result;
	}

	return nullptr;
}

FGSAppliedEquipmentEntry* FGSEquipmentList::FindEntryFromInstance(UGSEquipmentInstance* EquipmentInstance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FGSAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == EquipmentInstance)
		{
			return &Entry;
		}
	}

	return nullptr;
}

void FGSEquipmentList::RemoveEntry(UGSEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FGSAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (UGSAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();

			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

//////////////////////////////////////////////////////////////////////
// UGSEquipmentManagerComponent

UGSEquipmentManagerComponent::UGSEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UGSEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

UGSEquipmentInstance* UGSEquipmentManagerComponent::EquipItem(UObject* Instigator, TSubclassOf<UGSEquipmentDefinition> EquipmentClass)
{
	UGSEquipmentInstance* Result = nullptr;
	if (EquipmentClass != nullptr)
	{
		Result = EquipmentList.AddEntry(Instigator, EquipmentClass);
		if (Result != nullptr)
		{
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	return Result;
}

void UGSEquipmentManagerComponent::UnequipItem(UGSEquipmentInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

bool UGSEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FGSAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		UGSEquipmentInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UGSEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UGSEquipmentManagerComponent::UninitializeComponent()
{
	TArray<UGSEquipmentInstance*> AllEquipmentInstances;

	// gathering all instances before removal to avoid side effects affecting the equipment list iterator	
	for (const FGSAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (UGSEquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

void UGSEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing GSEquipmentInstances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FGSAppliedEquipmentEntry& Entry : EquipmentList.Entries)
		{
			UGSEquipmentInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

UGSEquipmentInstance* UGSEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UGSEquipmentInstance> InstanceType)
{
	for (FGSAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UGSEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<UGSEquipmentInstance*> UGSEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UGSEquipmentInstance> InstanceType) const
{
	TArray<UGSEquipmentInstance*> Results;
	for (const FGSAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UGSEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}

FGSAppliedEquipmentEntry* UGSEquipmentManagerComponent::GetEntryFromEquipmentInstance(UGSEquipmentInstance* EquipmentInstance)
{
	return EquipmentList.FindEntryFromInstance(EquipmentInstance);
}