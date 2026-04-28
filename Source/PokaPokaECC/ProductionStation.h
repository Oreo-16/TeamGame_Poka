// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "ProductionStation.generated.h"

UCLASS()
class POKAPOKAECC_API AProductionStation : public AActor, public  IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	
	// Sets default values for this actor's properties
	AProductionStation();
	virtual void OnInteract_Implementation(AActor* Caller) override;

protected:
	// Called when the game starts or when spawned
	// 制作にかかる秒数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
	float ProductionTime = 3.0f;

	// 制作中かどうか（重複防止用）
	UPROPERTY(BlueprintReadOnly, Category = "Production")
	bool bIsProducing = false;

	// タイマー終了時に呼ばれる関数
	void FinishProduction();

	UFUNCTION(BlueprintImplementableEvent, Category = "Production")
	void OnProductionStarted();

	// BP側で「物が現れる演出」や「音」を制御するためのイベント
	UFUNCTION(BlueprintImplementableEvent, Category = "Production")
	void OnProductionComplete();
public:	
	// Called every frame
	FTimerHandle ProductionTimerHandle;

};
