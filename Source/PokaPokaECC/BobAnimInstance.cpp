#include "BobAnimInstance.h"
#include "GameFramework/Pawn.h"

void UBobAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* OwnerPawn = TryGetPawnOwner();

    if (OwnerPawn)
    {
        Speed = OwnerPawn->GetVelocity().Size();
    }
    else
    {
        Speed = 0.0f;
    }
}