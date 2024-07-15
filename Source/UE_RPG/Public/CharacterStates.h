#pragma once

UENUM(BlueprintType)
enum class ECharacterTypes : uint8
{
	Player,
	NPC_Monster,
	NPC_Person,

	Type_None
};

//Player
//====================================================================
UENUM(BlueprintType)
enum class ECharacterStateTypes : uint8
{
	UnEquiped,
	Equiped,
	Aim,
	Death
};

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	Normal,
	Action,/*can working**/
	SuperAction,/*can't all action**/
};
//====================================================================


//NPC
//====================================================================
UENUM(BlueprintType)
enum class ENPCState : uint8
{
	Normal,
	Patrol,
	Combat,
	Chase,
	SideStep,
	Death
};

UENUM(BlueprintType)
enum class ENPCActionState : uint8
{
	Normal,
	Action,
	SuperAction
};
//====================================================================


//Enemy Only
//====================================================================
UENUM(BlueprintType)
enum class EEnemyCombatTypes : uint8
{
	Dummy,
	One_Hand,
	Two_Hand
};