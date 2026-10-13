//Copyright GPlan Project. All Rights Reserved.


#include "GPPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Engine/LocalPlayer.h"
#include "Characters/GPPlayerCharacter.h"

AGPPlayerController::AGPPlayerController()
{
}

void AGPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}


}

void AGPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{

		//Jumping
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AGPPlayerController::Input_Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGPPlayerController::Input_StopJumping);
		}

		//Moving
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGPPlayerController::Input_Move);
		}

		//Looking
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGPPlayerController::Input_Look);
		}
		//Firing
		if (FireAction)
		{
			//按下开火键
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AGPPlayerController::Input_StartFire);

			//松开开火键
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AGPPlayerController::Input_StopFire);

			//输入取消时也停止开火
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Canceled, this, &AGPPlayerController::Input_StopFire);
		}
	}

}

void AGPPlayerController::Input_Move(const FInputActionValue& Value)
{
	//input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		//find out which way is forward
		const FRotator Rotation = ControlledPawn->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		//get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//add movement 
		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGPPlayerController::Input_Look(const FInputActionValue& Value)
{
	//input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	//add yaw and pitch input to controller
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);

}

void AGPPlayerController::Input_Jump()
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->Jump();
	}
}

void AGPPlayerController::Input_StopJumping()
{
	if (ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn()))
	{
		ControlledCharacter->StopJumping();
	}
}

void AGPPlayerController::Input_StartFire()
{
	if (AGPPlayerCharacter* ControlledCharacter = Cast<AGPPlayerCharacter>(GetPawn()))
	{
		ControlledCharacter->StartWeaponFire();
	}
}

void AGPPlayerController::Input_StopFire()
{
	if (AGPPlayerCharacter* ControlledCharacter = Cast<AGPPlayerCharacter>(GetPawn()))
	{
		ControlledCharacter->StopWeaponFire();
	}
}
