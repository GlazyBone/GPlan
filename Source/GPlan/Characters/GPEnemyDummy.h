//Copyright GPlan Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GPEnemyDummy.generated.h"


class UGPHealthComponent;
class UAnimMontage;


UCLASS()
class GPLAN_API AGPEnemyDummy : public ACharacter
{
	GENERATED_BODY()

public:
	AGPEnemyDummy();

protected:
	virtual void BeginPlay() override;

protected:
	/** 血量组件 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UGPHealthComponent> HealthComponent;

    /** 死亡动画 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Death")
    TObjectPtr<UAnimMontage> DeathMontage;

private:

    /** 防止重复处理死亡事件 */
    bool bEnemyDead = false;

    /** 处理死亡事件 */
    UFUNCTION()
    void EnemyDead(AActor* DamageCauser);

};
