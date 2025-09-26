// Aleksei Miakishev All Rights Reserved

#pragma once

#include "NativeGameplayTags.h"

namespace FrontendGameplayTags
{
	//Frontend Widget Stack
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Modal);
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameMenu);
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_GameHUD);
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_WidgetStack_Frontend);

	//Frontend widgets
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_PressAnyKeyScreen);
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_MainMenuScreen);
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_OptionsScreen);
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_ConfirmScreen);
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_StoryScreen);
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_KeyRemapScreen);
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Widget_CreditScreen);

	//Frontend Options Images
	FRONTENDUI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frontend_Image_TestImage);
}
