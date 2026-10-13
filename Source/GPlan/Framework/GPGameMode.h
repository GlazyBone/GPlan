//Copyright GPlan Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GPGameMode.generated.h"

//声明动态委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameWon);


UCLASS(minimalapi)
class AGPGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AGPGameMode();


public:

	/** 接收敌人死亡通知 */
	void NotifyEnemyKilled(AActor* EnemyActor);

	/** 当前击杀数量 */
	int32 GetEnemiesKilled() const { return EnemiesKilled; }

	/** 游戏胜利动态委托 */
	UPROPERTY(BlueprintAssignable, Category = "Flow")
    FOnGameWon OnGameWon;

	/** 过关需要的击杀数量 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flow")
    int32 KillsToWin = 5;   

private:
	//当前已击杀敌人的数量
	int32 EnemiesKilled = 0;
	//是否过关
	bool bGameWon = false;


};



