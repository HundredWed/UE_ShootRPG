#pragma once

UENUM(BlueprintType)
enum class ECharacterStateTypes : uint8
{
	UnEquiped,
	Equiped
};

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	Normal,
	Action
};