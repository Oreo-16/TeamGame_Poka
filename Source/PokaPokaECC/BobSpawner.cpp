#include "BobSpawner.h"
#include "BobNPCCharacter.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
// ★追加: 入力を処理するためのインクルード
#include "Components/InputComponent.h" 
// ★追加: Bobを探し出すためのインクルード
#include "Kismet/GameplayStatics.h" 

ABobSpawner::ABobSpawner()
{
    PrimaryActorTick.bCanEverTick = false;
    DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
    RootComponent = DefaultRoot;

    // ★追加: このアクタ（スポナー）が「プレイヤー0（ローカルプレイヤー）」の入力を直接受け取るように設定
    AutoReceiveInput = EAutoReceiveInput::Player0;
}

void ABobSpawner::BeginPlay()
{
    Super::BeginPlay();

    SpawnAndMoveBob();

    // ★追加: アクタの入力コンポーネントが有効になったら、Enterキーの入力をバインドする
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
        // --- 追加: Spawnerで設定したカウンターの参照をBobに渡す ---
        SpawnedBob->TargetCounter = this->TargetCounter;

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

// ★追加: Enterキーを押した時の処理本体
void ABobSpawner::TestMakeBobLeave()
{
    TArray<AActor*> FoundBobs;

    // ワールド内にいる「ABobNPCCharacter」を全て探し出す
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABobNPCCharacter::StaticClass(), FoundBobs);

    for (AActor* Actor : FoundBobs)
    {
        ABobNPCCharacter* Bob = Cast<ABobNPCCharacter>(Actor);
        if (Bob)
        {
            // 見つけたBob全員に帰宅処理を実行させる
            Bob->ReceiveFoodAndLeave();
        }
    }
}