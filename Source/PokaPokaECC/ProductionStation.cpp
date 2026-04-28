#include "ProductionStation.h"
#include "TimerManager.h"

AProductionStation::AProductionStation()
{
	PrimaryActorTick.bCanEverTick = false;
	bIsProducing = false;
	ProductionTime = 3.0f; // デフォルト値
}

void AProductionStation::OnInteract_Implementation(AActor* Caller)
{
	// 制作中なら何もしない
	if (bIsProducing)
	{
		return;
	}

	bIsProducing = true;

	OnProductionStarted();
	// タイマーのセット
	// 第1引数：ハンドルの参照
	// 第2引数：このオブジェクト
	// 第3引数：終了時に呼び出す関数のアドレス
	// 第4引数：待機時間
	// 第5引数：ループするか（今回は1回きりなのでfalse）
	GetWorldTimerManager().SetTimer(
		ProductionTimerHandle,
		this,
		&AProductionStation::FinishProduction,
		ProductionTime,
		false
	);

	//UE_LOG(LogTemp, Log, TEXT("Production started by: %s"), *Caller->GetName());
}

void AProductionStation::FinishProduction()
{
	bIsProducing = false;

	// タイマーハンドルをクリア
	//GetWorldTimerManager().ClearTimer(ProductionTimerHandle);

	// BP側に通知（ここで「メッシュの表示」や「アイテムのスポーン」をBPで行う）
	OnProductionComplete();

	UE_LOG(LogTemp, Log, TEXT("Production finished!"));
}

