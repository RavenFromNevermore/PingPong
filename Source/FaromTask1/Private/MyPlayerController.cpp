// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerInput.h"
#include <Components/TextBlock.h>
#include "TimerManager.h"

AMyPlayerController::AMyPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("APingPongPlayerController BeginPlay called."));

	if (IsLocalPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("IsLocalPlayerController() is TRUE."));

		// Match Message Widget creation
		if (MatchMessageWidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("MatchMessageWidgetClass is VALID: %s"), *MatchMessageWidgetClass->GetName());
			CurrentMatchMessageWidget = CreateWidget<UUserWidget>(this, MatchMessageWidgetClass);
			if (CurrentMatchMessageWidget)
			{
				CurrentMatchMessageWidget->AddToViewport();
				CurrentMatchMessageWidget->SetVisibility(ESlateVisibility::Hidden);
				UE_LOG(LogTemp, Warning, TEXT("Match Message Widget Added to Viewport."));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ERROR: CreateWidget failed to create CurrentMatchMessageWidget!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ERROR: MatchMessageWidgetClass is NOT set in Blueprint!"));
		}
		if (ScoreWidgetClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("ScoreWidgetClass is VALID: %s"), *ScoreWidgetClass->GetName());
			CurrentScoreWidget = CreateWidget<UUserWidget>(this, ScoreWidgetClass);
			if (CurrentScoreWidget)
			{
				UE_LOG(LogTemp, Warning, TEXT("CurrentScoreWidget created successfully. Attempting AddToViewport."));
				CurrentScoreWidget->AddToViewport();
				UE_LOG(LogTemp, Warning, TEXT("Score Widget Added to Viewport."));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ERROR: CreateWidget failed to create CurrentScoreWidget!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ERROR: ScoreWidgetClass is NOT set in Blueprint!"));
		}
	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IsLocalPlayerController() is FALSE. Skipping UI creation."));
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAxis("MoveRight", this, &AMyPlayerController::HandleMoveRight);
	}
}

void AMyPlayerController::HandleMoveRight(float AxisValue)
{
	APingPongPaddle* PossessedPaddle = Cast<APingPongPaddle>(GetPawn());

	if (PossessedPaddle)
	{
		FVector CurrentLocation = PossessedPaddle->GetActorLocation();
		FVector NewLocation = CurrentLocation;
		NewLocation.Y += AxisValue * PossessedPaddle->MoveSpeed * GetWorld()->GetDeltaSeconds();
		NewLocation.Y = FMath::Clamp(NewLocation.Y, -500.0f, 0.0f);
		PossessedPaddle->SetActorLocation(NewLocation);

		if (!HasAuthority())
		{
			PossessedPaddle->ServerMoveRight(AxisValue);
		}
	}
}

void AMyPlayerController::Client_DisplayMatchMessage_Implementation(const FText& Message)
{
	if (CurrentMatchMessageWidget)
	{

		UTextBlock* MessageTextBlock = Cast<UTextBlock>(CurrentMatchMessageWidget->GetWidgetFromName(TEXT("MessageTextBlock")));

		if (MessageTextBlock)
		{
			MessageTextBlock->SetText(Message);
			CurrentMatchMessageWidget->SetVisibility(ESlateVisibility::Visible);

			GetWorldTimerManager().ClearTimer(TimerHandle_ClearMatchMessage);

			GetWorldTimerManager().SetTimer(TimerHandle_ClearMatchMessage, this, &AMyPlayerController::Client_ClearMatchMessage_Implementation, 3.0f, false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Client_DisplayMatchMessage: MessageTextBlock not found in CurrentMatchMessageWidget!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Client_DisplayMatchMessage: CurrentMatchMessageWidget is NULL!"));
	}
}

void AMyPlayerController::Client_ClearMatchMessage_Implementation()
{
	if (CurrentMatchMessageWidget)
	{
		CurrentMatchMessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}