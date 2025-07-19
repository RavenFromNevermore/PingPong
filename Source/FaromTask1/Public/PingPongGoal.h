#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PingPongGoal.generated.h"

UCLASS()
class FAROMTASK1_API APingPongGoal : public AActor
{
 	GENERATED_BODY()

public:
 	APingPongGoal();

protected:
 	virtual void BeginPlay() override;

public:
 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
 	UBoxComponent* GoalCollisionBox;

 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goal")
    int32 PlayerIdToScore;

 	UFUNCTION()
 	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
