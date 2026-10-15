#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DawsPlayerCharacter.generated.h"
class USpringArmComponent;
class UCameraComponent;
UCLASS()
class DAWSLEGEND_API ADawsPlayerCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    ADawsPlayerCharacter();
protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    void MoveForward(float Value);
    void MoveRight(float Value);
    void TurnCamera(float Value);
    void LookUp(float Value);
    void StartSprint();
    void StopSprint();
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    TObjectPtr<UCameraComponent> FollowCamera;
    UPROPERTY(EditDefaultsOnly, Category="Movement")
    float WalkSpeed = 430.0f;
    UPROPERTY(EditDefaultsOnly, Category="Movement")
    float SprintSpeed = 680.0f;
};