// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SanPawn.generated.h"

class AController;
class UObject;
struct FFrame;

UCLASS()
class PROJECT_S_API ASanPawn : public APawn
{
	GENERATED_BODY()

public:
	// FobjectInitializer를 사용하는 것은 객체 초기화에 관련된 작업을 수행하는데 도움을 줌
	// 이렇게 설정하면 기본값 초기화 및 속성 설정과 같은 중요한 초기화 작업을 수행 할 수 있음
	// 
	ASanPawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
