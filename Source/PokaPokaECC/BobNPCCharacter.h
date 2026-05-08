#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BobNPCCharacter.generated.h"

// 客が帰りきったことをSpawnerに知らせるためのデリゲート
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomerLeftDelegate);

UENUM(BlueprintType)
enum class ECustomerState : uint8
{
    MovingToShop UMETA(DisplayName = "Moving To Shop"), // お店に向かっている
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
    TArray<FVector> PathPoints;
    int32 CurrentPathIndex;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    FVector ExitLocation;

    // --- 追加: お金を置く対象となるカウンター（台）の参照 ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
    AActor* TargetCounter;

    // トマトが当たってから歩き出すまでのリアクション（アニメーション）時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float ReactionTime = 2.5f;

    // 次の客を呼ぶためのイベント通知用
    UPROPERTY(BlueprintAssignable, Category = "Event")
    FOnCustomerLeftDelegate OnCustomerLeft;

    // --- AI移動関数 ---
    UFUNCTION(BlueprintCallable, Category = "AI")
    void MoveToDestination(FVector Destination);

    UFUNCTION(BlueprintCallable, Category = "AI")
    void StartPathMovementWithDelay(TArray<FVector> InPathPoints, FVector InExitLocation, float DelayTime);

    // トマトが当たった時にBPから呼ぶ関数
    UFUNCTION(BlueprintCallable, Category = "Event")
    void ReceiveFoodAndLeave();

    // トマトが当たった時に、Blueprintでアニメーションを再生させるためのイベント
    UFUNCTION(BlueprintImplementableEvent, Category = "Animation")
    void PlayReactionAnimation();

private:
    UFUNCTION()
    void ExecutePathMovement();
    void MoveToNextPathPoint();

    // アニメーション終了後に実際に歩き出すための関数
    UFUNCTION()
    void StartWalkingHome();
};