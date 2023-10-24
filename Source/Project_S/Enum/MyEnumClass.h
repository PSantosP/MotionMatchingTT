// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyEnumClass.generated.h"


UENUM(BlueprintType)
enum class Animation_State : uint8
{
	Unarmed UMETA(DisplayName = "Unarmed"),
	Pistol UMETA(DisplayName = "Pistol"),
};


UENUM(BlueprintType)
enum class Movement_Input : uint8
{
	Forward		UMETA(DisplayName = "Forward"),
	Right		UMETA(DisplayName = "Right"),
	Backward	UMETA(DisplayName = "Backward"),
	Left		UMETA(DisplayName = "Left"),
};

/**
 * 
 */
UCLASS()
class PROJECT_S_API UMyEnumClass : public UObject
{
	GENERATED_BODY()
	
	UMyEnumClass();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enum", meta = (AllowPrivateAccess = "true"))
	Animation_State E_AnimationState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enum", meta = (AllowPrivateAccess = "true"))
	Movement_Input E_MovementInput;

public:
	void SetAnimationState(Animation_State State);
	void SetMovementInput(Movement_Input Input);

	Animation_State GetAnimationState() { return E_AnimationState; }
	Movement_Input GetMovementInput() { return E_MovementInput; }
};
