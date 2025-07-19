#include "PingPongBall.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

APingPongBall::APingPongBall()
{
    PrimaryActorTick.bCanEverTick = true;
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    RootComponent = BallMesh;
    BallMesh->SetMobility(EComponentMobility::Movable);

    bReplicates = true;
    SetReplicateMovement(true);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (SphereMesh.Succeeded())
    {
        BallMesh->SetStaticMesh(SphereMesh.Object);
        BallMesh->SetWorldScale3D(FVector(0.3f));
    }

    BallMesh->SetSimulatePhysics(true); 
    
    BallMesh->SetEnableGravity(false);   
    BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BallMesh->SetCollisionProfileName(TEXT("BlockAll")); 
    BallMesh->SetLinearDamping(0.00f); 
    BallMesh->SetAngularDamping(0.00f); 
    BallMesh->SetNotifyRigidBodyCollision(true);
    BallMesh->OnComponentHit.AddDynamic(this, &APingPongBall::OnHit);
}

void APingPongBall::BeginPlay()
{
    Super::BeginPlay();

    if (HasAuthority())
    {
        ResetBall();
    }
}

void APingPongBall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APingPongBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APingPongBall, Velocity);
}

void APingPongBall::ResetBall() {
    SetActorLocation(FVector(200.0f, -260.0f, 40.0f));

    BallMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
    BallMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

    float Angle = FMath::RandRange(-45.0f, 45.0f);
    if (FMath::RandBool()) {
        Angle += 180.0f;
    }
    FVector InitialDirection = FVector(FMath::Cos(FMath::DegreesToRadians(Angle)), FMath::Sin(FMath::DegreesToRadians(Angle)), 0.0f).GetSafeNormal();

    UE_LOG(LogTemp, Warning, TEXT("Before Impulse: SimulatePhysics=%s, Mass=%f, Current Physics Velocity=%s"),
        BallMesh->IsSimulatingPhysics() ? TEXT("TRUE") : TEXT("FALSE"),
        BallMesh->GetMass(),
        *BallMesh->GetPhysicsLinearVelocity().ToString());

    BallMesh->SetPhysicsLinearVelocity(InitialDirection * Speed);

    UE_LOG(LogTemp, Warning, TEXT("Ball Reset! Initial Location: %s, Applied Impulse in Direction: %s, Current Physics Velocity: %s"),
        *GetActorLocation().ToString(),
        *InitialDirection.ToString(),
        *BallMesh->GetPhysicsLinearVelocity().ToString()
    );
}

void APingPongBall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!HasAuthority()) return;

    FVector CurrentVelocity = BallMesh->GetPhysicsLinearVelocity();
    float CurrentSpeed = CurrentVelocity.Size();

    float NewTargetSpeed = FMath::Max(CurrentSpeed * BounceStrengthMultiplier, Speed);

    if (CurrentSpeed > KINDA_SMALL_NUMBER)
    {
        FVector ScaledVelocity = CurrentVelocity.GetSafeNormal() * NewTargetSpeed;
        BallMesh->SetPhysicsLinearVelocity(ScaledVelocity);
    }
}