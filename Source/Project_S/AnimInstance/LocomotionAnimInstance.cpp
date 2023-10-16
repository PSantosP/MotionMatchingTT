// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_S/AnimInstance/LocomotionAnimInstance.h"
#include "Project_S/Character/SantosCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ULocomotionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	APawn* Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Character = Cast<ASantosCharacter>(Pawn);
		MovementComponent = Character->GetCharacterMovement();
	}
}

void ULocomotionAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (Character != nullptr)
	{
		// 캐릭터의 현재 속도를 가져옴
		// 지면 속도는 xy축으로만 계산되므로 상하이동은 영향을 주지 않음
		Velocity = MovementComponent->Velocity;
		GroundSpeed = Velocity.Size2D();

		// 지면속도가 3.0f보다 크고 현재 가속도가 zero가 아닌지 확인이 되면 움직이는 것으로 확인
		ShouldMove = (GroundSpeed > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector);

		IsFalling = MovementComponent->IsFalling();
		GetDirectionAngle();
	}
}

void ULocomotionAnimInstance::GetDirectionAngle()
{
	// 방향을 구한다.
	// CalculateDirection을 통해 2D 평면에서 두 점 사이의 각도나 방향을 계산한다.
	FVector NewVector = FVector(Velocity.X, Velocity.Y, 0.f);
	Direction = FRotator::NormalizeAxis(CalculateDirection(NewVector, Character->GetActorRotation()));

	/*UE_LOG(LogTemp, Warning, TEXT("현재 회전값 : %f"), Direction);*/
	if (Direction > -45.f && Direction <= 45.f)			// Direction의 회전값이 -45도와 45도 사이에 있으면 Forward방향
	{
		E_MovementInput = Movement_Input::Forward;
	}
	else if (Direction > 45.f && Direction <= 135.f)	// Direction의 회전값이 45도와 135도 사이에 있으면 Right방향
	{
		E_MovementInput = Movement_Input::Right;
	}
	else if (Direction > 135.f || Direction <= -135.f)	// Direction의 회전값이 135도와 -135도 사이에 있으면 Backward방향
	{
		E_MovementInput = Movement_Input::Backward;
	}
	else if (Direction > -135.f && Direction <= -45.f)	// Direction의 회전값이 -45도와 45도 사이에 있으면 Left방향
	{
		E_MovementInput = Movement_Input::Left;
	}

	GetOrientationAngle();
	GetIsSprint();
}

void ULocomotionAnimInstance::GetOrientationAngle()
{
	F_OrientationAngle = Direction - 0.f;
	R_OrientationAngle = Direction - 90.f;
	B_OrientationAngle = Direction - 180.f;
	L_OrientationAngle = Direction - (-90.f);
}

void ULocomotionAnimInstance::GetIsSprint()
{
	if (IsValid(Character))
	{
		IsSprint = Character->IsSprint;
	}
}
