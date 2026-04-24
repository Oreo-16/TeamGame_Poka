// Copyright Epic Games, Inc. All Rights Reserved.

#include "PokaPokaECCCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "PokaPokaECC.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

APokaPokaECCCharacter::APokaPokaECCCharacter()
{
	// Tick() を毎フレーム呼び出すように設定
	PrimaryActorTick.bCanEverTick = true;

	// コリジョンカプセルのサイズ設定
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// コントローラーの回転に合わせてキャラクターを回転させない
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// キャラクターの移動設定（入力方向にキャラクターが向く）
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// ジャンプや歩行速度の基本設定
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// ★トップビュー用：カメラブーム（SpringArm）の作成と設定
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1200.0f; // カメラとの距離
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f)); // 見下ろす角度
	CameraBoom->bUsePawnControlRotation = false; // カメラを勝手に回転させない
	CameraBoom->bDoCollisionTest = false; // 障害物でカメラが寄るのを防ぐ

	// ★追加・修正：カメラブームの回転をワールド空間で固定し、親（キャラクター）の回転を一切引き継がないようにする
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	// ★トップビュー用：フォローカメラの作成と設定
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // カメラを勝手に回転させない
}

void APokaPokaECCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// アクションバインディングの設定
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// ジャンプ
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// 移動
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APokaPokaECCCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APokaPokaECCCharacter::Look);

		// 視点移動（※トップビュー固定の場合は実質機能しなくなりますが、入力自体は残しておきます）
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APokaPokaECCCharacter::Look);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APokaPokaECCCharacter::Interact);
	}
	else
	{
		UE_LOG(LogPokaPokaECC, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APokaPokaECCCharacter::Move(const FInputActionValue& Value)
{
	// 入力値をVector2Dとして取得
	FVector2D MovementVector = Value.Get<FVector2D>();

	// 実際の移動処理へルーティング
	DoMove(MovementVector.X, MovementVector.Y);
}

void APokaPokaECCCharacter::Look(const FInputActionValue& Value)
{
	// 入力値をVector2Dとして取得
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// 実際の視点移動処理へルーティング
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void APokaPokaECCCharacter::DoMove(float Right, float Forward)
{
	// コントローラーをPlayerControllerとして取得
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// プレイヤーの現在のカメラ（レベルに置いた固定カメラ）の情報を取得
		if (APlayerCameraManager* CameraManager = PC->PlayerCameraManager)
		{
			// カメラの向きを取得し、Z軸（ヨー）だけの回転にする
			const FRotator CameraRotation = CameraManager->GetCameraRotation();
			const FRotator YawRotation(0, CameraRotation.Yaw, 0);

			// カメラから見た前方ベクトルと右方ベクトルを計算
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// 移動入力を追加
			AddMovementInput(ForwardDirection, Forward);
			AddMovementInput(RightDirection, Right);
		}
	}
}

void APokaPokaECCCharacter::DoLook(float Yaw, float Pitch)
{
	
}

void APokaPokaECCCharacter::DoJumpStart()
{
	// キャラクターにジャンプを指示
	Jump();
}

void APokaPokaECCCharacter::DoJumpEnd()
{
	// キャラクターにジャンプ終了を指示
	StopJumping();
}

void APokaPokaECCCharacter::Interact(const FInputActionValue& Value)
{
	if (HeldItem)
	{
		// ------------------------------------
		// モノを置く（ぽいっと投げる）処理
		// ------------------------------------
		// 1. キャラクターから切り離す
		HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// ★修正：瞬間移動（SetActorLocation）を削除し、物理演算で投げる
		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent());
		if (PrimComp)
		{
			PrimComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			PrimComp->SetSimulatePhysics(true);

			// キャラクターの正面 ＋ 少し上向き のベクトルを作る
			FVector TossDirection = GetActorForwardVector() + FVector(0.0f, 0.0f, 0.5f);
			TossDirection.Normalize();

			// 投げる強さ（質量を掛け合わせることで、重さに関わらず一定の距離飛ぶようにする）
			float TossStrength = 300.0f * PrimComp->GetMass();

			// 力を加えて放り投げる！
			PrimComp->AddImpulse(TossDirection * TossStrength);
		}

		// リセット
		HeldItem = nullptr;
		bIsItemSnapping = false;
	}
	else
	{
		// ------------------------------------
		// モノを持つ（拾う）処理
		// ------------------------------------
		FVector Start = GetActorLocation();
		FVector End = Start + (GetActorForwardVector() * InteractDistance);

		FHitResult OutHit;
		FCollisionShape Sphere = FCollisionShape::MakeSphere(50.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		bool bHit = GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_Visibility, Sphere, Params);

		if (bHit && OutHit.GetActor())
		{
			AActor* HitActor = OutHit.GetActor();

			if (HitActor->ActorHasTag(FName("Holdable")))
			{
				HeldItem = HitActor;

				UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent());
				if (PrimComp)
				{
					PrimComp->SetSimulatePhysics(false);
					PrimComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}

				// ★修正：瞬間移動（SnapToTarget...）ではなく、元の位置を維持（KeepWorldTransform）してくっつける
				HeldItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, HandSocketName);

				// ★追加：Tickでの引き寄せ処理（スッと手元に飛んでくる動き）を開始
				bIsItemSnapping = true;
			}
		}
	}
}

// 毎フレーム呼ばれる処理
void APokaPokaECCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// アイテムを手元に引き寄せ中の場合
	if (bIsItemSnapping && HeldItem)
	{
		// 現在の相対位置と回転を取得
		FVector CurrentLoc = HeldItem->GetRootComponent()->GetRelativeLocation();
		FRotator CurrentRot = HeldItem->GetRootComponent()->GetRelativeRotation();

		// 目標地点はソケットの中心（0,0,0）
		FVector TargetLoc = FVector::ZeroVector; 
		FRotator TargetRot = FRotator::ZeroRotator;

		// FInterpTo を使って、現在地から目標地点へ滑らかに（ゆったりと）移動させる
		FVector NewLoc = FMath::VInterpTo(CurrentLoc, TargetLoc, DeltaTime, ItemSnapSpeed);
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, ItemSnapSpeed);

		HeldItem->GetRootComponent()->SetRelativeLocationAndRotation(NewLoc, NewRot);

		// 十分に手元に近づいたら引き寄せ完了（カチッと固定）
		if (CurrentLoc.IsNearlyZero(1.0f) && CurrentRot.IsNearlyZero(1.0f))
		{
			HeldItem->GetRootComponent()->SetRelativeLocationAndRotation(TargetLoc, TargetRot);
			bIsItemSnapping = false;
		}
	}
}