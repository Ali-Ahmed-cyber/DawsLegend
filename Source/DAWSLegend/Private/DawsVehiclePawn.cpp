#include "DawsVehiclePawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "DawsPlayerCharacter.h"
#include "GameFramework/PlayerController.h"
ADawsVehiclePawn::ADawsVehiclePawn()
{
    PrimaryActorTick.bCanEverTick = true;
    VehicleCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("VehicleCollision"));
    SetRootComponent(VehicleCollision);
    VehicleCollision->SetBoxExtent(FVector(170,85,55));
    VehicleCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    VehicleCollision->SetCollisionResponseToAllChannels(ECR_Block);
    VehicleCollision->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
    VehicleCollision->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
    VehicleMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("VehicleMovement"));
    VehicleMovement->MaxSpeed = 1450.0f;
    VehicleMovement->Acceleration = 850.0f;
    VehicleMovement->Deceleration = 650.0f;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    VehicleBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleBody"));
    VehicleBody->SetupAttachment(VehicleCollision);
    if (Cube.Succeeded()) VehicleBody->SetStaticMesh(Cube.Object);
    VehicleBody->SetRelativeLocation(FVector(0,0,8));
    VehicleBody->SetRelativeScale3D(FVector(1.65f,0.78f,0.42f));
    VehicleBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(VehicleBody->GetMaterial(0), this)) {
        Mat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.05f,0.18f,0.20f));
        VehicleBody->SetMaterial(0, Mat);
    }
    const FVector WheelLocations[4] = { FVector(95,-78,-18), FVector(95,78,-18), FVector(-95,-78,-18), FVector(-95,78,-18) };
    for (int32 Index = 0; Index < 4; ++Index) {
        const FName WheelName(*FString::Printf(TEXT("WheelVisual%d"), Index));
        UStaticMeshComponent* Wheel = CreateDefaultSubobject<UStaticMeshComponent>(WheelName);
        Wheel->SetupAttachment(VehicleCollision);
        if (Cylinder.Succeeded()) Wheel->SetStaticMesh(Cylinder.Object);
        Wheel->SetRelativeLocation(WheelLocations[Index]);
        Wheel->SetRelativeRotation(FRotator(0,0,90));
        Wheel->SetRelativeScale3D(FVector(0.28f,0.16f,0.28f));
        Wheel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        WheelVisuals.Add(Wheel);
    }
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(VehicleCollision);
    CameraBoom->SetRelativeLocation(FVector(-80,0,110));
    CameraBoom->TargetArmLength = 620.0f;
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->SetRelativeRotation(FRotator(-10,0,0));
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}
void ADawsVehiclePawn::BeginPlay()
{
    Super::BeginPlay();
}
void ADawsVehiclePawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (FMath::Abs(ThrottleInput) > 0.01f) AddMovementInput(GetActorForwardVector(), ThrottleInput);
    const float Speed = GetVelocity().Size2D();
    if (FMath::Abs(SteeringInput) > 0.01f && Speed > 15.0f)
        AddActorLocalRotation(FRotator(0, SteeringInput * TurnRate * DeltaSeconds * FMath::Clamp(Speed / 450.0f, 0.35f, 1.4f), 0));
    if (Speed < 5.0f) VehicleMovement->StopMovementImmediately();
}
void ADawsVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &ADawsVehiclePawn::SetThrottle);
    PlayerInputComponent->BindAxis("MoveRight", this, &ADawsVehiclePawn::SetSteering);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ADawsVehiclePawn::ExitVehicle);
}
void ADawsVehiclePawn::SetThrottle(float Value) { ThrottleInput = FMath::Clamp(Value, -1.0f, 1.0f); }
void ADawsVehiclePawn::SetSteering(float Value) { SteeringInput = FMath::Clamp(Value, -1.0f, 1.0f); }
FText ADawsVehiclePawn::GetInteractionPrompt_Implementation() const
{
    return FText::FromString(TEXT("Press E to drive vehicle"));
}
void ADawsVehiclePawn::Interact_Implementation(AActor* InteractingActor)
{
    ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(InteractingActor);
    if (!Player || DriverCharacter) return;
    APlayerController* PC = Cast<APlayerController>(Player->GetController());
    if (!PC) return;
    DriverCharacter = Player;
    Player->SetActorLocation(GetActorLocation());
    Player->SetActorHiddenInGame(true);
    Player->SetActorEnableCollision(false);
    PC->Possess(this);
}
void ADawsVehiclePawn::ExitVehicle()
{
    if (!DriverCharacter) return;
    ADawsPlayerCharacter* Player = DriverCharacter;
    APlayerController* PC = Cast<APlayerController>(Controller);
    if (!PC) return;
    Player->SetActorLocation(GetActorLocation() - GetActorRightVector() * 260.0f + FVector(0,0,15));
    Player->SetActorHiddenInGame(false);
    Player->SetActorEnableCollision(true);
    DriverCharacter = nullptr;
    ThrottleInput = 0.0f;
    SteeringInput = 0.0f;
    PC->Possess(Player);
}
