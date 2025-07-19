#include "MyGameModeBase.h"
#include "EngineUtils.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerController.h"
#include "PingPongBall.h"
#include "MyGameStateBase.h"

AMyGameModeBase::AMyGameModeBase()
{
    PlayerControllerClass = AMyPlayerController::StaticClass();
    GameStateClass = AMyGameStateBase::StaticClass();
}

void AMyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    AMyPlayerController* CustomPlayerController = Cast<AMyPlayerController>(NewPlayer);
    if (!CustomPlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("PostLogin: NewPlayer is not MyPlayerController!"));
        return;
    }

    TArray<AActor*> FoundPaddles;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APingPongPaddle::StaticClass(), FoundPaddles);

    if (GameState)
    {
        const int32 PlayerIndex = GameState->PlayerArray.Num() - 1;

        if (FoundPaddles.IsValidIndex(PlayerIndex))
        {
            APawn* PaddleToPossess = Cast<APawn>(FoundPaddles[PlayerIndex]);
            if (PaddleToPossess)
            {
                CustomPlayerController->Possess(PaddleToPossess);
                UE_LOG(LogTemp, Warning, TEXT("Player %d possessed Paddle %s"), PlayerIndex, *PaddleToPossess->GetName());

                ACameraActor* CommonCamera = nullptr;
                for (TActorIterator<ACameraActor> It(GetWorld()); It; ++It)
                {
                    ACameraActor* Camera = *It;
                    if (Camera && Camera->GetName().Contains("CameraActor"))
                    {
                        CommonCamera = Camera;
                        break;
                    }
                }

                if (!CommonCamera)
                {
                    UE_LOG(LogTemp, Warning, TEXT("CommonCamera is null. Cannot set view target."));
                    return;
                }

                CustomPlayerController->SetViewTargetWithBlend(CommonCamera, 0.5f, EViewTargetBlendFunction::VTBlend_EaseInOut);
                UE_LOG(LogTemp, Warning, TEXT("Player %s set view target to FixedCamera."), *CustomPlayerController->GetName());

            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No paddle found for Player %d"), PlayerIndex);
        }

        const int32 PlayerCount = GameState->PlayerArray.Num();

        if (PlayerCount == 1)
        {
            CustomPlayerController->Client_DisplayMatchMessage(FText::FromString(TEXT("Waiting for second player...")));

        }
        else if (PlayerCount == 2)
        {
            for (APlayerState* PS : GameState->PlayerArray)
            {
                AMyPlayerController* MyPC = Cast<AMyPlayerController>(PS->GetOwner());
                if (MyPC)
                {
                    MyPC->Client_DisplayMatchMessage(FText::FromString(TEXT("Match started!"))); 
                }
            }
            SpawnBall();
        }
    }
}

void AMyGameModeBase::BeginPlay() {
    Super::BeginPlay();

    if (HasAuthority())
    {
        TArray<AActor*> FoundBalls;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APingPongBall::StaticClass(), FoundBalls);
        if (FoundBalls.Num() > 0) {
            CurrentBall = Cast<APingPongBall>(FoundBalls[0]);
            UE_LOG(LogTemp, Warning, TEXT("Found existing ball in level: %s"), *CurrentBall->GetName());

        }
    }
}

void AMyGameModeBase::SpawnBall() {
    if (!HasAuthority()) return;
    if (!CurrentBall) {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        CurrentBall = GetWorld()->SpawnActor<APingPongBall>(FVector(200.0f, -260.0f, 40.0f), FRotator::ZeroRotator, SpawnParams);
        UE_LOG(LogTemp, Warning, TEXT("Ball spawned by GameMode at %s."), *CurrentBall->GetActorLocation().ToString());
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Ball already exists, not spawning new one. Resetting existing ball."));
    }

    if (CurrentBall) {
        CurrentBall->ResetBall();
    }
}

void AMyGameModeBase::ScorePoint(int32 PlayerId) {
    if (!HasAuthority()) return;
    AMyGameStateBase* MyGameState = GetGameState<AMyGameStateBase>();
    if (MyGameState) {
        if (PlayerId == 0) {
            MyGameState->Player0Score++;
        }
        else if (PlayerId == 1) {
            MyGameState->Player1Score++;
        }

        MyGameState->OnRep_Scores();

        UE_LOG(LogTemp, Warning, TEXT("Player %d Scored! Score: P0: %d, P1: %d"), PlayerId, MyGameState->Player0Score, MyGameState->Player1Score);

        if (CurrentBall) {
            CurrentBall->ResetBall();
        }
        else {
            SpawnBall();
        }
    }
}