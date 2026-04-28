// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BobAnimInstance.generated.h"

/**
 * Bob用のアニメーションインスタンス
 */
UCLASS()
class UBobAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    // 毎フレームのアニメーション更新処理
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    // ブループリントから読み取り可能なSpeed変数
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Speed=10;
};
