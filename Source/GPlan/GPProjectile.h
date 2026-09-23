//Copyright GPlan Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GPProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
UCLASS()
class GPLAN_API AGPProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGPProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	TObjectPtr<USphereComponent> BulletCollision;//子弹的碰撞框

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	TObjectPtr<UStaticMeshComponent> BulletMesh;//子弹的模型组件

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;//子弹移动组件

	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	float Damage = 20.0f;//子弹伤害
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//碰撞回调函数
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
