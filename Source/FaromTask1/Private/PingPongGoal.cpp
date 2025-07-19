#include "PingPongGoal.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameModeBase.h"
#include "PingPongBall.h"

APingPongGoal::APingPongGoal()
{
    PrimaryActorTick.bCanEverTick = false;

    GoalCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("GoalCollisionBox"));
    RootComponent = GoalCollisionBox;

    GoalCollisionBox->SetCollisionProfileName(TEXT("OverlapAll"));
    GoalCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GoalCollisionBox->SetGenerateOverlapEvents(true);

    GoalCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APingPongGoal::OnOverlapBegin);

    PlayerIdToScore = 0;
}

void APingPongGoal::BeginPlay()
{
    Super::BeginPlay();
}

void APingPongGoal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority()) return;

    APingPongBall* Ball = Cast<APingPongBall>(OtherActor);
    if (Ball)
    {
        AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
        if (GameMode)
        {
            GameMode->ScorePoint(PlayerIdToScore);
        }
    }
}