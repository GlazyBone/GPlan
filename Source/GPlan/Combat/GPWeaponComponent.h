//Copyright GPlan Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GPWeaponComponent.generated.h"

class AGPWeaponBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GPLAN_API UGPWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGPWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:

	/** 默认武器蓝图类 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<AGPWeaponBase> DefaultWeaponClass;

    /** 角色骨骼上的武器挂点 */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName AttachSocketName = TEXT("weapon_r_socket");

    /** 当前装备的武器（运行时状态，只读可见） */
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<AGPWeaponBase> CurrentWeapon;

    //—— 对外接口（Character 转发到这里）——
    void StartFire();
    void StopFire();

    /** 给外部（UI/调试）读当前武器 */
    FORCEINLINE AGPWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

protected:

    /** 生成并装备默认武器 */
    void EquipDefaultWeapon();
		
};
