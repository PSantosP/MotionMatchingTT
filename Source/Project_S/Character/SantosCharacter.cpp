// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_S/Character/SantosCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"


// 커스텀 로그 정의
DEFINE_LOG_CATEGORY(LOG_CHARACTER);

// Sets default values
ASantosCharacter::ASantosCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);							// 캡슐 사이즈 초기화

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;										// pawn의 yaw회전을 회전하도록 설정
	bUseControllerRotationRoll = false;


	GetCharacterMovement()->bOrientRotationToMovement = true;						// 캐릭터를 회전의 움직임에 맞게 회전하는지 여부					
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 120.0f, 0.0f);			// 캐릭터의 회전 속도

	GetCharacterMovement()->JumpZVelocity = 700.f;							// 점프 시 속도 값
	GetCharacterMovement()->AirControl = 0.35f;								// 공중에서 움직임 제어 값
	GetCharacterMovement()->MaxWalkSpeed = 300.f;							// 캐릭터 걷기 최대 속도
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;						// 최소 걷기 속도
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;			// 걷는 도안의 감속 설정
	GetCharacterMovement()->BrakingDecelerationFalling = 0.f;			// 공중에서 떨어지는 감속 설정
	GetCharacterMovement()->bOrientRotationToMovement = false;				// 캐릭터는 이동 방향에 상관없이 항상 일정한 방향으로 향하게 설정


	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->MaxAcceleration = 1500.f;
	GetCharacterMovement()->CrouchedHalfHeight = 60.f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 350.0f;
	CameraBoom->bUsePawnControlRotation = true;								// 부모가 회전 하면 같이 돌게 설정
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 15.f;
	CameraBoom->CameraRotationLagSpeed = 20.f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;							// 카메라는 같이 회전하지 않게끔 설정
	FollowCamera->bAutoActivate = true;


	FirstCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstCamera"));
	FirstCamera->SetRelativeRotation(FRotator(0.f, 90.f, -90.f));
	FirstCamera->SetRelativeLocation(FVector(5.f, 10.f, 0.f));
	FirstCamera->SetupAttachment(GetMesh());
	FirstCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("head"));
	FirstCamera->bAutoActivate = false;
	FirstCamera->bUsePawnControlRotation = true;
	//FirstCamera->Activate(false);
	// 애님 인스턴스를 불러오려고 했는데 굳이 그럴 필요가 없다는 것을 알음
	//static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Project_S_Content/Animations/ABP_LocomotionAnimInstance.ABP_LocomotionAnimInstance_C'"));
	//if (AnimInstance.Succeeded())
	//{
	//	GetMesh()->AnimClass = AnimInstance.Class;
	//}
}


// Called when the game starts or when spawned

void ASantosCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 캐릭터의 부모는 Pawn 즉 Pawn의 Controller의 다운캐스팅으로 인해 APlayerController를 가져온다.
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// ULocalPlayer의 GetSubsystem은 어떤 특정 서브시스템 클래스를 불러오는 역할을 한다.
		// 그 때 UEnhacnedINputLocalPlayerSubsystem을 불러온다.
		// 즉 서브시스템을 불러와 매핑을 해놓으면 향상된 입력 서브시스템을 사용할 수 있게 한다.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 0은 기본적으로 첫번째 매핑 컨텍스트를 나타낸다.
			// 예를 들어 동물을 탔을 때 입력이 달라져야 한다면 1번째의 동물 입력 시스템을 만들어 놓고
			// 매핑을 바꾸면 된다.
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


// Called to bind functionality to input
void ASantosCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// PlayerInputComponent에 UEnhancedInputComponent를 캐스팅해와서 액션을 집어넣음
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASantosCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASantosCharacter::Look);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASantosCharacter::Sprint);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASantosCharacter::Sprint);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ASantosCharacter::FCrouch);

		EnhancedInputComponent->BindAction(CameraChangeAction, ETriggerEvent::Started, this, &ASantosCharacter::CameraChange);
	}

}


void ASantosCharacter::Move(const FInputActionValue& Value)
{
	// 값을 받아온다
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 회전 방향을 통해 각 방향을 얻음
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);		// 앞의 방향을 얻음

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);			// 오른쪽의 방향을 얻음

		// 이동
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASantosCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASantosCharacter::Sprint(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		if (IsCrouch == false)
		{
			IsSprint = Value.Get<bool>();

			GetCharacterMovement()->MaxWalkSpeed = IsSprint == true ? 600.f : 300.f;
		}
	}
}

void ASantosCharacter::FCrouch(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		if (IsCrouch)
		{
			IsCrouch = false;
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
			UnCrouch();
		}
		else
		{
			IsCrouch = true;
			IsSprint = false;
			GetCharacterMovement()->MaxWalkSpeed = 250.f;
			Crouch();
		}
	}
}

void ASantosCharacter::CameraChange(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		if (GetFirstCamera()->IsActive())
		{
			GetFirstCamera()->SetActive(false);
			GetFollowCamera()->SetActive(true);
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
		else
		{
			GetFirstCamera()->SetActive(true);
			GetFollowCamera()->SetActive(false);
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}

	}
}
