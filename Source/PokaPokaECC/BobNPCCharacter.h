#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BobNPCCharacter.generated.h"

// ★追加: 次の客を呼ぶためのデリゲート（イベント）宣言
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomerLeftDelegate);

// ★追加: 客の「状態」を管理するリスト
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

    // トマトが当たってから歩き出すまでのリアクション（アニメーション）時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float ReactionTime = 2.5f;

    // 次の客を呼ぶためのイベント通知用
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnCustomerLeftDelegate OnCustomerLeft;

    // --- AI移動関数 ---
    UFUNCTION(BlueprintCallable, Category = "AI")
    void MoveToDestination(FVector Destination);

    // ★追加: スポナーから「経由地のリスト」と「帰り道」を受け取って移動を開始する
    UFUNCTION(BlueprintCallable, Category = "AI")
    void StartPathMovementWithDelay(TArray<FVector> InPathPoints, FVector InExitLocation, float DelayTime);

    // ★追加: プレイヤーが料理を渡した時にBPから呼ぶ関数
    UFUNCTION(BlueprintCallable, Category = "Event")
    void ReceiveFoodAndLeave();

private:
    // タイマーから呼び出される実行用関数
    UFUNCTION()
    void ExecutePathMovement();
    void MoveToNextPathPoint();
};