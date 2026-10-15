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
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
private:
    UPROPERTY(VisibleAnywhere, Category="Systems") UDawsHealthComponent* HealthComponent;
    UPROPERTY(VisibleAnywhere, Category="Visual") UStaticMeshComponent* BodyVisual;
    UPROPERTY(VisibleAnywhere, Category="Visual") UStaticMeshComponent* HeadVisual;
    FName EnemyId = TEXT("grunt");
    float AttackCooldown = 0.0f;
    UFUNCTION() void HandleDeath();
};