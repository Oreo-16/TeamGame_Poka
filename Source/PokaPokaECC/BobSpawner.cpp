#include "BobSpawner.h"
#include "BobNPCCharacter.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h" // ★追加

ABobSpawner::ABobSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    // ★追加: 空のSceneComponentを作成し、このアクタのRootComponent（根元）に設定する
    DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
    RootComponent = DefaultRoot;
}

void ABobSpawner::BeginPlay()
{
    Super::BeginPlay();

    // 今回はテストとして、ゲーム開始時に自動でスポーンさせる
    SpawnAndMoveBob();
}

void ABobSpawner::SpawnAndMoveBob()
{
    if (!BobClassToSpawn)
    {
        return; // クラスが設定されていなければ何もしない
    }

    // スポナー自身の位置と回転を取得
    FVector SpawnLocation = GetActorLocation();
    FRotator SpawnRotation = GetActorRotation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // Bobをスポーンさせる
    ABobNPCCharacter* SpawnedBob = GetWorld()->SpawnActor<ABobNPCCharacter>(BobClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

    if (SpawnedBob)
    {
        // TargetLocationはSpawnerからの相対座標になっているため、ワールド座標に変換して渡す
        FVector WorldTargetLocation = GetTransform().TransformPosition(TargetLocation);
        SpawnedBob->MoveToDestination(WorldTargetLocation);
    }
}