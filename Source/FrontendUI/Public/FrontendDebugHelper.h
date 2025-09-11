#pragma once

namespace Debug
{
	static void Print(const FString &Message, int32 InKey = -1, const FColor& InColor = FColor::MakeRandomColor())
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(InKey, 10.0f, InColor, Message);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	};
}