//Copyright GPlan Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GPHealthComponent.generated.h"

//声明动态多播委托：血量变化时传输当前血量、最大血量、血量变化值
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHPChanged, float, CurrentHP, float, MaxHP, float, HPDelta);

//声明动态多播委托：死亡时传输击杀Actor
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DamageCauser);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GPLAN_API UGPHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGPHealthComponent();

	//属性配置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HP")
	float MaxHP = 100.0f;//最大血量

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
	float CurrentHP = 100.0f;//当前血量

	//事件委托
	UPROPERTY(BlueprintAssignable, Category = "HP")
	FOnHPChanged OnHPChanged;//血量变化事件

	UPROPERTY(BlueprintAssignable, Category = "HP")
	FOnDeath OnDeath;//死亡事件

	//接口函数
	UFUNCTION(BlueprintPure, Category = "HP")
	bool IsDead() const { return bIsDead; }

	UFUNCTION(BlueprintPure, Category = "HP")
	float GetHPPercent() const { return(MaxHP > 0.0f) ? (CurrentHP / MaxHP) : 0.0f; }

protected:
	virtual void BeginPlay() override;

	//受伤回调函数
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);


private:
	bool bIsDead = false;//判断对象是否死亡

};
