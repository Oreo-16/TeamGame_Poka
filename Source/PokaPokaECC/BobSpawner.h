#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BobSpawner.generated.h"

class ABobNPCCharacter;

UCLASS()
class POKAPOKAECC_API ABobSpawner : public AActor
{
    GENERATED_BODY()

public:
    ABobSpawner();

protected:
    virtual void BeginPlay() override;

public:
    // ★追加: アクタが位置情報を持てるようにするためのルートコンポーネント
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* DefaultRoot;

    // スポーンさせるBobのブループリントクラスをエディタで指定する用
    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<ABobNPCCharacter> BobClassToSpawn;

    // 歩かせる目的地の座標 (MakeEditWidgetでエディタ上に移動可能なひし形のピンが出ます)
    UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true), Category = "Spawn")
    FVector TargetLocation;

    // スポーン処理本体
    UFUNCTION(BlueprintCallable, Category = "Spawn")
    void SpawnAndMoveBob();
};