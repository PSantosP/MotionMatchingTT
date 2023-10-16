// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SantosCharacter.generated.h"


class USpringArmComponent;			// 카메라 셀카봉
class UCameraComponent;				// 카메라
class UInputMappingContext;			// 인풋매핑(실제 입력)
class UInputAction;					// 인풋액션(인풋했을 때 액션하기 위한 작업)
struct FInputActionValue;			// 인풋값(인풋값들)



// 커스텀 로그
DECLARE_LOG_CATEGORY_EXTERN(LOG_CHARACTER, Log, All);

UCLASS()
class PROJECT_S_API ASantosCharacter : public ACharacter
{
	GENERATED_BODY()

	// 어디서든 수정가능, 블루프린트에서 읽기 가능, 카테고리 = 카메라, private이어도 접근 가능
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

public:
	// Sets default values for this character's properties
	ASantosCharacter();

protected:
	// Called when the game starts or when spawned

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);

protected:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;
public:

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	bool IsSprint;
};
