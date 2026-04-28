#include "Spawner.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

ASpawner::ASpawner()
{
    PrimaryActorTick.bCanEverTick = false;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
    SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
    SpawnPoint->SetupAttachment(RootComponent);
}

void ASpawner::OnInteract_Implementation(AActor* Caller)
{
    if (GetWorldTimerManager().IsTimerActive(SpawnTimerHandle)) return;

    if (ObjectToSpawn)
    {
        // ログ：タイマー開始（全角文字を避けるため英語のみ）
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Timer Start"));

        GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ASpawner::ExecuteSpawn, 3.0f, false);
    }
}

void ASpawner::ExecuteSpawn()
{
    if (ObjectToSpawn && SpawnPoint && GetWorld())
    {
        FVector Location = SpawnPoint->GetComponentLocation();

        // 1. 赤い球を出す（元の位置）
        DrawDebugSphere(GetWorld(), Location, 20.f, 8, FColor::Red, false, 5.f);

        // 2. 高さを足す
        Location.Z += SpawnZOffset;

        // 3. 緑の球を出す（移動後の位置）
        DrawDebugSphere(GetWorld(), Location, 25.f, 8, FColor::Green, false, 5.f);

        FRotator Rotation = SpawnPoint->GetComponentRotation();
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        GetWorld()->SpawnActor<AActor>(ObjectToSpawn, Location, Rotation, SpawnParams);

        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Spawn Done"));
    }
}