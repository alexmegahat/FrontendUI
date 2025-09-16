// Aleksei Miakishev All Rights Reserved


#include "Widgets/Components/Frontend_CommonRotator.h"
#include "CommonTextBlock.h"

void UFrontend_CommonRotator::SetSelectedOptionByText(const FText& OptionName)
{
	const int32 FoundIndex = TextLabels.IndexOfByPredicate(
		[OptionName](const FText& TextItem)->bool
		{
			return TextItem.EqualTo(OptionName);
		}
	);
	
	if (FoundIndex != INDEX_NONE)
	{
		SetSelectedItem(FoundIndex);	
	}
	else
	{
		MyText->SetText(OptionName);
	}
}
