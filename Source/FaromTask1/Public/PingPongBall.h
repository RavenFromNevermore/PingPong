#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PingPongBall.generated.h"

UCLASS()
class FAROMTASK1_API APingPongBall : public AActor
{
    GENERATED_BODY()

public:
    APingPongBall();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BallMesh;

    UPROPERTY(Replicated)
    FVector Velocity;

    UPROPERTY(EditAnywhere)
    float Speed = 800.0f;

    UPROPERTY(EditAnywhere, Category = "Ball Physics")
    float BounceStrengthMultiplier = 1.1f;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void ResetBall();

protected:
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
