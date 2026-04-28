#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class POKAPOKAECC_API ASpawner : public AActor
{
    GENERATED_BODY()

public:
    ASpawner();

    // インタラクト関数（BlueprintNativeEvent）
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void OnInteract(AActor* Caller);
    virtual void OnInteract_Implementation(AActor* Caller);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USceneComponent* SpawnPoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    TSubclassOf<AActor> ObjectToSpawn;

    // ★高さを調整する変数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float SpawnZOffset = 150.0f;

    FTimerHandle SpawnTimerHandle;

    // スポーン実行関数
    void ExecuteSpawn();
};