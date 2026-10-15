#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "DawsPlayerCharacter.generated.h"
class UDawsHealthComponent;
class UDawsCombatComponent;
class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
UCLASS()
class DAWSLEGEND_API ADawsPlayerCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    ADawsPlayerCharacter();
    void SetCameraFOV(float NewFOV);
    bool IsAiming() const { return bIsAiming; }
    bool IsHitMarkerVisible() const { return HitMarkerTime > 0.0f; }
    void LoadCheckpoint();
    void ResetForNewCampaign();
    FText GetTargetInteractionPrompt() const;
    void SetStatusMessage(const FString& Message, float Duration = 2.5f) { StatusMessage = Message; StatusMessageTime = Duration; }
    FString GetStatusMessage() const { return StatusMessageTime > 0.0f ? StatusMessage : FString(); }
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Systems")
    UDawsHealthComponent* HealthComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Systems")
    UDawsCombatComponent* CombatComponent;
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    void MoveForward(float Value);
    void MoveRight(float Value);
    void TurnCamera(float Value);
    void LookUp(float Value);
    void StartSprint();
    void StopSprint();
    void StartJump();
    void StopJump();
    void FireWeapon();
    void StartAim();
    void StopAim();
    void ReloadWeapon();
    void InteractWithWorld();
    void SaveCheckpoint();
    UFUNCTION() void HandlePlayerDeath();
    void RespawnAtCheckpoint();
    FTimerHandle RespawnTimer;
    FVector InitialSpawnLocation = FVector::ZeroVector;
    float VisualAnimationTime = 0.0f;
    float RecoilAmount = 0.0f;
    float HitMarkerTime = 0.0f;
    bool bIsAiming = false;
    FString StatusMessage;
    float StatusMessageTime = 0.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    TObjectPtr<UCameraComponent> FollowCamera;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> BodyVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> HeadVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> LeftArmVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> RightArmVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> LeftLegVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> RightLegVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> WeaponVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> NeckVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> HairVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> LeftEyeVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> RightEyeVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> NoseVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> MouthVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> BackpackVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> BeltVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> ChestCrestVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> LeftShoulderStripe;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> RightShoulderStripe;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> LeftBootVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") TObjectPtr<UStaticMeshComponent> RightBootVisual;
    UPROPERTY(EditDefaultsOnly, Category="Movement")
    float WalkSpeed = 430.0f;
    UPROPERTY(EditDefaultsOnly, Category="Movement")
    float SprintSpeed = 680.0f;
    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float WeaponDamage = 34.0f;
    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float WeaponRange = 9000.0f;
};