#pragma once

#include "CoreMinimal.h"
#include "Widget/CustomUMGWidget.h"
#include "CPP_SettingsWidget.generated.h"

class UButton;


UCLASS()
class UE_RPG_API UCPP_SettingsWidget : public UCustomUMGWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

private:

	UFUNCTION()
	void OnQuitGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	UButton* QuitGameButton;
};
