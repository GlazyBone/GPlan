//Copyright GPlan Project. All Rights Reserved.


#include "Combat/GPWeaponBase.h"
#include "Combat/GPProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"
#include "GPlan.h"

AGPWeaponBase::AGPWeaponBase()
{
	//关闭每帧调用
	PrimaryActorTick.bCanEverTick = false;

	//创建骨骼网格体并设为根组件
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	//关闭武器模型碰撞
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AGPWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

FVector AGPWeaponBase::GetMuzzleLocation() const
{
	//如果找不到枪械或枪械插槽则返回枪械位置
	if (!WeaponMesh || !WeaponMesh->DoesSocketExist(MuzzleSocketName))
	{
		return GetActorLocation();
	}

	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

FRotator AGPWeaponBase::GetAimRotation() const
{
	//获取武器的持有者
	const APawn* OwningPawn = Cast<APawn>(GetOwner());

	
	if (!OwningPawn)
	{
		UE_LOG(LogGP, Warning, TEXT("[%s] 未找到武器持有者"), *GetName());

		return GetActorRotation();
	}

	return OwningPawn->GetBaseAimRotation();

}

void AGPWeaponBase::FireOnce()
{
	if (!ProjectileClass)
	{
		UE_LOG(LogGP, Warning, TEXT("%s未配置子弹类"), *GetName());
		return;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	//获取子弹的瞄准方向和生成位置
	const FVector SpawnLocation = GetMuzzleLocation();
	const FRotator SpawnRotation = GetAimRotation();

	//配置子弹的生成参数
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	if (!SpawnParams.Instigator)
	{
		SpawnParams.Instigator = Cast<APawn>(GetOwner());
	}

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//生成子弹 Actor
	AGPProjectile* Projectile = World->SpawnActor<AGPProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	//检查生成结果
	if (!Projectile)
	{
		UE_LOG(LogGP, Warning, TEXT("[%s] 子弹生成失败"), *GetName());
	}


}



void AGPWeaponBase::StartFire()
{
	//如果正在开火则返回
	if (bIsFiring) { return; }
	
	//标记为正在开火
	bIsFiring = true;

	//立刻射出第一发降低延迟
	FireOnce();

	//启动循环定时器：每隔 FireRate 秒重复执行 Fire()，直到松开扳机
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AGPWeaponBase::FireOnce, FireRate, true);

}

void AGPWeaponBase::StopFire()
{
	//松开扳机，清除并重置开火定时器
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);

	//标记为停止开火
	bIsFiring = false;
}

