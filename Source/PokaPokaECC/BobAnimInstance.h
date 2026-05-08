#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BobAnimInstance.generated.h"

UCLASS()
class UBobAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Speed = 10;
};