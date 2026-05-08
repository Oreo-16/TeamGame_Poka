#include "BobSpawner.h"
#include "BobNPCCharacter.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "Components/InputComponent.h" 
#include "Kismet/GameplayStatics.h" 

ABobSpawner::ABobSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
    DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
    RootComponent = DefaultRoot;

    AutoReceiveInput = EAutoReceiveInput::Player0;
}

void ABobSpawner::BeginPlay()
{
    Super::BeginPlay();

    SpawnAndMoveBob();

    if (InputComponent)
    {
        InputComponent->BindKey(EKeys::Enter, IE_Pressed, this, &ABobSpawner::TestMakeBobLeave);
    }
}

void ABobSpawner::SpawnAndMoveBob()
{
    if (!BobClassToSpawn) return;

    FVector SpawnLocation = GetActorLocation();
    FRotator SpawnRotation = GetActorRotation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    ABobNPCCharacter* SpawnedBob = GetWorld()->SpawnActor<ABobNPCCharacter>(BobClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

    if (SpawnedBob)
    {
        // 客が帰りきった（Destroy直前）ときに、再びこのSpawnAndMoveBob関数を呼んで次の客を生成する
        SpawnedBob->OnCustomerLeft.AddDynamic(this, &ABobSpawner::SpawnAndMoveBob);

        TArray<FVector> WorldPathLocations;
        for (FVector Loc : PathLocations)
        {
            WorldPathLocations.Add(GetTransform().TransformPosition(Loc));
        }

        FVector WorldExitLocation = GetTransform().TransformPosition(ExitLocation);

        SpawnedBob->StartPathMovementWithDelay(WorldPathLocations, WorldExitLocation, 2.0f);
    }
}

void ABobSpawner::TestMakeBobLeave()
{
    TArray<AActor*> FoundBobs;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABobNPCCharacter::StaticClass(), FoundBobs);

    for (AActor* Actor : FoundBobs)
    {
        ABobNPCCharacter* Bob = Cast<ABobNPCCharacter>(Actor);
        if (Bob)
        {
            Bob->ReceiveFoodAndLeave();
        }
    }
}