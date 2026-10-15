#include "DawsWorldBuilder.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "Engine/PointLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"

ADawsWorldBuilder::ADawsWorldBuilder()
{
    PrimaryActorTick.bCanEverTick = false;
    SetActorHiddenInGame(true);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (Cube.Succeeded()) CubeMesh = Cube.Object;
    if (Cylinder.Succeeded()) CylinderMesh = Cylinder.Object;
    if (Sphere.Succeeded()) SphereMesh = Sphere.Object;
}
void ADawsWorldBuilder::BeginPlay()
{
    Super::BeginPlay();
    BuildEnvironment();
}
AActor* ADawsWorldBuilder::AddPrimitive(UStaticMesh* Shape, FVector Location, FVector Scale, FLinearColor Tint, bool bCollision)
{
    if (!GetWorld() || !Shape) return nullptr;
    AStaticMeshActor* Block = GetWorld()->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator);
    if (!Block) return nullptr;
    UStaticMeshComponent* Mesh = Block->GetStaticMeshComponent();
    Mesh->SetStaticMesh(Shape);
    Block->SetActorScale3D(Scale);
    Mesh->SetCollisionEnabled(bCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
    Mesh->SetMobility(EComponentMobility::Static);
    if (UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), Block)) {
        Mat->SetVectorParameterValue(TEXT("Color"), Tint);
        Mesh->SetMaterial(0, Mat);
    }
    GeneratedActors.Add(Block);
    return Block;
}
AActor* ADawsWorldBuilder::AddBlock(FVector Location, FVector Scale, FLinearColor Tint, bool bCollision)
{
    return AddPrimitive(CubeMesh, Location, Scale, Tint, bCollision);
}
void ADawsWorldBuilder::BuildEnvironment()
{
    // Procedural runtime city set: district geometry, collision and mission lanes are assembled from engine meshes.
    AddBlock(FVector(0,0,-70), FVector(90,220,1), FLinearColor(0.13f,0.17f,0.16f));
    // Main road and sidewalks.
    AddBlock(FVector(0,0,-5), FVector(8,220,0.12f), FLinearColor(0.055f,0.06f,0.065f));
    AddBlock(FVector(-520,0,2), FVector(2,220,0.18f), FLinearColor(0.76f,0.68f,0.45f));
    AddBlock(FVector(520,0,2), FVector(2,220,0.18f), FLinearColor(0.76f,0.68f,0.45f));
    // Street markings.
    for (int32 I=-9; I<=9; ++I) AddBlock(FVector(0,I*1000.0f,3), FVector(1.6f,1.4f,0.025f), FLinearColor(0.88f,0.82f,0.61f), false);
    // Distinct buildings frame a broad mission plaza.
    const FVector BuildingLocations[] = {
        FVector(-1300,-1050,260), FVector(-1300,1050,340), FVector(1300,-1050,300),
        FVector(1300,1050,390), FVector(-1900,0,210), FVector(1900,0,250),
        FVector(-1300,-1900,300), FVector(1300,1900,230)
    };
    const FVector BuildingScales[] = {
        FVector(7,7,5.2f), FVector(7,6,6.8f), FVector(6,7,6.0f), FVector(8,6,7.8f),
        FVector(5,8,4.2f), FVector(5,8,5.0f), FVector(8,5,6.0f), FVector(8,5,4.6f)
    };
    const FLinearColor Facades[] = {
        FLinearColor(0.49f,0.34f,0.22f), FLinearColor(0.67f,0.55f,0.38f),
        FLinearColor(0.36f,0.40f,0.39f), FLinearColor(0.54f,0.43f,0.31f)
    };
    for (int32 I=0; I<8; ++I) {
        AddBlock(BuildingLocations[I], BuildingScales[I], Facades[I%4]);
        AddBlock(BuildingLocations[I]+FVector(0,0,BuildingScales[I].Z*100.0f+25.0f),
                 FVector(BuildingScales[I].X*0.82f,BuildingScales[I].Y*0.82f,0.25f),
                 FLinearColor(0.17f,0.19f,0.18f));
        // Raised doorway and windows are geometric details, not placeholders for character art.
        AddBlock(BuildingLocations[I]+FVector(0,-BuildingScales[I].Y*100.0f-3,0),
                 FVector(0.8f,0.08f,1.35f), FLinearColor(0.15f,0.20f,0.21f), false);
        for (int32 W=-1; W<=1; ++W)
            AddBlock(BuildingLocations[I]+FVector(W*BuildingScales[I].X*48.0f,-BuildingScales[I].Y*100.0f-4,BuildingScales[I].Z*25.0f),
                     FVector(0.55f,0.07f,0.48f), FLinearColor(0.10f,0.30f,0.34f), false);
    }
    // Cover, crates and barriers create usable combat lanes.
    for (int32 I=0; I<12; ++I) {
        const float X = (I%4-1.5f)*360.0f;
        const float Y = (I/4-1.0f)*540.0f;
        AddBlock(FVector(X,Y,55), FVector(1.1f,1.0f,0.55f), FLinearColor(0.35f,0.24f,0.14f));
    }
    // Mission beacon markers, visible from the plaza.
    AddBlock(FVector(0,850,110), FVector(0.55f,0.55f,2.2f), FLinearColor(0.05f,0.75f,0.62f), false);
    AddBlock(FVector(850,0,110), FVector(0.55f,0.55f,2.2f), FLinearColor(0.92f,0.58f,0.12f), false);
    AddBlock(FVector(-850,0,110), FVector(0.55f,0.55f,2.2f), FLinearColor(0.80f,0.18f,0.12f), false);
    // Extra urban blocks frame the extended campaign route, with alleys and cover near each chapter marker.
    for (int32 District = 0; District < 5; ++District) {
        const float Y = 2800.0f + District * 1450.0f;
        AddBlock(FVector(-1150,Y,180.0f + (District%2)*55.0f), FVector(5.5f,5.0f,3.6f + (District%3)*0.8f), Facades[District%4]);
        AddBlock(FVector(1150,Y+250.0f,210.0f + (District%3)*35.0f), FVector(5.0f,4.5f,4.2f), Facades[(District+1)%4]);
        AddBlock(FVector(-650,Y+600.0f,50.0f), FVector(1.1f,0.7f,0.5f), FLinearColor(0.32f,0.22f,0.14f));
        AddBlock(FVector(650,Y+650.0f,50.0f), FVector(1.1f,0.7f,0.5f), FLinearColor(0.32f,0.22f,0.14f));
    }

    // Final depot district: warehouses, cargo containers and a broadcast mast.
    AddBlock(FVector(-1280,9520,245), FVector(6.5f,6.0f,4.8f), FLinearColor(0.48f,0.37f,0.25f));
    AddBlock(FVector(1280,9960,275), FVector(6.8f,6.5f,5.4f), FLinearColor(0.38f,0.42f,0.40f));
    AddBlock(FVector(-1280,9520,490), FVector(5.2f,4.8f,0.22f), FLinearColor(0.15f,0.17f,0.17f), false);
    AddBlock(FVector(1280,9960,520), FVector(5.4f,5.2f,0.22f), FLinearColor(0.15f,0.17f,0.17f), false);
    const FVector ContainerLocations[6] = {
        FVector(-650,9250,75), FVector(650,9250,75), FVector(-650,9700,75),
        FVector(650,9700,75), FVector(-650,10150,75), FVector(650,10150,75)
    };
    const FLinearColor ContainerColors[3] = {
        FLinearColor(0.12f,0.26f,0.34f), FLinearColor(0.35f,0.18f,0.12f), FLinearColor(0.18f,0.30f,0.22f)
    };
    for (int32 Index = 0; Index < UE_ARRAY_COUNT(ContainerLocations); ++Index)
        AddBlock(ContainerLocations[Index], FVector(1.25f,2.0f,0.7f), ContainerColors[Index%3]);
    AddPrimitive(CylinderMesh, FVector(780,10400,340), FVector(0.14f,0.14f,6.8f), FLinearColor(0.14f,0.16f,0.17f), false);
    AddPrimitive(SphereMesh, FVector(780,10400,685), FVector(0.42f,0.42f,0.42f), FLinearColor(0.95f,0.68f,0.20f), false);
    // Street furniture and local character details along the whole route.
    for (int32 I = 0; I < 8; ++I) {
        const float Y = 350.0f + I * 1350.0f;
        for (int32 Side : {-1, 1}) {
            const float X = Side * 900.0f;
            AddPrimitive(CylinderMesh, FVector(X,Y,150), FVector(0.07f,0.07f,3.0f), FLinearColor(0.13f,0.14f,0.14f), false);
            AddBlock(FVector(X-Side*45.0f,Y,305), FVector(0.75f,0.22f,0.10f), FLinearColor(0.95f,0.77f,0.34f), false);
            AddBlock(FVector(X,Y+240.0f,60), FVector(1.0f,0.75f,0.55f), FLinearColor(0.32f,0.22f,0.14f), true);
            AddBlock(FVector(X,Y+240.0f,118), FVector(1.12f,0.82f,0.12f), Facades[(I+1)%4], false);
        }
    }
    // Palm trees and rooftop water tanks establish the warm urban district silhouette.
    for (int32 I = 0; I < 6; ++I) {
        const float Y = 850.0f + I * 1650.0f;
        for (int32 Side : {-1, 1}) {
            const FVector TreeBase(Side*2250.0f,Y,120.0f);
            AddPrimitive(CylinderMesh, TreeBase, FVector(0.22f,0.22f,2.4f), FLinearColor(0.34f,0.22f,0.12f), false);
            AddPrimitive(SphereMesh, TreeBase+FVector(0,0,175), FVector(1.1f,1.0f,0.72f), FLinearColor(0.12f,0.32f,0.16f), false);
        }
    }
    const FVector TankLocations[4] = { FVector(-1300,-1050,550), FVector(-1300,1050,680), FVector(1300,-1050,600), FVector(1300,1050,780) };
    for (const FVector& TankLocation : TankLocations)
        AddPrimitive(CylinderMesh, TankLocation, FVector(0.85f,0.85f,0.65f), FLinearColor(0.25f,0.28f,0.28f), false);
    // Balanced warm daylight and ambient fill so shaded alleys remain readable.
    ASkyLight* AmbientSky = GetWorld()->SpawnActor<ASkyLight>(FVector(0,0,1200), FRotator::ZeroRotator);
    if (AmbientSky) {
        GeneratedActors.Add(AmbientSky);
        AmbientSky->GetLightComponent()->SetMobility(EComponentMobility::Movable);
        AmbientSky->GetLightComponent()->SetIntensity(1.15f);
        AmbientSky->GetLightComponent()->RecaptureSky();
    }
    // Simple sun lighting.
    ADirectionalLight* Sun = GetWorld()->SpawnActor<ADirectionalLight>(FVector(0,0,1600), FRotator(-42,-28,0));
    if (Sun) {
        GeneratedActors.Add(Sun);
        Sun->GetLightComponent()->SetIntensity(100000.0f);
        Sun->GetLightComponent()->SetLightColor(FLinearColor(1.0f,0.84f,0.65f));
    }
}
void ADawsWorldBuilder::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    for (AActor* Generated : GeneratedActors) {
        if (IsValid(Generated) && Generated != this) Generated->Destroy();
    }
    GeneratedActors.Reset();
    Super::EndPlay(EndPlayReason);
}
