#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DawsWorldBuilder.generated.h"

UCLASS()
class DAWSLEGEND_API ADawsWorldBuilder : public AActor
{
    GENERATED_BODY()
public:
    ADawsWorldBuilder();
protected:
    virtual void BeginPlay() override;
private:
    UPROPERTY()
    TObjectPtr<UStaticMesh> CubeMesh;
    UPROPERTY()
    TObjectPtr<UMaterialInterface> GroundMaterial;
    void BuildEnvironment();
    AActor* AddBlock(FVector Location, FVector Scale, FLinearColor Tint, bool bCollision = true);
};