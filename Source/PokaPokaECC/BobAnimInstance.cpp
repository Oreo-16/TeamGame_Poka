#include "BobAnimInstance.h"
#include "GameFramework/Pawn.h"

void UBobAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    // 親クラスの更新処理を呼び出す
    Super::NativeUpdateAnimation(DeltaSeconds);

    // このアニメーションを所有しているPawn（キャラクター）を取得
    APawn* OwnerPawn = TryGetPawnOwner();

    if (OwnerPawn)
    {
        // キャラクターの現在の速度ベクトルを取得し、その長さ（速さ）を計算してSpeedに代入
        Speed = OwnerPawn->GetVelocity().Size();
    }
    else
    {
        // 取得できなかった場合は0で初期化d
        Speed = 0.0f;
    }
}