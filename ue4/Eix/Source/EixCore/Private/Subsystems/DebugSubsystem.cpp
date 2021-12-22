#include "Subsystems/DebugSubsystem.h"

namespace DebugCategory
{
	extern const FName IKCharacter("IKCharacter");

	extern const FName All("All");

	TArray<FName> AllCategories =
	{
		IKCharacter
	};
}

bool UDebugSubsystem::IsDebugCategoryEnabled(const FName& Category) const
{
#if UE_BUILD_SHIPPING || UE_BUILD_TEST
	return false;
#else
	const bool* bEnabledPtr = DebugCategoryEnabledInfo.Find(Category);
	return bEnabledPtr != nullptr && *bEnabledPtr;
#endif
}

void UDebugSubsystem::DebugCategorySetEnabled(const FName& Category_In, bool bEnabled_In)
{
	if (Category_In == DebugCategory::All)
	{
		for (FName CategoryName : DebugCategory::AllCategories)
		{
			DebugCategorySetEnabled(CategoryName, bEnabled_In);
		}
		return;
	}
	
	bool& bEnabled = DebugCategoryEnabledInfo.FindOrAdd(Category_In);
	bEnabled = bEnabled_In;
}