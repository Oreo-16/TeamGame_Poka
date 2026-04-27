#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemHoldComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class POKAPOKAECC_API UItemHoldComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemHoldComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// キャラクターから呼ばれるメインの入力処理
	void PrimaryInteract();

	// 設定値
	UPROPERTY(EditAnywhere, Category = "Interact")
	FName HandSocketName = FName("HoldSocket");

	UPROPERTY(EditAnywhere, Category = "Interact")
	float InteractDistance = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Interact")
	float ItemSnapSpeed = 15.0f;

private:
	// 内部状態
	UPROPERTY()
	AActor* HeldItem = nullptr;

	UPROPERTY()
	AActor* PlacingItem = nullptr;

	bool bIsItemSnapping = false;
	bool bIsItemPlacing = false;
	FVector PlaceTargetLocation = FVector::ZeroVector;
	FRotator PlaceTargetRotation = FRotator::ZeroRotator;

	// 持ち主（キャラクター）への参照をキャッシュ
	class ACharacter* OwnerCharacter;
};