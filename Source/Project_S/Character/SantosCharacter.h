// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SantosCharacter.generated.h"


UENUM(BlueprintType)
enum class Animation_State : uint8
{
	Unarmed UMETA(DisplayName = "Unarmed"),
	Pistol UMETA(DisplayName = "Pistol"),
};

class USpringArmComponent;			// 카메라 셀카봉
class UCameraComponent;				// 카메라
class UInputMappingContext;			// 인풋매핑(실제 입력)
class UInputAction;					// 인풋액션(인풋했을 때 액션하기 위한 작업)
struct FInputActionValue;			// 인풋값(인풋값들)
class ULocomotionAnimInstance;		// 애님인스턴스


// 커스텀 로그
DECLARE_LOG_CATEGORY_EXTERN(LOG_CHARACTER, Log, All);

UCLASS()
class PROJECT_S_API ASantosCharacter : public ACharacter
{
	GENERATED_BODY()

	// 어디서든 수정가능, 블루프린트에서 읽기 가능, 카테고리 = 카메라, private이어도 접근 가능
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* CameraChangeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeAnimationAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (AllowPrivateAccess = "true"))
	ULocomotionAnimInstance* AnimInstance;

public:
	// Sets default values for this character's properties
	ASantosCharacter();

protected:
	// Called when the game starts or when spawned

	void Move(const FInputActionValue& Value);
	void MoveEnd(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void FCrouch(const FInputActionValue& Value);
	void CameraChange(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);
	void ChangeAnimation(const FInputActionValue& Value);
	UFUNCTION()
	void DodgeAnimSelect(float Value, bool ForwardOrRight);
	UFUNCTION()
	void DodgeAnimEnded(UAnimMontage* Montage, bool bInterrupted);


	bool CheckUpWall();

protected:
	// Called to bind functionality to input
	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;
public:

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class UCameraComponent* GetFirstCamera() const { return FirstCamera; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	bool IsSprint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	bool IsCrouch;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge", meta = (AllowPrivateAccess = "true"))
	bool IsDodge;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge", meta = (AllowPrivateAccess = "true"))
	float Forward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dodge", meta = (AllowPrivateAccess = "true"))
	float Right;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Turn", meta = (AllowPrivateAccess = "true"))
	float TurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Enum", meta = (AllowPrivateAccess = "true"))
	Animation_State E_AnimationState;
};
