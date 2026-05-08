#include "ItemSpawner.h"
#include "Engine/World.h"
#include"TimerManager.h"

// コンストラクタ：ゲーム開始前の初期設定
AItemSpawner::AItemSpawner()
{
	// スポナー自体は毎フレームの更新（Tick）を必要としないので処理を軽くするためにオフにする
	PrimaryActorTick.bCanEverTick = false;

	// メッシュコンポーネントを作成し、ルート（基準点）に設定
	SpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerMesh"));
	RootComponent = SpawnerMesh;

	// プレイヤーのSphereCastで「これはスポナーだ！」と判別できるようにタグを追加
	Tags.Add(FName("Spawner"));
}

AActor* AItemSpawner::RequestItem()
{
	// 1. 調理不要な設定なら、即座に生成して渡す
	if (!bNeedsCooking)
	{
		return SpawnItem();
	}

	// 2. 調理が必要な場合：完成品が既にあるなら渡す
	if (CookedItem)
	{
		AActor* ItemToGive = CookedItem;
		CookedItem = nullptr;
		return ItemToGive;
	}

	// 3. 完成品がなく、調理もしていないなら調理開始
	if (!bIsCooking)
	{
		StartCooking();
	}

	return nullptr; // 調理中、または開始直後は何も返さない
}


void AItemSpawner::StartCooking()
{
	bIsCooking = true;
	UE_LOG(LogTemp, Warning, TEXT("Cooking Started... Wait 3 seconds."));

	// 3秒後に OnCookingComplete を実行
	GetWorld()->GetTimerManager().SetTimer(
		CookingTimerHandle, this, &AItemSpawner::OnCookingComplete, 3.0f, false);
}

void AItemSpawner::OnCookingComplete()
{
	bIsCooking = false;

	// アイテムを生成
	CookedItem = SpawnItem();

	if (CookedItem)
	{
		// --- 追加：調理台の上に固定するための処理 ---
		if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(CookedItem->GetRootComponent()))
		{
			// 物理をオフにして空中に固定（あるいは台の上に静止）させる
			PrimComp->SetSimulatePhysics(false);
			// プレイヤーが触れて飛ばさないように衝突判定を調整（拾う判定は残す）
			PrimComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			//PrimComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		// 調理台の少し上に配置（座標調整）
		FVector CookingLocation = GetActorLocation() + FVector(30.0f, 0.0f, 110.0f);
		CookedItem->SetActorLocation(CookingLocation);

		// アイテムに "Holdable" タグがあるか確認（拾えるようにするため）
		if (!CookedItem->ActorHasTag("Holdable"))
		{
			CookedItem->Tags.Add(FName("Holdable"));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("料理が完成して台の上に置かれました！"));
	//bIsCooking = false;
	//CookedItem = SpawnItem(); // 内部的に生成して保持しておく
	//UE_LOG(LogTemp, Warning, TEXT("Cooking Complete! Item is ready."));
}

// アイテムを生成する関数
AActor* AItemSpawner::SpawnItem()
{
	// BP側で「出すアイテム」が設定されていなければ、エラーを防ぐため何もしない
	if (!ItemClassToSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemSpawner: ItemClassToSpawn is null!"));
		return nullptr;
	}

	// アイテムを生成する位置と回転を設定（箱の少し上に出現させる）
	FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 40.0f);
	FRotator SpawnRotation = GetActorRotation();

	// 世界（World）にアイテムを生成（Spawn）する
	AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(ItemClassToSpawn, SpawnLocation, SpawnRotation);

	// 生成したアイテムを返す
	return SpawnedItem;
}