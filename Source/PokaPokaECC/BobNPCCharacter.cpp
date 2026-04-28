#include "BobNPCCharacter.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ABobNPCCharacter::ABobNPCCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // ★重要: ワールドに配置された時も、スポーンされた時も自動でAIコントローラーを割り当てる
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    // NPCが移動方向を向くようにする設定
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
}

void ABobNPCCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ABobNPCCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABobNPCCharacter::MoveToDestination(FVector Destination)
{
    // 自身を操作しているAIコントローラーを取得
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        // ナビゲーションメッシュを利用して目的地まで歩かせる
        AIController->MoveToLocation(Destination);
    }
}