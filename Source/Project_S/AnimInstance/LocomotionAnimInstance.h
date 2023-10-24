// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LocomotionAnimInstance.generated.h"



class ASantosCharacter;
class UCharacterMovementComponent;
class USoundCue;

/**
 * 
 */
UCLASS()
class PROJECT_S_API ULocomotionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	ULocomotionAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	void GetDirectionAngle();
	void GetOrientationAngle();
	void GetIsSprint();
	void GetIsCrouch();
	void GetYaw();
	void GetPitch();
	void GetTurnRate();
	void GetAnimationState();


public:
	UFUNCTION(BluePrintCallable)
	void GetAnimNotifyTrace(bool left, bool land);

	UAnimMontage* SelectMontage(bool Select, bool ForwardOrRight);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "References", meta = (AllowPrivateAccess = "true"))
	ASantosCharacter* Character;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "References", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* MovementComponent;


public :
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Essential Movement Data", meta = (AllowPrivateAccess = "true"))
	FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Essential Movement Data", meta = (AllowPrivateAccess = "true"))
	float GroundSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Essential Movement Data", meta = (AllowPrivateAccess = "true"))
	bool ShouldMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Essential Movement Data", meta = (AllowPrivateAccess = "true"))
	bool IsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Essential Movement Data", meta = (AllowPrivateAccess = "true"))
	bool IsSprint;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Essential Movement Data", meta = (AllowPrivateAccess = "true"))
	bool IsCrouch;
	//UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Essential Movement Data", meta = (AllowPrivateAccess = "true"))
	//Animation_State E_AnimationState;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Direction", meta = (AllowPrivateAccess = "true"))
	float DirectionAngle;
	//UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Direction", meta = (AllowPrivateAccess = "true"))
	//Movement_Input E_MovementInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Direction", meta = (AllowPrivateAccess = "true"))
	float F_OrientationAngle;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Direction", meta = (AllowPrivateAccess = "true"))
	float R_OrientationAngle;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Direction", meta = (AllowPrivateAccess = "true"))
	float B_OrientationAngle;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Direction", meta = (AllowPrivateAccess = "true"))
	float L_OrientationAngle;


public:

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	float TurnYaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	float TurnYawLastTick;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	float TurnYawChangeOver;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	float TurnYawOffset;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	bool DoOnceTurn;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	float TurnCurveValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	float LastTurnCurveValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeFMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeBMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeRMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeLMontage;
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	float Pitch;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	float TurnDirectionAngle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	TArray<USoundCue*> NormalFootSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	TArray<USoundCue*> GlassFootSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	TArray<USoundCue*> LandSoundCue;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	float FootSoundVolume = 2.5f;

};
