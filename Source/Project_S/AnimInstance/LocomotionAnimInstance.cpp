// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_S/AnimInstance/LocomotionAnimInstance.h"
#include "Project_S/Character/SantosCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ULocomotionAnimInstance::NativeInitializeAnimation()
{
	Character = Cast<ASantosCharacter>(GetOwningActor());
	CharacterMovementComponent = Character->GetCharacterMovement();
}

void ULocomotionAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (Character != nullptr)
	{
		// 캐릭터의 현재 속도를 가져옴
		// 지면 속도는 xy축으로만 계산되므로 상하이동은 영향을 주지 않음
		Velocity = CharacterMovementComponent->Velocity;
		GroundSpeed = Velocity.Size2D();

		// 지면속도가 3.0f보다 크고 현재 가속도가 zero가 아닌지 확인이 되면 움직이는 것으로 확인
		ShouldMove = (GroundSpeed > 3.f && CharacterMovementComponent->GetCurrentAcceleration() != FVector::ZeroVector);

		IsFalling = CharacterMovementComponent->IsFalling();
	}
}
