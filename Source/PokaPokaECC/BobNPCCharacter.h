#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BobNPCCharacter.generated.h"

// ★追加: 客が帰りきったことをSpawnerに知らせるためのデリゲート（通知機能）
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomerLeftDelegate);

UENUM(BlueprintType)
enum class ECustomerState : uint8
{
    MovingToShop UMETA(DisplayName = "Moving To Shop"), // お店に向かっている（経由地）
    Waiting      UMETA(DisplayName = "Waiting"),        // 料理待ち
    Leaving      UMETA(DisplayName = "Leaving")         // 帰る
};

UCLASS()
class POKAPOKAECC_API ABobNPCCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABobNPCCharacter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // --- 状態管理 ---
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
    ECustomerState CurrentState;

    // --- 経路データ ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    TArray<FVector> PathPoints; // 経由地のリスト
    int32 CurrentPathIndex;     // 今何番目の経由地に向かっているか

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    FVector ExitLocation;       // 帰る場所

    // ★追加: 目的地に着いたときの待機時間（アニメーション再生時間）
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float WaitTime = 3.0f;

    // ★追加: 次の客を呼ぶためのイベント通知用デリゲート
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnCustomerLeftDelegate OnCustomerLeft;

    // --- AI移動関数 ---
    UFUNCTION(BlueprintCallable, Category = "AI")
    void MoveToDestination(FVector Destination);

    UFUNCTION(BlueprintCallable, Category = "AI")
    void StartPathMovementWithDelay(TArray<FVector> InPathPoints, FVector InExitLocation, float DelayTime);

    UFUNCTION(BlueprintCallable, Category = "Event")
    void ReceiveFoodAndLeave();

    // ★追加: 目的地に着いたときに、Blueprintでアニメーションを再生させるためのイベント
    UFUNCTION(BlueprintImplementableEvent, Category = "Animation")
    void PlayWaitAnimation();

private:
    // タイマーから呼び出される実行用関数
    UFUNCTION()
    void ExecutePathMovement();
    void MoveToNextPathPoint();
};