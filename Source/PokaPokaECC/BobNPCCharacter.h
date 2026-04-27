#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BobNPCCharacter.generated.h"

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

    // AIコントローラーを使って目的地に移動させる関数
    UFUNCTION(BlueprintCallable, Category = "AI")
    void MoveToDestination(FVector Destination);
};