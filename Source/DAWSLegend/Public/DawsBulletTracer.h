#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DawsBulletTracer.generated.h"

class UStaticMeshComponent;

UCLASS()
class DAWSLEGEND_API ADawsBulletTracer : public AActor
{
    GENERATED_BODY()
public:
    ADawsBulletTracer();
    void InitializeTracer(const FVector& Start, const FVector& End, FLinearColor Tint, float Speed = 14000.0f);
protected:
    virtual void Tick(float DeltaSeconds) override;
private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> BeamMesh;
    FVector Direction = FVector::ForwardVector;
    FVector EndPoint = FVector::ZeroVector;
    float TravelSpeed = 14000.0f;
};
