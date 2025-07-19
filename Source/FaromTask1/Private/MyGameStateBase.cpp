#include "MyGameStateBase.h"
#include "Net/UnrealNetwork.h"

AMyGameStateBase::AMyGameStateBase()
{
    Player0Score = 0;
    Player1Score = 0;
}

void AMyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AMyGameStateBase, Player0Score);
    DOREPLIFETIME(AMyGameStateBase, Player1Score);
}

void AMyGameStateBase::OnRep_Scores()
{
    UE_LOG(LogTemp, Log, TEXT("Score Updated (Client): P0: %d, P1: %d"), Player0Score, Player1Score);
    OnScoreChanged.Broadcast(Player0Score, Player1Score);
}