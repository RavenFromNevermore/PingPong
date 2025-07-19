#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PingPongPaddle.generated.h"

UCLASS()
class FAROMTASK1_API APingPongPaddle : public APawn
{
	GENERATED_BODY()

public:
	APingPongPaddle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PaddleCube;

	void MoveRight(float AxisValue);

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	int32 PlayerNumber = 0;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerMoveRight(float AxisValue);

	void ServerMoveRight_Implementation(float AxisValue);
	bool ServerMoveRight_Validate(float AxisValue);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OtLifetimeProps) const override;

};
