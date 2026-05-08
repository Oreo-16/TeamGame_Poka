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

    CurrentState = ECustomerState::MovingToShop;
    CurrentPathIndex = 0;

    // 追加: 初期化
    TargetCounter = nullptr;
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
        TargetLoc.Z = GetActorLocation().Z; // 高低差を無視

        if (FVector::Dist(GetActorLocation(), TargetLoc) < 150.0f)
        {
            CurrentPathIndex++;
            MoveToNextPathPoint();
        }
    }
    // 2. 帰宅中の場合
    else if (CurrentState == ECustomerState::Leaving)
    {
        FVector TargetLoc = ExitLocation;
        TargetLoc.Z = GetActorLocation().Z;

        if (FVector::Dist(GetActorLocation(), TargetLoc) < 150.0f)
        {
            // 帰りきったのでSpawnerに通知を送る（これで次の客が生成される）
            OnCustomerLeft.Broadcast();
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

void ABobNPCCharacter::StartPathMovementWithDelay(TArray<FVector> InPathPoints, FVector InExitLocation, float DelayTime)
{
    PathPoints = InPathPoints;
    ExitLocation = InExitLocation;
    CurrentPathIndex = 0;
    CurrentState = ECustomerState::MovingToShop;

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABobNPCCharacter::ExecutePathMovement, DelayTime, false);
}

void ABobNPCCharacter::ExecutePathMovement()
{
    MoveToNextPathPoint();
}

void ABobNPCCharacter::MoveToNextPathPoint()
{
    if (PathPoints.IsValidIndex(CurrentPathIndex))
    {
        MoveToDestination(PathPoints[CurrentPathIndex]);
    }
    else
    {
        // 目的地に着いたら待機状態にする（ここではアニメーションは再生しない）
        CurrentState = ECustomerState::Waiting;
    }
}

// プレイヤーがトマトを当てた時に呼ばれる処理
void ABobNPCCharacter::ReceiveFoodAndLeave()
{
    // 待機中じゃなければ（お店に向かっている途中なら）無視する
    if (CurrentState != ECustomerState::Waiting) return;

    CurrentState = ECustomerState::Leaving;

    // Blueprintのイベントを呼び出し、リアクションアニメーションを再生
    PlayReactionAnimation();

    // ReactionTime秒後（例: 2.5秒後）に自動で歩いて帰る処理を呼ぶ
    FTimerHandle WaitTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(WaitTimerHandle, this, &ABobNPCCharacter::StartWalkingHome, ReactionTime, false);
}

void ABobNPCCharacter::StartWalkingHome()
{
    MoveToDestination(ExitLocation);
}