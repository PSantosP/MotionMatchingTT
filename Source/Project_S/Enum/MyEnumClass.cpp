// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_s/Enum/MyEnumClass.h"


UMyEnumClass::UMyEnumClass()
{
	E_AnimationState = Animation_State::Unarmed;
	E_MovementInput = Movement_Input::Forward;
}

void UMyEnumClass::SetAnimationState(Animation_State State)
{
	E_AnimationState = State;
}

void UMyEnumClass::SetMovementInput(Movement_Input Input)
{
	E_MovementInput = Input;
}
