// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SanPawn.h"

// 생성자
// Super(ObjetInitializer)를 호출하여 부모 클래스의 생성자를 초기화 한다.
// 이후 ObjectInitialzier를 사용하여 초기화 작업을 수행 할 수 있음.
// 객체가 만들어질 때 초기화할 속성을 설정하거나 다른 초기화 관련 작업을 수행할 때 사용
ASanPawn::ASanPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASanPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASanPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASanPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

