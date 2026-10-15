#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DawsInteractable.h"
#include "DawsVehiclePawn.generated.h"
class UBoxComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;
class ADawsPlayerCharacter;
UCLASS()
class DAWSLEGEND_API ADawsVehiclePawn : public APawn, public IDawsInteractable
{
    GENERATED_BODY()
public:
    ADawsVehiclePawn();
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual FText GetInteractionPrompt_Implementation() const override;
    virtual void Interact_Implementation(AActor* InteractingActor) override;
    void ApplyVehicleDamage(float Amount);
    void SetCameraFOV(float NewFOV);
    float GetVehicleHealth() const { return VehicleHealth; }
    bool IsVehicleDisabled() const { return bDisabled; }
protected:
    virtual void BeginPlay() override;
private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UBoxComponent> VehicleCollision;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> VehicleBody;
    UPROPERTY(VisibleAnywhere)
    TArray<TObjectPtr<UStaticMeshComponent>> WheelVisuals;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USpringArmComponent> CameraBoom;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> FollowCamera;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UFloatingPawnMovement> VehicleMovement;
    UPROPERTY()
    TObjectPtr<ADawsPlayerCharacter> DriverCharacter;
    float ThrottleInput = 0.0f;
    float SteeringInput = 0.0f;
    float TurnRate = 78.0f;
    float VehicleHealth = 250.0f;
    bool bDisabled = false;
    void SetThrottle(float Value);
    void SetSteering(float Value);
    void ExitVehicle();
};