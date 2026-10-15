#include "DawsWorldBuilder.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/DirectionalLight.h"
#include "Engine/PointLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"

ADawsWorldBuilder::ADawsWorldBuilder()
{
    PrimaryActorTick.bCanEverTick = false;
    SetActorHiddenInGame(true);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (Cube.Succeeded()) CubeMesh = Cube.Object;
}
void ADawsWorldBuilder::BeginPlay()
{
    Super::BeginPlay();
    BuildEnvironment();
}
AActor* ADawsWorldBuilder::AddBlock(FVector Location, FVector Scale, FLinearColor Tint, bool bCollision)
{
    if (!GetWorld() || !CubeMesh) return nullptr;
    AStaticMeshActor* Block = GetWorld()->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator);
    if (!Block) return nullptr;
    UStaticMeshComponent* Mesh = Block->GetStaticMeshComponent();
    Mesh->SetStaticMesh(CubeMesh);
    Block->SetActorScale3D(Scale);
    Mesh->SetCollisionEnabled(bCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
    Mesh->SetMobility(EComponentMobility::Static);
    UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), Block);
    if (Mat) { Mat->SetVectorParameterValue(TEXT("Color"), Tint); Mesh->SetMaterial(0, Mat); }
    return Block;
}
void ADawsWorldBuilder::BuildEnvironment()
{
    // A complete playable greybox environment, generated at runtime so the project does not depend on missing binary map files.
    AddBlock(FVector(0,0,-70), FVector(90,220,1), FLinearColor(0.13f,0.17f,0.16f));
    // Main road and sidewalks.
    AddBlock(FVector(0,0,-5), FVector(8,220,0.12f), FLinearColor(0.055f,0.06f,0.065f));
    AddBlock(FVector(-520,0,2), FVector(2,220,0.18f), FLinearColor(0.76f,0.68f,0.45f));
    AddBlock(FVector(520,0,2), FVector(2,220,0.18f), FLinearColor(0.76f,0.68f,0.45f));
    // Street markings.
    for (int32 I=-9; I<=9; ++I) AddBlock(FVector(0,I*1000.0f,3), FVector(1.6f,1.4f,0.025f), FLinearColor(0.88f,0.82f,0.61f), false);
    // Distinct buildings frame a broad mission plaza.
    const FVector BuildingLocations[] = {
        FVector(-1050,-1050,260), FVector(-1050,1050,340), FVector(1050,-1050,300),
        FVector(1050,1050,390), FVector(-1900,0,210), FVector(1900,0,250),
        FVector(0,-1900,300), FVector(0,1900,230)
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
    // Extra urban blocks frame the extended campaign route, with alleys and cover near each chapter marker.\n    for (int32 District = 0; District < 5; ++District) {\n        const float Y = 2800.0f + District * 1450.0f;\n        AddBlock(FVector(-1150,Y,180.0f + (District%2)*55.0f), FVector(5.5f,5.0f,3.6f + (District%3)*0.8f), Facades[District%4]);\n        AddBlock(FVector(1150,Y+250.0f,210.0f + (District%3)*35.0f), FVector(5.0f,4.5f,4.2f), Facades[(District+1)%4]);\n        AddBlock(FVector(-650,Y+600.0f,50.0f), FVector(1.1f,0.7f,0.5f), FLinearColor(0.32f,0.22f,0.14f));\n        AddBlock(FVector(650,Y+650.0f,50.0f), FVector(1.1f,0.7f,0.5f), FLinearColor(0.32f,0.22f,0.14f));\n    }\n    // Simple sun lighting.
    ADirectionalLight* Sun = GetWorld()->SpawnActor<ADirectionalLight>(FVector(0,0,1600), FRotator(-42,-28,0));
    if (Sun) {
        Sun->GetLightComponent()->SetIntensity(7.0f);
        Sun->GetLightComponent()->SetLightColor(FLinearColor(1.0f,0.84f,0.65f));
    }
}