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

UENUM(BlueprintType)
enum class ECharacterTypes : uint8
{
	Player,
	NPC_Monster,
	NPC_Person,

	Type_None
};