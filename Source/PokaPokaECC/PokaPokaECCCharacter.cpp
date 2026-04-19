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

APokaPokaECCCharacter::APokaPokaECCCharacter()
{
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
	if (GetController() != nullptr)
	{
		// トップビュー用に修正：コントローラーの回転ではなく、CameraBoom（カメラ）の向きを基準にする
		// これにより、カメラが固定された状態でも画面の上下左右とキャラクターの移動方向が常に一致します。
		const FRotator CameraRotation = CameraBoom->GetComponentRotation();
		const FRotator YawRotation(0, CameraRotation.Yaw, 0);

		// カメラから見た前方ベクトルを取得
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// カメラから見た右方ベクトルを取得
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 移動入力を追加
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
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