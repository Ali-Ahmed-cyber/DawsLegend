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

    // Market lane: tea counters, fabric awnings, stools and stacked produce crates.
    const FVector StallCenters[4] = {
        FVector(-760,2250,0), FVector(760,2500,0),
        FVector(-760,2850,0), FVector(760,3150,0)
    };
    const FLinearColor AwningColors[4] = {
        FLinearColor(0.08f,0.42f,0.39f), FLinearColor(0.74f,0.22f,0.14f),
        FLinearColor(0.88f,0.68f,0.22f), FLinearColor(0.12f,0.25f,0.42f)
    };
    for (int32 Stall = 0; Stall < UE_ARRAY_COUNT(StallCenters); ++Stall) {
        const FVector Center = StallCenters[Stall];
        AddBlock(Center + FVector(0,0,72), FVector(1.45f,0.65f,0.12f), FLinearColor(0.29f,0.18f,0.11f));
        AddBlock(Center + FVector(0,0,205), FVector(1.6f,0.82f,0.10f), AwningColors[Stall], false);
        for (int32 Post : {-1,1})
            AddPrimitive(CylinderMesh, Center + FVector(Post*135.0f,0,140), FVector(0.055f,0.055f,1.4f), FLinearColor(0.18f,0.15f,0.12f), false);
        AddBlock(Center + FVector(-80,45,112), FVector(0.28f,0.28f,0.32f), FLinearColor(0.56f,0.31f,0.12f));
        AddBlock(Center + FVector(45,50,112), FVector(0.28f,0.28f,0.32f), FLinearColor(0.28f,0.42f,0.18f));
        AddBlock(Center + FVector(145,-55,48), FVector(0.42f,0.42f,0.36f), FLinearColor(0.34f,0.23f,0.14f));
        AddPrimitive(CylinderMesh, Center + FVector(-135,-105,42), FVector(0.12f,0.12f,0.42f), FLinearColor(0.12f,0.14f,0.15f), true);
    }
    // A small shaded tea corner and public seating along the market wall.
    AddBlock(FVector(980,3650,175), FVector(2.1f,1.6f,0.10f), FLinearColor(0.13f,0.34f,0.29f), false);
    for (int32 Seat = 0; Seat < 3; ++Seat) {
        const FVector Bench(880.0f + Seat*150.0f, 3550.0f, 42.0f);
        AddBlock(Bench, FVector(0.52f,0.12f,0.10f), FLinearColor(0.38f,0.25f,0.14f));
        AddPrimitive(CylinderMesh, Bench+FVector(-38,0,-20), FVector(0.055f,0.055f,0.22f), FLinearColor(0.18f,0.17f,0.15f), true);
        AddPrimitive(CylinderMesh, Bench+FVector(38,0,-20), FVector(0.055f,0.055f,0.22f), FLinearColor(0.18f,0.17f,0.15f), true);
    }
    // Clinic district landmark: pale walls and a high-contrast green medical cross.
    AddBlock(FVector(-1420,3550,220), FVector(5.2f,4.0f,4.4f), FLinearColor(0.72f,0.72f,0.62f));
    AddBlock(FVector(-1420,3144,300), FVector(0.55f,0.06f,1.25f), FLinearColor(0.05f,0.48f,0.30f), false);
    AddBlock(FVector(-1420,3140,300), FVector(1.2f,0.06f,0.38f), FLinearColor(0.05f,0.48f,0.30f), false);
    AddBlock(FVector(-1420,3550,455), FVector(4.0f,3.0f,0.16f), FLinearColor(0.12f,0.20f,0.20f), false);
    // Silent relay landmark: lattice-style mast and three horizontal antenna bars.
    AddPrimitive(CylinderMesh, FVector(820,4680,350), FVector(0.13f,0.13f,7.0f), FLinearColor(0.18f,0.20f,0.21f), false);
    for (int32 Antenna = 0; Antenna < 3; ++Antenna) {
        const float Z = 390.0f + Antenna*95.0f;
        AddBlock(FVector(820,4680,Z), FVector(1.1f,0.08f,0.06f), FLinearColor(0.72f,0.68f,0.55f), false);
        AddPrimitive(SphereMesh, FVector(820,4680,Z+42.0f), FVector(0.11f), FLinearColor(0.08f,0.78f,0.68f), false);
    }
    for (int32 Brace = -1; Brace <= 1; ++Brace)
        AddPrimitive(CylinderMesh, FVector(820+Brace*90.0f,4680,80), FVector(0.07f,0.07f,0.8f), FLinearColor(0.16f,0.18f,0.19f), false);
    // Industrial yard: segmented fence, steel drums and stacked cargo near the late chapters.
    for (int32 Fence = 0; Fence < 8; ++Fence) {
        const float X = -900.0f + Fence*260.0f;
        AddBlock(FVector(X,7000,110), FVector(0.08f,0.08f,1.1f), FLinearColor(0.19f,0.21f,0.21f), false);
    }
    AddBlock(FVector(0,7000,175), FVector(10.0f,0.06f,0.06f), FLinearColor(0.28f,0.31f,0.30f), false);
    AddBlock(FVector(0,7000,90), FVector(10.0f,0.06f,0.06f), FLinearColor(0.28f,0.31f,0.30f), false);
    for (int32 Drum = 0; Drum < 4; ++Drum)
        AddPrimitive(CylinderMesh, FVector(-600.0f+Drum*400.0f,7300,55), FVector(0.36f,0.36f,0.55f), Drum%2==0 ? FLinearColor(0.35f,0.20f,0.12f) : FLinearColor(0.16f,0.28f,0.30f), true);
    // River-side route: visible water strip, bank edge and warning posts.
    AddBlock(FVector(2670,8200,-5), FVector(2.4f,17.0f,0.08f), FLinearColor(0.04f,0.25f,0.34f), false);
    AddBlock(FVector(2440,8200,8), FVector(0.22f,17.0f,0.12f), FLinearColor(0.58f,0.49f,0.31f), false);
    for (int32 Marker = 0; Marker < 7; ++Marker) {
        const float Y = 6800.0f + Marker*430.0f;
        AddPrimitive(CylinderMesh, FVector(2250,Y,55), FVector(0.12f,0.12f,0.55f), FLinearColor(0.86f,0.34f,0.10f), false);
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
    // Final riverfront district: a ferry landing, a small customs office and floodlights for the last chase.
    AddBlock(FVector(0,12100,-8), FVector(14.0f,8.0f,0.10f), FLinearColor(0.12f,0.19f,0.20f), false);
    AddBlock(FVector(-1180,11920,190), FVector(4.8f,3.8f,3.8f), FLinearColor(0.62f,0.49f,0.32f));
    AddBlock(FVector(1180,12250,220), FVector(5.2f,4.0f,4.4f), FLinearColor(0.37f,0.42f,0.40f));
    AddBlock(FVector(-1180,11920,390), FVector(4.0f,3.1f,0.18f), FLinearColor(0.12f,0.18f,0.18f), false);
    AddBlock(FVector(1180,12250,440), FVector(4.4f,3.4f,0.18f), FLinearColor(0.12f,0.18f,0.18f), false);
    for (int32 Floodlight = 0; Floodlight < 4; ++Floodlight) {
        const float X = Floodlight < 2 ? -850.0f : 850.0f;
        const float Y = 11750.0f + (Floodlight % 2) * 950.0f;
        AddPrimitive(CylinderMesh, FVector(X,Y,250), FVector(0.10f,0.10f,2.5f), FLinearColor(0.16f,0.18f,0.18f), false);
        AddBlock(FVector(X,Y,505), FVector(0.48f,0.22f,0.12f), FLinearColor(0.96f,0.78f,0.42f), false);
    }
    AddBlock(FVector(0,12400,90), FVector(4.2f,0.12f,0.12f), FLinearColor(0.82f,0.70f,0.44f), false);

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
