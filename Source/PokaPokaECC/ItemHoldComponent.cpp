#include "ItemHoldComponent.h"
#include "GameFramework/Character.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"

UItemHoldComponent::UItemHoldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UItemHoldComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UItemHoldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 1. ŽèŒ³‚Ö‚Ìˆø‚«Šñ‚¹
	if (bIsItemSnapping && HeldItem && OwnerCharacter)
	{
		FVector CurrentLoc = HeldItem->GetRootComponent()->GetRelativeLocation();
		FRotator CurrentRot = HeldItem->GetRootComponent()->GetRelativeRotation();

		FVector NewLoc = FMath::VInterpTo(CurrentLoc, FVector::ZeroVector, DeltaTime, ItemSnapSpeed);
		FRotator NewRot = FMath::RInterpTo(CurrentRot, FRotator::ZeroRotator, DeltaTime, ItemSnapSpeed);

		HeldItem->GetRootComponent()->SetRelativeLocationAndRotation(NewLoc, NewRot);

		if (CurrentLoc.Equals(FVector::ZeroVector, 2.0f))
		{
			HeldItem->GetRootComponent()->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
			bIsItemSnapping = false;
		}
	}

	// 2. Š÷‚Ö‚Ì”z’uˆÚ“®
	if (bIsItemPlacing && PlacingItem)
	{
		FVector CurrentLoc = PlacingItem->GetActorLocation();
		FRotator CurrentRot = PlacingItem->GetActorRotation();

		FVector NewLoc = FMath::VInterpTo(CurrentLoc, PlaceTargetLocation, DeltaTime, ItemSnapSpeed);
		FRotator NewRot = FMath::RInterpTo(CurrentRot, PlaceTargetRotation, DeltaTime, ItemSnapSpeed);

		PlacingItem->SetActorLocationAndRotation(NewLoc, NewRot);

		if (CurrentLoc.Equals(PlaceTargetLocation, 2.0f))
		{
			PlacingItem->SetActorLocationAndRotation(PlaceTargetLocation, PlaceTargetRotation);
			bIsItemPlacing = false;
			PlacingItem = nullptr;
		}
	}
}

void UItemHoldComponent::PrimaryInteract()
{
	if (!OwnerCharacter) return;

	FVector Start = OwnerCharacter->GetActorLocation();
	FVector Forward = OwnerCharacter->GetActorForwardVector();
	FVector OverlapCenter = Start + (Forward * (InteractDistance * 0.6f));
	FCollisionShape Sphere = FCollisionShape::MakeSphere(70.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);
	if (HeldItem) Params.AddIgnoredActor(HeldItem);

	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByChannel(Overlaps, OverlapCenter, FQuat::Identity, ECC_Visibility, Sphere, Params);

	if (HeldItem)
	{
		AActor* FoundCounter = nullptr;
		for (const FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor() && Overlap.GetActor()->ActorHasTag("Counter"))
			{
				FoundCounter = Overlap.GetActor();
				break;
			}
		}

		if (FoundCounter)
		{
			HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			PlaceTargetLocation = FoundCounter->GetActorLocation() + FVector(0.0f, 0.0f, 90.0f);
			PlaceTargetRotation = FRotator::ZeroRotator;

			if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent()))
			{
				PrimComp->SetSimulatePhysics(false);
				PrimComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}

			PlacingItem = HeldItem;
			bIsItemPlacing = true;
			HeldItem = nullptr;
			bIsItemSnapping = false;
		}
		else
		{
			// “Š‚°”ò‚Î‚µ
			HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent()))
			{
				PrimComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				PrimComp->SetSimulatePhysics(true);
				FVector TossDir = (Forward + FVector(0, 0, 0.5f)).GetSafeNormal();
				PrimComp->AddImpulse(TossDir * 300.0f * PrimComp->GetMass());
			}
			HeldItem = nullptr;
			bIsItemSnapping = false;
		}
	}
	else
	{
		for (const FOverlapResult& Overlap : Overlaps)
		{
			AActor* HitActor = Overlap.GetActor();
			if (HitActor && HitActor->ActorHasTag("Holdable"))
			{
				if (HitActor == PlacingItem) { bIsItemPlacing = false; PlacingItem = nullptr; }

				HeldItem = HitActor;
				if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(HeldItem->GetRootComponent()))
				{
					PrimComp->SetSimulatePhysics(false);
					PrimComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}
				HeldItem->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, HandSocketName);
				bIsItemSnapping = true;
				break;
			}
		}
	}
}