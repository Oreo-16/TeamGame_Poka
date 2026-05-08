// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

UCLASS()
class POKAPOKAECC_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawner();

protected:
	// スポーンさせるアイテムのクラス（BPのインスペクタで設定できるようにする）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings")
	TSubclassOf<AActor> ItemClassToSpawn;

	// スポナーの見た目（木箱などのメッシュ）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* SpawnerMesh;


	// --- 【追記】調理機能用の設定 ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Settings")
	bool bNeedsCooking = false; // エディタでチェックを入れると調理台になる

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
	bool bIsCooking = false;

	UPROPERTY()
	AActor* CookedItem = nullptr; // 完成したがまだ受け取っていないアイテム

	FTimerHandle CookingTimerHandle;

public:
	// プレイヤーがインタラクトした時に呼ばれる関数
	// ItemHoldComponentからアイテムを要求された時に呼ばれる
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	AActor* RequestItem();
	
	// 生成したアイテムを返す（プレイヤー側の処理で、それを受け取って手に付けるため）
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	AActor* SpawnItem();

private:
	// 調理開始と完了の内部処理
	void StartCooking();
	void OnCookingComplete();
};
