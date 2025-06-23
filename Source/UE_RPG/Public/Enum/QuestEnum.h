#pragma once

UENUM(BlueprintType)
enum class EQusetType : uint8
{
	EQT_GetItem UMETA(DisplayName = "GetItem"),
	EQT_GoToNPC UMETA(DisplayName = "GoToNPC"),
	EQT_GoToSpace UMETA(DisplayName = "GoToSpace"),
	EQT_ComBat UMETA(DisplayName = "ComBat")

};

UENUM(BlueprintType)
enum class EQusetState : uint8
{
	EQS_Normal UMETA(DisplayName = "Normal"),
	EQS_InProgress UMETA(DisplayName = "InProgress"),
	EQS_ConditionClear UMETA(DisplayName = "ConditionClear"),
	EQS_Clear UMETA(DisplayName = "Clear"),
};