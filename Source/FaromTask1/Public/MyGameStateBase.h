#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Delegates/DelegateCombinations.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScoreChanged, int32, Player0Score, int32, Player1Score);

UCLASS()
class FAROMTASK1_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AMyGameStateBase();

	UPROPERTY(ReplicatedUsing=OnRep_Scores)
	int32 Player0Score;

	UPROPERTY(ReplicatedUsing = OnRep_Scores)
	int32 Player1Score;

	UFUNCTION()
	void OnRep_Scores();

	UPROPERTY(BlueprintAssignable, Category = "Game")
	FOnScoreChanged OnScoreChanged;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
