#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"
#include "PingPongPaddle.h"
#include "MyPlayerController.h" 
#include "PingPongBall.h"
#include "MyGameStateBase.h"
#include "MyGameModeBase.generated.h"

UCLASS(MinimalAPI, Blueprintable)
class AMyGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMyGameModeBase(); 
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable,Category="Game")
    void ScorePoint(int32 PlayerId);

protected:
    UPROPERTY()
    APingPongBall* CurrentBall;

    void SpawnBall();
};