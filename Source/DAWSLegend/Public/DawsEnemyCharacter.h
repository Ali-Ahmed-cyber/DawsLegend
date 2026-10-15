#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DawsEnemyCharacter.generated.h"
class UDawsHealthComponent;
class UStaticMeshComponent;
UCLASS()
class DAWSLEGEND_API ADawsEnemyCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    ADawsEnemyCharacter();
    void ConfigureEnemy(FName NewEnemyId, bool bSpecialTarget = false);
    void ReceiveWeaponDamage(float Amount);
    float GetHealthRatio() const;
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
private:
    UPROPERTY(VisibleAnywhere, Category="Systems") UDawsHealthComponent* HealthComponent;
    UPROPERTY(VisibleAnywhere, Category="Visual") UStaticMeshComponent* BodyVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") UStaticMeshComponent* HeadVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") UStaticMeshComponent* LeftArmVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") UStaticMeshComponent* RightArmVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") UStaticMeshComponent* LeftLegVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") UStaticMeshComponent* RightLegVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") UStaticMeshComponent* HelmetVisual;
    FName EnemyId = TEXT("grunt");
    float AttackCooldown = 0.0f;
    float RangedDamage = 8.0f;
    float MeleeDamage = 12.0f;
    float VisualAnimationTime = 0.0f;
    UFUNCTION() void HandleDeath();
};