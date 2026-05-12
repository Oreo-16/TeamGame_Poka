#include "CookingStation.h"
#include "TimerManager.h" // タイマー機能を使うために必要
#include "Engine/World.h"

// 初期設定
ACookingStation::ACookingStation()
{
	PrimaryActorTick.bCanEverTick = false;

	// メッシュとアイテム置き場（ソケット）の作成
	StationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StationMesh"));
	RootComponent = StationMesh;

	ItemSocket = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSocket"));
	ItemSocket->SetupAttachment(RootComponent);
	ItemSocket->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // コンロの少し上に設定

	// プレイヤーが判別できるようにタグをつける
	Tags.Add(FName("CookingStation"));

	// デフォルト設定
	CookTime = 5.0f;
	BurnTime = 5.0f;
	CurrentState = ECookingState::Empty;
	CurrentItem = nullptr;
}

// 1. 食材を置いた時の処理
bool ACookingStation::PlaceItem(AActor* ItemToPlace)
{
	// すでに使われている、またはアイテムが無い場合は拒否
	if (CurrentState != ECookingState::Empty || ItemToPlace == nullptr) return false;

	CurrentItem = ItemToPlace;

	// 食材をソケットの位置に吸い付かせる
	CurrentItem->SetActorLocationAndRotation(ItemSocket->GetComponentLocation(), ItemSocket->GetComponentRotation());
	CurrentItem->AttachToComponent(ItemSocket, FAttachmentTransformRules::KeepWorldTransform);

	// 調理開始！状態を変更し、完成タイマーをセットする
	CurrentState = ECookingState::Cooking;
	GetWorldTimerManager().SetTimer(CookingTimerHandle, this, &ACookingStation::OnCookingFinished, CookTime, false);

	UE_LOG(LogTemp, Warning, TEXT("調理スタート！"));
	return true;
}

// 2. 完成した時の処理（タイマーから呼ばれる）
void ACookingStation::OnCookingFinished()
{
	if (CurrentItem) CurrentItem->Destroy(); // 生の食材を消す

	// 完成品（焼けたお肉など）を出現させる
	if (CookedItemClass)
	{
		CurrentItem = GetWorld()->SpawnActor<AActor>(CookedItemClass, ItemSocket->GetComponentLocation(), ItemSocket->GetComponentRotation());
		CurrentItem->AttachToComponent(ItemSocket, FAttachmentTransformRules::KeepWorldTransform);
	}

	CurrentState = ECookingState::Done;
	UE_LOG(LogTemp, Warning, TEXT("料理完成！早く取らないと焦げる！"));

	// さらに放置した時の「焦げタイマー」をスタートする
	GetWorldTimerManager().SetTimer(CookingTimerHandle, this, &ACookingStation::OnBurnt, BurnTime, false);
}

// 3. 焦げた時の処理（タイマーから呼ばれる）
void ACookingStation::OnBurnt()
{
	if (CurrentItem) CurrentItem->Destroy(); // 完成品を消す

	// 焦げたゴミを出現させる
	if (BurntItemClass)
	{
		CurrentItem = GetWorld()->SpawnActor<AActor>(BurntItemClass, ItemSocket->GetComponentLocation(), ItemSocket->GetComponentRotation());
		CurrentItem->AttachToComponent(ItemSocket, FAttachmentTransformRules::KeepWorldTransform);
	}

	CurrentState = ECookingState::Burnt;
	UE_LOG(LogTemp, Error, TEXT("あーあ、焦げちゃった..."));
}

// 4. プレイヤーがアイテムを取り出す時の処理
AActor* ACookingStation::RetrieveItem()
{
	// 空っぽ、または調理中（まだ生）の時は取り出せないようにする
	if (CurrentState == ECookingState::Empty || CurrentState == ECookingState::Cooking) return nullptr;

	AActor* ItemToReturn = CurrentItem;

	// 取り出したので、焦げタイマーをストップして安全にする！
	GetWorldTimerManager().ClearTimer(CookingTimerHandle);

	CurrentItem = nullptr;
	CurrentState = ECookingState::Empty; // 空っぽ状態に戻す

	return ItemToReturn;
}