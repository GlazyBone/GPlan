//Copyright GPlan Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "GPPlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UGPWeaponComponent;
class UInputAction;
struct FInputActionValue;

UCLASS()
class GPLAN_API AGPPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** 第三人称CameraBoom */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** 第三人称FollowCamera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;


public:
	AGPPlayerCharacter();

	/** 返回CameraBoom **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** 返回FollowCamera **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	virtual void BeginPlay() override;

	/** 武器组件：武器的生成/装备/开火全部归它管 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UGPWeaponComponent> WeaponComponent;

public:

	//武器开火
	void StartWeaponFire();

	//武器停火
	void StopWeaponFire();

};

