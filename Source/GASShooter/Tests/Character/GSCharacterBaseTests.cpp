

#if WITH_TESTS

// LL Tests
#include "Tests/TestHarnessAdapter.h"

// Automation Tests
#include "Misc/AutomationTest.h"

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FDaysOfWeekTest, "GASShooter.Tests.DaysOfWeekDemo", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ClientContext | EAutomationTestFlags::EditorContext)

void FDaysOfWeekTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray <FString>& OutTestCommands) const
{
	TArray<FString> TestCases;

	TestCases.Add("Monday");
	TestCases.Add("Tuesday");
	TestCases.Add("Wednesday");
	TestCases.Add("Thursday");
	TestCases.Add("Friday");
	TestCases.Add("Saturday");
	TestCases.Add("Sunday");
	TestCases.Add("Failing Case");

	for (const FString& TestCase : TestCases)
	{
		OutBeautifiedNames.Add(TestCase + TEXT("ContainsTheWordDay"));
		OutTestCommands.Add(TestCase);
	}
}

bool FDaysOfWeekTest::RunTest(const FString& Parameters)
{
	TestTrue(FString::Printf(TEXT("Parameter string [%s] did not contain 'day'."), *Parameters), Parameters.Contains(TEXT("day")));

	return true;
}

#endif //WITH_TESTS