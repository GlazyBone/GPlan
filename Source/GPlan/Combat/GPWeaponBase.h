//Copyright GPlan Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GPWeaponBase.generated.h"

class USkeletalMeshComponent;
class AGPProjectile;
class USoundBase;
class UNiagaraSystem;

UCLASS()
class GPLAN_API AGPWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGPWeaponBase();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	/** 武器模型 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	/** 武器子弹 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Fire")
	TSubclassOf<AGPProjectile> ProjectileClass;

	/** 开火间隔 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Fire")
	float FireRate = 0.15f;

	/** 枪口插槽名 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Fire")
	FName MuzzleSocketName = TEXT("Muzzle");

	/** 开枪音效 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Effects")
	TObjectPtr<USoundBase> FireSound;

	/** 枪口火光粒子 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Effects")
	TObjectPtr<UNiagaraSystem> MuzzleFire;

	//内部状态
	FTimerHandle FireTimerHandle;//开火计时器

	//获取枪口位置
	FVector GetMuzzleLocation() const;

	//获取瞄准方向
	FRotator GetAimRotation() const;

	//单次开火
	void FireOnce();    //真正发射一颗子弹：取枪口位置 → SpawnProjectile → 把伤害值传给子弹

	//判断是否开火
	bool bIsFiring = false;

public:
	/** 开始开火（按下左键调用） */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void StartFire();

	/** 停止开火（松开左键调用） */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void StopFire();

	/** 获取武器骨骼网格体指针 */
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }

};
