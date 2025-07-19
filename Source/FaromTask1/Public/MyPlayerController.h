#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PingPongPaddle.h"
#include "MyPlayerController.generated.h"

class UUserWidget;

UCLASS()
class FAROMTASK1_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ScoreWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentScoreWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MatchMessageWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentMatchMessageWidget;

	FTimerHandle TimerHandle_ClearMatchMessage;

private:
	void HandleMoveRight(float AxisValue);

public: 
	
	UFUNCTION(Client, Reliable, Category = "UI")
	void Client_DisplayMatchMessage(const FText& Message);

	void Client_DisplayMatchMessage_Implementation(const FText& Message);

	UFUNCTION(Client, Reliable, Category = "UI")
	void Client_ClearMatchMessage();

	void Client_ClearMatchMessage_Implementation();
};