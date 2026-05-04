// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/ST_NPC.h"
#include "Structs/ST_DialogueAnswer.h"
#include "CPP_UIManager.generated.h"

class UCPP_InventoryWidget;
class ACPP_Character;

DECLARE_DELEGATE(FOnRevertPlayerWidgetDelegate);

UENUM()
enum class EWidgetType : uint8
{
	None,
	Player,
	NPCDialogue
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_RPG_API UCPP_UIManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCPP_UIManager();
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitUIManager();
	void SetMainWidget(EWidgetType type);
	void RegisterPlayerCharacterToWidget(ACPP_Character* player);

	UCPP_InventoryWidget* GetInventoryWidget();

	FOnRevertPlayerWidgetDelegate RevertPlayerWidget;

protected:
	virtual void BeginPlay() override;

private:	

	void ShowCurrentWidget();
	void HideCurrentWidget();

	void RequestShowCursor();
	void RequestHideCursor();

	UFUNCTION()
	void SwitchToPlayerWidget();

	void SetMainWidgetToPlayer();
	void SetMainWidgetToDialogue();

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TMap<EWidgetType, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY()
	TMap<EWidgetType, TObjectPtr<UUserWidget>> Widgets;

	EWidgetType CurrentWidgetType = EWidgetType::Player;

	TObjectPtr<UUserWidget> CurrentWidget;
	


};
