#include "DawsBulletTracer.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

ADawsBulletTracer::ADawsBulletTracer()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorEnableCollision(false);
    BeamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeamMesh"));
    SetRootComponent(BeamMesh);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Cube.Succeeded()) BeamMesh->SetStaticMesh(Cube.Object);
    BeamMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BeamMesh->SetRelativeScale3D(FVector(1.35f,0.018f,0.018f));
    if (UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(BeamMesh->GetMaterial(0), this)) {
        Mat->SetVectorParameterValue(TEXT("Color"), FLinearColor(1.0f,0.72f,0.22f));
        BeamMesh->SetMaterial(0, Mat);
    }
    InitialLifeSpan = 1.2f;
}
void ADawsBulletTracer::InitializeTracer(const FVector& Start, const FVector& End, FLinearColor Tint, float Speed)
{
    SetActorLocation(Start);
    EndPoint = End;
    const FVector Delta = End - Start;
    Direction = Delta.GetSafeNormal();
    TravelSpeed = FMath::Max(1000.0f, Speed);
    SetActorRotation(Direction.Rotation());
    if (UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(BeamMesh->GetMaterial(0), this)) {
        Mat->SetVectorParameterValue(TEXT("Color"), Tint);
        BeamMesh->SetMaterial(0, Mat);
    }
    if (Delta.Size() < 140.0f) {
        SetActorLocation(End);
        SetLifeSpan(0.06f);
    }
}
void ADawsBulletTracer::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    const FVector Next = GetActorLocation() + Direction * TravelSpeed * DeltaSeconds;
    if (FVector::DotProduct(EndPoint - Next, Direction) <= 0.0f) {
        SetActorLocation(EndPoint);
        SetLifeSpan(0.045f);
        SetActorTickEnabled(false);
        return;
    }
    SetActorLocation(Next);
}
