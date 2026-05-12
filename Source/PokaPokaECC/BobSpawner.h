#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BobSpawner.generated.h"

class ABobNPCCharacter;
class USceneComponent;

UCLASS()
class POKAPOKAECC_API ABobSpawner : public AActor
{
    GENERATED_BODY()

public:
    ABobSpawner();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* DefaultRoot;

    UPROPERTY(EditAnywhere, Category = "Spawn")
    TSubclassOf<ABobNPCCharacter> BobClassToSpawn;

    UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true), Category = "Spawn")
    TArray<FVector> PathLocations;

    UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true), Category = "Spawn")
    FVector ExitLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
    AActor* TargetCounter;

    UFUNCTION(BlueprintCallable, Category = "Spawn")
    void SpawnAndMoveBob();

    void TestMakeBobLeave();
};