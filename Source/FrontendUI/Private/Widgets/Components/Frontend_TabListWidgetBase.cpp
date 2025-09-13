// Aleksei Miakishev All Rights Reserved


#include "Widgets/Components/Frontend_TabListWidgetBase.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Editor/WidgetCompilerLog.h"

void UFrontend_TabListWidgetBase::RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName)
{
	RegisterTab(InTabID, TabButtonEntryWidgetClass, nullptr);
	if (UFrontendCommonButtonBase* FoundButton = Cast<UFrontendCommonButtonBase>(GetTabButtonBaseByID(InTabID)))
	{
		FoundButton->SetButtonText(InTabDisplayName);
	}
}

#if WITH_EDITOR	
void UFrontend_TabListWidgetBase::ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(TEXT("The variable TabButtonEntryWidgetClass has no valid entry specified.") + GetClass()->GetName() + TEXT(" needs a valid entry widget class to function properly.")));
	}
}
#endif

