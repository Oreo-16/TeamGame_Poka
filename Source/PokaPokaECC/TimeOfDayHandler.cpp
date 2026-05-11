#include "TimeOfDayHandler.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"

ATimeOfDayHandler::ATimeOfDayHandler()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATimeOfDayHandler::BeginPlay()
{
	Super::BeginPlay();
}

void ATimeOfDayHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SunLight)
	{
		float RotationSpeed = 360.0f / DayLengthInSeconds;
		FRotator NewRotation = SunLight->GetActorRotation();
		NewRotation.Pitch += RotationSpeed * DeltaTime;
		SunLight->SetActorRotation(NewRotation);
	}

	if (SkyLight && SunLight)
	{
		if (USkyLightComponent* LightComp = SkyLight->GetLightComponent())
		{
			float CurrentPitch = SunLight->GetActorRotation().Pitch;
			float NewIntensity = (CurrentPitch < 0.0f) ? 0.0f : 1.0f;
			LightComp->SetIntensity(0.1f);
		}
	}
}