// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/ST_NPC.h"
#include "Structs/ST_DialogueAnswer.h"
#include "CPP_UIManager.generated.h"

class ACPP_Controller;

UENUM()
enum class EWidgetType : uint8
{
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

	void SetMainWidget(EWidgetType type);

protected:
	virtual void BeginPlay() override;

private:	

	void HideCurrentWidget();
	void SwitchToPlayerWidget();

	void SetMainWidgetToPlayer();
	void SetMainWidgetToDialogue();

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TMap<EWidgetType, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
	TMap<EWidgetType, TObjectPtr<UUserWidget>> Widgets;

	ACPP_Controller* PlayerController;

	EWidgetType CurrentWidgetType = EWidgetType::Player;

	TObjectPtr<UUserWidget> CurrentWidget;
	


};
