#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// ↓ 前回のエラーを教訓に、必ず .generated.h が一番下になるように注意してください！
#include "CookingStation.generated.h"

// 調理の状態を表すリスト（BPでも使えるようにする）
UENUM(BlueprintType)
enum class ECookingState : uint8
{
	Empty       UMETA(DisplayName = "空っぽ"),
	Cooking     UMETA(DisplayName = "調理中"),
	Done        UMETA(DisplayName = "完成"),
	Burnt       UMETA(DisplayName = "焦げ（失敗）")
};

UCLASS()
class POKAPOKAECC_API ACookingStation : public AActor
{
	GENERATED_BODY()

public:
	ACookingStation();

protected:
	// --- 見た目と置き場所の設定 ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* StationMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* ItemSocket; // 食材がポンッと乗る場所（基準点）

	// --- 調理時間とアイテムの設定（BPで自由に変更可能） ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking Settings")
	float CookTime; // 料理が完成するまでの時間（秒）

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking Settings")
	float BurnTime; // 完成後、放置すると焦げるまでの時間（秒）

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking Settings")
	TSubclassOf<AActor> CookedItemClass; // 完成した時に出現させるアイテム

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooking Settings")
	TSubclassOf<AActor> BurntItemClass; // 焦げた時に出現させるアイテム（ゴミ）

	// --- 現在の状態管理 ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cooking State")
	ECookingState CurrentState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cooking State")
	AActor* CurrentItem; // 今乗っているアイテム本体

	// タイマーを管理するための変数
	FTimerHandle CookingTimerHandle;

	// --- 内部処理（時間が来た時に呼ばれる） ---
	void OnCookingFinished();
	void OnBurnt();

public:
	// プレイヤーがアクセスするための関数
	UFUNCTION(BlueprintCallable, Category = "Cooking")
	bool PlaceItem(AActor* ItemToPlace); // 食材を置く

	UFUNCTION(BlueprintCallable, Category = "Cooking")
	AActor* RetrieveItem(); // 出来上がった食材を取り出す

	// 現在の状態を確認するための便利関数
	UFUNCTION(BlueprintCallable, Category = "Cooking")
	ECookingState GetCurrentState() const { return CurrentState; }
};