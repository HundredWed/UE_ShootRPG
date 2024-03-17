#pragma once

UENUM(BlueprintType)
enum class ECharacterStateTypes : uint8
{
	UnEquiped,
	Equiped,
	Aim
};

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	Normal,
	Action,/*can working**/
	SuperAction,/*can't working**/
	Death
};

UENUM(BlueprintType)
enum class ECharacterTypes : uint8
{
	Player,
	NPC_Monster,
	NPC_Person,

	Type_None
};