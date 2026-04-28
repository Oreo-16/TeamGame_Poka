// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// エンジン管理用のクラス (U接頭辞)


UINTERFACE(MinimalAPI)
class UInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 調理器具やドアなど、プレイヤーが「Eキー」などで干渉できる全アクター用のインターフェース
 */
class POKAPOKAECC_API IInteractableInterface
{
	GENERATED_BODY()

public:
	// BPでイベントとして実装できるようにする
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void OnInteract(AActor* Caller);
};