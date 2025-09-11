// Aleksei Miakishev All Rights Reserved


#include "Widgets/Widget_PrimaryLayout.h"
#include "FrontendDebugHelper.h"

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InTag)
{
	checkf(RegisteredWidgetStackMap.Contains(InTag), TEXT("Cannot find widget stack by tag '%s'"), *InTag.ToString());

	return RegisteredWidgetStackMap.FindRef(InTag);
}

void UWidget_PrimaryLayout::RegisterWidgetStack(UPARAM(meta=(Categories = "Frontend.WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStack);
			//Debug::Print(TEXT("Widget stack registered under tag ") + InStackTag.ToString());
		}
	}
}
