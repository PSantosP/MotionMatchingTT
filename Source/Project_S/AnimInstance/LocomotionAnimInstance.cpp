// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_S/AnimInstance/LocomotionAnimInstance.h"
#include "Project_S/Character/SantosCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ULocomotionAnimInstance::ULocomotionAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeF(TEXT("/Script/Engine.AnimMontage'/Game/Project_S_Content/Animations/Unarmed/Dodge/DiveRoll_F_Montage.DiveRoll_F_Montage'"));
	if (DodgeF.Succeeded())
	{
		DodgeFMontage = DodgeF.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeB(TEXT("/Script/Engine.AnimMontage'/Game/Project_S_Content/Animations/Unarmed/Dodge/DiveRoll_B_Montage.DiveRoll_B_Montage'"));
	if (DodgeB.Succeeded())
	{
		DodgeBMontage = DodgeB.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeR(TEXT("/Script/Engine.AnimMontage'/Game/Project_S_Content/Animations/Unarmed/Dodge/DiveRoll_R_Montage.DiveRoll_R_Montage'"));
	if (DodgeR.Succeeded())
	{
		DodgeRMontage = DodgeR.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DodgeL(TEXT("/Script/Engine.AnimMontage'/Game/Project_S_Content/Animations/Unarmed/Dodge/DiveRoll_L_Montage.DiveRoll_L_Montage'"));
	if (DodgeL.Succeeded())
	{
		DodgeLMontage = DodgeL.Object;
	}

	NormalFootSoundCue.Add(LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Project_S_Content/Sound/Footsteps/WalkandRun/Left_FootFX.Left_FootFX'")));
	NormalFootSoundCue.Add(LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Project_S_Content/Sound/Footsteps/WalkandRun/Right_FootFX.Right_FootFX'")));
	GlassFootSoundCue.Add(LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Project_S_Content/Sound/TestCue.TestCue'")));
	GlassFootSoundCue.Add(LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Project_S_Content/Sound/TestCue.TestCue'")));
	LandSoundCue.Add(LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Project_S_Content/Sound/Footsteps/Land/Land_SoundFX.Land_SoundFX'")));
	LandSoundCue.Add(LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Project_S_Content/Sound/TestCue.TestCue'")));
	
	USoundCue* BGMCue = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/StarterContent/Audio/Starter_Background_Cue.Starter_Background_Cue'"));
	if (BGMCue)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), BGMCue, FVector::ZeroVector);
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
		GetOrientationAngle();
		GetIsSprint();
		GetIsCrouch();
		GetYaw();
		GetPitch();
		GetTurnRate();
	}
}

void ULocomotionAnimInstance::GetDirectionAngle()
{
	// 방향을 구한다.
	// CalculateDirection을 통해 2D 평면에서 두 점 사이의 각도나 방향을 계산한다.
	FVector NewVector = FVector(Velocity.X, Velocity.Y, 0.f);
	DirectionAngle = FRotator::NormalizeAxis(CalculateDirection(NewVector, Character->GetActorRotation()));

	/*UE_LOG(LogTemp, Warning, TEXT("현재 회전값 : %f"), Direction);*/
	if (DirectionAngle > -45.f && DirectionAngle <= 45.f)			// Direction의 회전값이 -45도와 45도 사이에 있으면 Forward방향
	{
		E_MovementInput = Movement_Input::Forward;
	}
	else if (DirectionAngle > 45.f && DirectionAngle <= 135.f)	// Direction의 회전값이 45도와 135도 사이에 있으면 Right방향
	{
		E_MovementInput = Movement_Input::Right;
	}
	else if (DirectionAngle > 135.f || DirectionAngle <= -135.f)	// Direction의 회전값이 135도와 -135도 사이에 있으면 Backward방향
	{
		E_MovementInput = Movement_Input::Backward;
	}
	else if (DirectionAngle > -135.f && DirectionAngle <= -45.f)	// Direction의 회전값이 -45도와 45도 사이에 있으면 Left방향
	{
		E_MovementInput = Movement_Input::Left;
	}


}

void ULocomotionAnimInstance::GetOrientationAngle()
{
	F_OrientationAngle = DirectionAngle - 0.f;
	R_OrientationAngle = DirectionAngle - 90.f;
	B_OrientationAngle = DirectionAngle - 180.f;
	L_OrientationAngle = DirectionAngle - (-90.f);
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

	TurnYawChangeOver = TurnYawLastTick - TurnYaw;
	
	// 캐릭터가 이동 중이거나 떨어질 때
	if (ShouldMove || IsFalling)
	{
		// 회전 오프셋 값을 0
		TurnYawOffset = 0.0f;
	}
	else
	{
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
}

void ULocomotionAnimInstance::GetPitch()
{
	if (Character != nullptr)
	{
		FRotator AimRotator = Character->GetBaseAimRotation();
		FRotator ActorRotator = Character->GetActorRotation();
		FRotator Delta = AimRotator - ActorRotator;
		Pitch = Delta.Pitch;
	}

}

void ULocomotionAnimInstance::GetTurnRate()
{
	if (Character != nullptr)
	{
		// float값을 보간해주는 역할
		TurnDirectionAngle = FMath::FInterpTo(0.f, Character->TurnRate, GetWorld()->GetDeltaSeconds(), 10.f);
	}
}



void ULocomotionAnimInstance::GetAnimNotifyTrace(bool left, bool land)
{
	if (Character != nullptr)
	{
		TArray<FHitResult> HitResults;
		FVector Start = Character->GetCapsuleComponent()->GetComponentLocation();	// 시작 위치

		float SphereRadius = 10.f;		// 구 반지름

		FVector End = Start - FVector(0.f, 0.f, 120.f);		// 스피어의 방향을 나타내는 벡터

		FCollisionQueryParams CollisionParams;
		CollisionParams.bReturnPhysicalMaterial = true;			// 물리 머티리얼을 찾기위해 사용
		bool bHit = GetWorld()->SweepMultiByChannel(
			HitResults,			// 충돌 정보 저장
			Start,				// 시작 위치
			End,				// 끝 위치 (방향 벡터를 더해 구의 위치를 나타냄)
			FQuat(),			// 회전 (기본값 사용)
			ECC_Visibility,		// 채널 (원하는 채널로 설정
			FCollisionShape::MakeSphere(SphereRadius),		// Sphere Trace를 위한 구체
			CollisionParams
			);

		if (bHit)
		{		// 디버깅을 위해 Trace를 시각적으로 표시
			for (const FHitResult& HitResult : HitResults)
			{
				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SphereRadius, 12, FColor::Red, false, 5.0f);
				if (HitResult.PhysMaterial != nullptr)
				{
					if (left == true && land == false)
					{
						switch (HitResult.PhysMaterial->SurfaceType)
						{
						case SurfaceType1:
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), NormalFootSoundCue[0], HitResult.Location, FootSoundVolume);
							break;
						case SurfaceType2:
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), GlassFootSoundCue[0], HitResult.Location, FootSoundVolume);
							break;
						default:
							break;
						}

					}
					else if (left == false && land == false)
					{
						switch (HitResult.PhysMaterial->SurfaceType)
						{
						case SurfaceType1:
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), NormalFootSoundCue[1], HitResult.Location, FootSoundVolume);
							break;
						case SurfaceType2:
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), GlassFootSoundCue[1], HitResult.Location, FootSoundVolume);
							break;
						default:
							break;
						}

					}
					else if (land == true)
					{
						switch (HitResult.PhysMaterial->SurfaceType)
						{
						case SurfaceType1:
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), LandSoundCue[0], HitResult.Location, FootSoundVolume);
							break;
						case SurfaceType2:
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), LandSoundCue[1], HitResult.Location, FootSoundVolume);
							break;
						default:
							break;
						}
					}
				}
			}
		}
		else
		{
			FVector EndTrace = Start + FVector(0.0f, 0.0f, 1.0f);
			DrawDebugLine(GetWorld(), Start, EndTrace, FColor::Green, false, 5.0f);
		}


	}
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
