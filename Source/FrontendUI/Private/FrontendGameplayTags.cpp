// Aleksei Miakishev All Rights Reserved

#include "FrontendGameplayTags.h"
#include "NativeGameplayTags.h"

namespace FrontendGameplayTags
{
	//Frontend Widget Stack
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Modal, "Frontend.WidgetStack.Modal");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameMenu, "Frontend.WidgetStack.GameMenu");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_GameHUD, "Frontend.WidgetStack.GameHUD");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_WidgetStack_Frontend, "Frontend.WidgetStack.Frontend");

	//Frontend widgets
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_PressAnyKeyScreen, "Frontend.Widget.PressAnyKeyScreen");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_MainMenuScreen, "Frontend.Widget.MainMenuScreen");
	UE_DEFINE_GAMEPLAY_TAG(Frontend_Widget_ConfirmScreen, "Frontend.Widget.ConfirmScreen");
	
}
