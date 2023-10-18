// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_S/AnimInstance/LocomotionAnimInstance.h"
#include "Project_S/Character/SantosCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ULocomotionAnimInstance::ULocomotionAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeF(TEXT("/Script/Engine.AnimMontage'/Game/Project_S_Content/Animations/Dodge/DiveRoll_F_Montage.DiveRoll_F_Montage'"));
	if (DodgeF.Succeeded())
	{
		DodgeFMontage = DodgeF.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeB(TEXT("/Script/Engine.AnimMontage'/Game/Project_S_Content/Animations/Dodge/DiveRoll_B_Montage.DiveRoll_B_Montage'"));
	if (DodgeB.Succeeded())
	{
		DodgeBMontage = DodgeB.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeR(TEXT("/Script/Engine.AnimMontage'/Game/Project_S_Content/Animations/Dodge/DiveRoll_R_Montage.DiveRoll_R_Montage'"));
	if (DodgeR.Succeeded())
	{
		DodgeRMontage = DodgeR.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeL(TEXT("/Script/Engine.AnimMontage'/Game/Project_S_Content/Animations/Dodge/DiveRoll_L_Montage.DiveRoll_L_Montage'"));
	if (DodgeL.Succeeded())
	{
		DodgeLMontage = DodgeL.Object;
	}
}

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
	GetIsCrouch();
	GetYaw();
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

void ULocomotionAnimInstance::GetIsCrouch()
{
	if (IsValid(Character))
	{
		//UE_LOG(LogTemp, Warning, TEXT("현재 앉은 상태 : %s"), IsCrouch ? TEXT("true") : TEXT("false"));
		IsCrouch = Character->IsCrouch;
	}
}

void ULocomotionAnimInstance::GetYaw()
{
	TurnYawLastTick = TurnYaw;
	TurnYaw = Character->GetActorRotation().Yaw;
	//UE_LOG(LogTemp, Warning, TEXT("TurnYaw : %f"), TurnYaw);
	//UE_LOG(LogTemp, Warning, TEXT("TurnLastTick : %f"), TurnYawLastTick);
	TurnYawChangeOver = TurnYawLastTick - TurnYaw;
	
	// 캐릭터가 이동 중이거나 떨어질 때
	if (ShouldMove || IsFalling)
	{
		// 회전 오프셋 값을 0
		TurnYawOffset = 0.0f;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("IsTurnGetCurveValue : %f"), GetCurveValue(TEXT("IsTurn")));
		//UE_LOG(LogTemp, Warning, TEXT("DistanceGetCurveValue : %f"), GetCurveValue(TEXT("DistanceCurve")));
		TurnYawOffset = FRotator::NormalizeAxis(TurnYawChangeOver + TurnYawOffset);

		// 턴했다면
		if (FMath::IsNearlyEqual(GetCurveValue(TEXT("IsTurn")), 1.0f, 0.001f))
		{
			if (DoOnceTurn == false)
			{
				// 현재 Distance커브값을 TurnCurveValue로 설정
				DoOnceTurn = true;
				TurnCurveValue = GetCurveValue(TEXT("DistanceCurve"));
			}
			LastTurnCurveValue = TurnCurveValue;
			TurnCurveValue = GetCurveValue(TEXT("DistanceCurve"));
			TurnYawOffset = TurnYawOffset - ((LastTurnCurveValue - TurnCurveValue) * (TurnYawOffset > 0.f ? -1.0f : 1.0f));
		}
		else
		{
			DoOnceTurn = false;
		}
	}
	/*UE_LOG(LogTemp, Warning, TEXT("TurnYaw : %f"), TurnYawOffset);*/
}

UAnimMontage* ULocomotionAnimInstance::SelectMontage(bool Select, bool ForwardOrRight)
{
	if (ForwardOrRight)
	{
		// Forward
		if (Select)
		{
			return DodgeFMontage;
		}
		else
		{
			return DodgeBMontage;
		}
	}
	else
	{
		// Right
		if (Select)
		{
			return DodgeRMontage;
		}
		else
		{
			return DodgeLMontage;
		}
	}
}
