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

// 分割したコンポーネントをインクルード
#include "ItemHoldComponent.h"

APokaPokaECCCharacter::APokaPokaECCCharacter()
{
	// コリジョンカプセルのサイズ設定
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// コントローラーの回転に合わせてキャラクターを回転させない（トップビュー用）
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// キャラクターの移動設定：入力方向にキャラクターが向き、歩行速度などを設定
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// トップビュー用：カメラブーム（SpringArm）の作成
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1200.0f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bDoCollisionTest = false;

	// カメラの回転を固定（親の回転を引き継がない）
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	// トップビュー用：フォローカメラの作成
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// ★追加：アイテム保持コンポーネントの作成
	ItemHoldComp = CreateDefaultSubobject<UItemHoldComponent>(TEXT("ItemHoldComp"));
}

void APokaPokaECCCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Enhanced Input の入力マッピングを追加
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APokaPokaECCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// ジャンプ
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// 移動
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APokaPokaECCCharacter::Move);

		// ★インタラクト（コンポーネントの機能を呼び出す）
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APokaPokaECCCharacter::Interact);
	}
}

void APokaPokaECCCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
<<<<<<< Updated upstream
		// 固定カメラ（CameraManager）の回転を基準に移動方向を決定
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
=======

		// プレイヤーの現在のカメラ（レベルに置いた固定カメラ）の情報を取得
		if (APlayerCameraManager* CameraManager = PC->PlayerCameraManager)
>>>>>>> Stashed changes
		{
			if (APlayerCameraManager* CameraManager = PC->PlayerCameraManager)
			{
				const FRotator CameraRotation = CameraManager->GetCameraRotation();
				const FRotator YawRotation(0, CameraRotation.Yaw, 0);

				const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
				const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

				AddMovementInput(ForwardDirection, MovementVector.Y);
				AddMovementInput(RightDirection, MovementVector.X);
			}
		}
	}
}

void APokaPokaECCCharacter::Interact(const FInputActionValue& Value)
{
	// キャラクター自身でロジックを持たず、コンポーネントに委託する
	if (ItemHoldComp)
	{
<<<<<<< Updated upstream
		ItemHoldComp->PrimaryInteract();
=======
		// ------------------------------------
		// モノを置く（手放す）処理
		// ------------------------------------
		// 1. キャラクターから切り離す
		HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// 2. 目の前に置く（自分にめり込まないように少し前に出す）
		FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
		HeldItem->SetActorLocation(DropLocation);

		// 3. 物理演算をオンにして床に落とす
		//UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent());
		if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent()))
		///*if (PrimComp)*/
		{
			PrimComp->SetSimulatePhysics(true);
		}

		// 持っているアイテムをリセット
		HeldItem = nullptr;
		return;
	
	}
	else
	{
		// ------------------------------------
		// モノを持つ（拾う）処理
		// ------------------------------------
		FVector Start = GetActorLocation() + FVector(0, 0, 50.0f);
		FVector End = Start + (GetActorForwardVector() * InteractDistance);






		FHitResult OutHit;
		FCollisionShape Sphere = FCollisionShape::MakeSphere(50.0f); // 半径50の球で探す

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); // 自分自身は無視する
		//// デバッグ用の線を表示（これで判定が届いているか確認できます）
		DrawDebugSphere(GetWorld(), End, 50.0f, 12, FColor::Red, false, 2.0f);
		// 目の前を判定（Sphere Trace）
		bool bHit = GetWorld()->SweepSingleByChannel(
			OutHit, Start, End, FQuat::Identity, ECC_Visibility, Sphere, Params);

		if (bHit && OutHit.GetActor())
		{
			AActor* HitActor = OutHit.GetActor();

			// ★【ここに追加！】インターフェース（制作台）かどうかの判定
			if (HitActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
			{
				// 制作台ならタイマーを起動して終了
				IInteractableInterface::Execute_OnInteract(HitActor, this);
				return;
			}
			// タグで「持てるモノ」か判定する（アイテム側に "Holdable" タグをつけておく）
			if (HitActor->ActorHasTag(FName("Holdable")))
			{
				HeldItem = HitActor;

				// 物理演算がオンになっていると手についてこないのでオフにする
				UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent());
				if (PrimComp)
				{
					PrimComp->SetSimulatePhysics(false);
				}

				// キャラクターのメッシュの指定したソケットにくっつける
				HeldItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandSocketName);
			}
		}
>>>>>>> Stashed changes
	}
	//// 1. まず「目の前に何があるか」を先に調べる（持っている・いないに関わらず判定したい場合が多いため）
	//FVector Start = GetActorLocation() + FVector(0, 0, 50.0f);
	//FVector End = Start + (GetActorForwardVector() * InteractDistance);

	//FHitResult OutHit;
	//FCollisionShape Sphere = FCollisionShape::MakeSphere(50.0f);
	//FCollisionQueryParams Params;
	//Params.AddIgnoredActor(this);

	//bool bHit = GetWorld()->SweepSingleByChannel(
	//	OutHit, Start, End, FQuat::Identity, ECC_Visibility, Sphere, Params);

	//// デバッグ用の線を表示（これで判定が届いているか確認できます）
	//DrawDebugSphere(GetWorld(), End, 50.0f, 12, FColor::Red, false, 2.0f);

	//if (bHit && OutHit.GetActor())
	//{
	//	AActor* HitActor = OutHit.GetActor();

	//	// ★最優先：当たったものが「制作台（インターフェース実装者）」なら実行
	//	if (HitActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
	//	{
	//		IInteractableInterface::Execute_OnInteract(HitActor, this);
	//		return; // 制作台を触ったならここで終了
	//	}
	//}

	//// 2. 制作台でない場合、持っているアイテムの「置く/拾う」処理へ
	//if (HeldItem)
	//{
	//	// --- モノを置く処理 ---
	//	HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	//	FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
	//	HeldItem->SetActorLocation(DropLocation);

	//	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent()))
	//	{
	//		PrimComp->SetSimulatePhysics(true);
	//	}

	//	HeldItem = nullptr;
	//}
	//else if (bHit && OutHit.GetActor())
	//{
	//	// --- モノを拾う処理 ---
	//	AActor* HitActor = OutHit.GetActor();
	//	if (HitActor->ActorHasTag(FName("Holdable")))
	//	{
	//		HeldItem = HitActor;

	//		if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent()))
	//		{
	//			PrimComp->SetSimulatePhysics(false);
	//		}

	//		HeldItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandSocketName);
	//	}
	//}
}