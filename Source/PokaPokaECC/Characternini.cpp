// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "Characternini.h"
//
//// Sets default values
//ACharacternini::ACharacternini()
//{
// 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//}
//// Copyright Epic Games, Inc. All Rights Reserved.
//
//#include "PokaPokaECCCharacter.h"
//#include "Engine/LocalPlayer.h"
//#include "Camera/CameraComponent.h"
//#include "Components/CapsuleComponent.h"
//#include "GameFramework/CharacterMovementComponent.h"
//#include "GameFramework/SpringArmComponent.h"
//#include "GameFramework/Controller.h"
//#include "EnhancedInputComponent.h"
//#include "EnhancedInputSubsystems.h"
//#include "InputActionValue.h"
//#include "PokaPokaECC.h"
//#include "Engine/World.h"
//#include "Components/PrimitiveComponent.h"
//
//APokaPokaECCCharacter::APokaPokaECCCharacter()
//{
//	// コリジョンカプセルのサイズ設定
//	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
//
//	// コントローラーの回転に合わせてキャラクターを回転させない
//	bUseControllerRotationPitch = false;
//	bUseControllerRotationYaw = false;
//	bUseControllerRotationRoll = false;
//
//	// キャラクターの移動設定（入力方向にキャラクターが向く）
//	GetCharacterMovement()->bOrientRotationToMovement = true;
//	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
//
//	// ジャンプや歩行速度の基本設定
//	GetCharacterMovement()->JumpZVelocity = 500.f;
//	GetCharacterMovement()->AirControl = 0.35f;
//	GetCharacterMovement()->MaxWalkSpeed = 500.f;
//	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
//	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
//	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
//
//	// ★トップビュー用：カメラブーム（SpringArm）の作成と設定
//	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
//	CameraBoom->SetupAttachment(RootComponent);
//	CameraBoom->TargetArmLength = 1200.0f; // カメラとの距離
//	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f)); // 見下ろす角度
//	CameraBoom->bUsePawnControlRotation = false; // カメラを勝手に回転させない
//	CameraBoom->bDoCollisionTest = false; // 障害物でカメラが寄るのを防ぐ
//
//	// ★追加・修正：カメラブームの回転をワールド空間で固定し、親（キャラクター）の回転を一切引き継がないようにする
//	CameraBoom->SetUsingAbsoluteRotation(true);
//	CameraBoom->bInheritPitch = false;
//	CameraBoom->bInheritYaw = false;
//	CameraBoom->bInheritRoll = false;
//
//	// ★トップビュー用：フォローカメラの作成と設定
//	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
//	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
//	FollowCamera->bUsePawnControlRotation = false; // カメラを勝手に回転させない
//}
//
//void APokaPokaECCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	// アクションバインディングの設定
//	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
//
//		// ジャンプ
//		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
//		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
//
//		// 移動
//		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APokaPokaECCCharacter::Move);
//		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APokaPokaECCCharacter::Look);
//
//		// 視点移動（※トップビュー固定の場合は実質機能しなくなりますが、入力自体は残しておきます）
//		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APokaPokaECCCharacter::Look);
//
//		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APokaPokaECCCharacter::Interact);
//	}
//	else
//	{
//		UE_LOG(LogPokaPokaECC, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
//	}
//}
//
//void APokaPokaECCCharacter::Move(const FInputActionValue& Value)
//{
//	// 入力値をVector2Dとして取得
//	FVector2D MovementVector = Value.Get<FVector2D>();
//
//	// 実際の移動処理へルーティング
//	DoMove(MovementVector.X, MovementVector.Y);
//}
//
//void APokaPokaECCCharacter::Look(const FInputActionValue& Value)
//{
//	// 入力値をVector2Dとして取得
//	FVector2D LookAxisVector = Value.Get<FVector2D>();
//
//	// 実際の視点移動処理へルーティング
//	DoLook(LookAxisVector.X, LookAxisVector.Y);
//}
//
//void APokaPokaECCCharacter::DoMove(float Right, float Forward)
//{
//	// コントローラーをPlayerControllerとして取得
//	if (APlayerController* PC = Cast<APlayerController>(GetController()))
//	{
//
//		// プレイヤーの現在のカメラ（レベルに置いた固定カメラ）の情報を取得
//		if (APlayerCameraManager* CameraManager = PC->PlayerCameraManager)
//		{
//			// カメラの向きを取得し、Z軸（ヨー）だけの回転にする
//			const FRotator CameraRotation = CameraManager->GetCameraRotation();
//			const FRotator YawRotation(0, CameraRotation.Yaw, 0);
//
//			// カメラから見た前方ベクトルと右方ベクトルを計算
//			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
//			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
//
//			// 移動入力を追加
//			AddMovementInput(ForwardDirection, Forward);
//			AddMovementInput(RightDirection, Right);
//		}
//	}
//}
//
//void APokaPokaECCCharacter::DoLook(float Yaw, float Pitch)
//{
//
//}
//
//void APokaPokaECCCharacter::DoJumpStart()
//{
//	// キャラクターにジャンプを指示
//	Jump();
//}
//
//void APokaPokaECCCharacter::DoJumpEnd()
//{
//	// キャラクターにジャンプ終了を指示
//	StopJumping();
//}
//
//void APokaPokaECCCharacter::Interact(const FInputActionValue& Value)
//{
//	if (HeldItem)
//	{
//		// ------------------------------------
//		// モノを置く（手放す）処理
//		// ------------------------------------
//		// 1. キャラクターから切り離す
//		HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//
//		// 2. 目の前に置く（自分にめり込まないように少し前に出す）
//		FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
//		HeldItem->SetActorLocation(DropLocation);
//
//		// 3. 物理演算をオンにして床に落とす
//		//UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent());
//		if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent()))
//			///*if (PrimComp)*/
//		{
//			PrimComp->SetSimulatePhysics(true);
//		}
//
//		// 持っているアイテムをリセット
//		HeldItem = nullptr;
//		return;
//
//	}
//	else
//	{
//		// ------------------------------------
//		// モノを持つ（拾う）処理
//		// ------------------------------------
//		FVector Start = GetActorLocation() + FVector(0, 0, 50.0f);
//		FVector End = Start + (GetActorForwardVector() * InteractDistance);
//
//
//
//
//
//
//		FHitResult OutHit;
//		FCollisionShape Sphere = FCollisionShape::MakeSphere(50.0f); // 半径50の球で探す
//
//		FCollisionQueryParams Params;
//		Params.AddIgnoredActor(this); // 自分自身は無視する
//		//// デバッグ用の線を表示（これで判定が届いているか確認できます）
//		DrawDebugSphere(GetWorld(), End, 50.0f, 12, FColor::Red, false, 2.0f);
//		// 目の前を判定（Sphere Trace）
//		bool bHit = GetWorld()->SweepSingleByChannel(
//			OutHit, Start, End, FQuat::Identity, ECC_Visibility, Sphere, Params);
//
//		if (bHit && OutHit.GetActor())
//		{
//			AActor* HitActor = OutHit.GetActor();
//
//			// ★【ここに追加！】インターフェース（制作台）かどうかの判定
//			if (HitActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
//			{
//				// 制作台ならタイマーを起動して終了
//				IInteractableInterface::Execute_OnInteract(HitActor, this);
//				return;
//			}
//			// タグで「持てるモノ」か判定する（アイテム側に "Holdable" タグをつけておく）
//			if (HitActor->ActorHasTag(FName("Holdable")))
//			{
//				HeldItem = HitActor;
//
//				// 物理演算がオンになっていると手についてこないのでオフにする
//				UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent());
//				if (PrimComp)
//				{
//					PrimComp->SetSimulatePhysics(false);
//				}
//
//				// キャラクターのメッシュの指定したソケットにくっつける
//				HeldItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandSocketName);
//			}
//		}
//	}
//	//// 1. まず「目の前に何があるか」を先に調べる（持っている・いないに関わらず判定したい場合が多いため）
//	//FVector Start = GetActorLocation() + FVector(0, 0, 50.0f);
//	//FVector End = Start + (GetActorForwardVector() * InteractDistance);
//
//	//FHitResult OutHit;
//	//FCollisionShape Sphere = FCollisionShape::MakeSphere(50.0f);
//	//FCollisionQueryParams Params;
//	//Params.AddIgnoredActor(this);
//
//	//bool bHit = GetWorld()->SweepSingleByChannel(
//	//	OutHit, Start, End, FQuat::Identity, ECC_Visibility, Sphere, Params);
//
//	//// デバッグ用の線を表示（これで判定が届いているか確認できます）
//	//DrawDebugSphere(GetWorld(), End, 50.0f, 12, FColor::Red, false, 2.0f);
//
//	//if (bHit && OutHit.GetActor())
//	//{
//	//	AActor* HitActor = OutHit.GetActor();
//
//	//	// ★最優先：当たったものが「制作台（インターフェース実装者）」なら実行
//	//	if (HitActor->GetClass()->ImplementsInterface(UInteractableInterface::StaticClass()))
//	//	{
//	//		IInteractableInterface::Execute_OnInteract(HitActor, this);
//	//		return; // 制作台を触ったならここで終了
//	//	}
//	//}
//
//	//// 2. 制作台でない場合、持っているアイテムの「置く/拾う」処理へ
//	//if (HeldItem)
//	//{
//	//	// --- モノを置く処理 ---
//	//	HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//
//	//	FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
//	//	HeldItem->SetActorLocation(DropLocation);
//
//	//	if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent()))
//	//	{
//	//		PrimComp->SetSimulatePhysics(true);
//	//	}
//
//	//	HeldItem = nullptr;
//	//}
//	//else if (bHit && OutHit.GetActor())
//	//{
//	//	// --- モノを拾う処理 ---
//	//	AActor* HitActor = OutHit.GetActor();
//	//	if (HitActor->ActorHasTag(FName("Holdable")))
//	//	{
//	//		HeldItem = HitActor;
//
//	//		if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent()))
//	//		{
//	//			PrimComp->SetSimulatePhysics(false);
//	//		}
//
//	//		HeldItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HandSocketName);
//	//	}
//	//}
//}wned
//void ACharacternini::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void ACharacternini::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
//// Called to bind functionality to input
//void ACharacternini::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

