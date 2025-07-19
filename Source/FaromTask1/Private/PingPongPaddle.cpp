#include "PingPongPaddle.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h" 
#include "UObject/ConstructorHelpers.h" 

APingPongPaddle::APingPongPaddle()
{
	PrimaryActorTick.bCanEverTick = true;

	PaddleCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaddleCube"));
	RootComponent = PaddleCube;

	AutoPossessPlayer = EAutoReceiveInput::Disabled;
	AutoPossessAI = EAutoPossessAI::Disabled;

	bReplicates = true;
	SetReplicateMovement(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		PaddleCube->SetStaticMesh(CubeMesh.Object);
		PaddleCube->SetWorldScale3D(FVector(0.2f, 1.5f, 0.2f));
	}
}

void APingPongPaddle::BeginPlay()
{
	Super::BeginPlay();
}

void APingPongPaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APingPongPaddle::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifeTimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifeTimeProps);
}

void APingPongPaddle::ServerMoveRight_Implementation(float AxisValue)
{
	FVector CurrentLocation = GetActorLocation();
	FVector NewLocation = CurrentLocation;
	NewLocation.Y += AxisValue * MoveSpeed * GetWorld()->GetDeltaSeconds();
	NewLocation.Y = FMath::Clamp(NewLocation.Y, -500.0f, 0.0f);
	SetActorLocation(NewLocation);
}

bool APingPongPaddle::ServerMoveRight_Validate(float AxisValue)
{
	return FMath::IsWithinInclusive(AxisValue, -1.0f, 1.0f);
}