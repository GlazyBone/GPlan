//Copyright GPlan Project. All Rights Reserved.


#include "Characters/GPEnemyDummy.h"
#include "GPlan.h"
#include "Combat/GPHealthComponent.h"
#include "Framework/GPGameMode.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Engine/World.h"

AGPEnemyDummy::AGPEnemyDummy()
{
	//关闭每帧调用
	PrimaryActorTick.bCanEverTick = false;

	//由胶囊体负责受击碰撞，mesh设置为无碰撞
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//添加血量组件
	HealthComponent = CreateDefaultSubobject<UGPHealthComponent>(TEXT("HealthComponent"));

}

void AGPEnemyDummy::BeginPlay()
{
	Super::BeginPlay();

	//如果没有血量组件则直接返回
	if (!HealthComponent)
	{
		UE_LOG(LogGP, Error, TEXT("[%s] HealthComponent 不存在"), *GetName());
		return;
	}

	//订阅血量组件的死亡事件
	HealthComponent->OnDeath.AddDynamic(this, &AGPEnemyDummy::EnemyDead);

}

void AGPEnemyDummy::EnemyDead(AActor* DamageCauser)
{
	//停止角色移动
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	//死亡后关闭碰撞
	SetActorEnableCollision(false);

	//如果配置了死亡动画，则播放
	if (DeathMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(DeathMontage);
		}
	}

	//向GameMode上报死亡
	if (HasAuthority())
	{
		if (UWorld* World = GetWorld())
		{
			if (AGPGameMode* GPGameMode = Cast<AGPGameMode>(World->GetAuthGameMode()))
			{
				GPGameMode->NotifyEnemyKilled(this);
			}
			else
			{
				UE_LOG(LogGP, Warning, TEXT("[%s] 未找到有效的 AGPGameMode"), *GetName());
			}
		}
	}

	//2秒后销毁
	SetLifeSpan(2.0f);
}



