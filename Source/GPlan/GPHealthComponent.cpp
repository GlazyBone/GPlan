//Copyright GPlan Project. All Rights Reserved.


#include "GPHealthComponent.h"
#include "GPlan.h"

UGPHealthComponent::UGPHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UGPHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = MaxHP;
	if(AActor* Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UGPHealthComponent::HandleTakeAnyDamage);
	}
	
}

void UGPHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage<=0.0f || bIsDead)
	{
		return;
	}
	//如果伤害超过当前血量按照当前血量扣除，避免血量为负数
	const float ActualDamage = FMath::Min(CurrentHP, Damage);
	CurrentHP -= ActualDamage;

	UE_LOG(LogGP, Display, TEXT("[%s] Took %f damage! Current Health: %f/%f"), *GetNameSafe(GetOwner()), ActualDamage, CurrentHP, MaxHP);

	//发送通知
	OnHPChanged.Broadcast(CurrentHP, MaxHP, -ActualDamage);

	//如果血量少于等于0并且没有死亡，触发死亡事件
	if(CurrentHP <= 0.0f && !bIsDead)
	{
		bIsDead = true;
		UE_LOG(LogGP, Display, TEXT("[%s] is DEAD! Caused by: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(DamageCauser));
		OnDeath.Broadcast(DamageCauser);
	}

}



