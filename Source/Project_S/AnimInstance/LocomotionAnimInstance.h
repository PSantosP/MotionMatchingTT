// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LocomotionAnimInstance.generated.h"


class ASantosCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class PROJECT_S_API ULocomotionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;


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
};
