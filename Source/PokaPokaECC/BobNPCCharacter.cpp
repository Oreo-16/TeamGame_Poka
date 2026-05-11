#include "BobNPCCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h" 

ABobNPCCharacter::ABobNPCCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    // 初期状態は「お店に向かっている」
    CurrentState = ECustomerState::MovingToShop;
    CurrentPathIndex = 0;
}

void ABobNPCCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ABobNPCCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 1. お店に向かっている途中の場合
    if (CurrentState == ECustomerState::MovingToShop && PathPoints.IsValidIndex(CurrentPathIndex))
    {
        FVector TargetLoc = PathPoints[CurrentPathIndex];
        TargetLoc.Z = GetActorLocation().Z; // 高低差を無視して判定

        // 目的地まで150cm以内に近づいたら「到着」とみなす
        if (FVector::Dist(GetActorLocation(), TargetLoc) < 150.0f)
        {
            CurrentPathIndex++; // 次の経由地へ
            MoveToNextPathPoint();
        }
    }
    // 2. 帰宅中の場合
    else if (CurrentState == ECustomerState::Leaving)
    {
        FVector TargetLoc = ExitLocation;
        TargetLoc.Z = GetActorLocation().Z;

        // 出口に到着したら
        if (FVector::Dist(GetActorLocation(), TargetLoc) < 150.0f)
        {
            // キャラクターを消滅させる
            Destroy();
        }
    }
}

void ABobNPCCharacter::MoveToDestination(FVector Destination)
{
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        AIController->MoveToLocation(Destination);
    }
}

// スポナーから呼ばれる設定関数
void ABobNPCCharacter::StartPathMovementWithDelay(TArray<FVector> InPathPoints, FVector InExitLocation, float DelayTime)
{
    PathPoints = InPathPoints;
    ExitLocation = InExitLocation;
    CurrentPathIndex = 0;
    CurrentState = ECustomerState::MovingToShop;

    // DelayTime秒後に ExecutePathMovement を呼ぶ
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABobNPCCharacter::ExecutePathMovement, DelayTime, false);
}

void ABobNPCCharacter::ExecutePathMovement()
{
    MoveToNextPathPoint();
}

// 次のポイントへ移動指示を出す
void ABobNPCCharacter::MoveToNextPathPoint()
{
    if (PathPoints.IsValidIndex(CurrentPathIndex))
    {
        MoveToDestination(PathPoints[CurrentPathIndex]);
    }
    else
    {
        // 経由地を全て歩き終えたら「注文待ち」状態になる
        CurrentState = ECustomerState::Waiting;
    }
}

// ★プレイヤーが料理を渡した時に呼ばれる処理
void ABobNPCCharacter::ReceiveFoodAndLeave()
{
    // 状態を「帰宅」に変更し、出口へ向かわせる
    CurrentState = ECustomerState::Leaving;
    MoveToDestination(ExitLocation);
}