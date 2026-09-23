//Copyright GPlan Project. All Rights Reserved.


#include "GPProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h" 
#include "GPlan.h"

// Sets default values
AGPProjectile::AGPProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;//关闭自身Tick

	//创建组件
	BulletCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Bullet"));
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	//把子弹碰撞设为根节点
	RootComponent = BulletCollision;
	//子弹模型挂在子弹碰撞下面
	BulletMesh->SetupAttachment(BulletCollision);

	//配置碰撞球体大小与碰撞通道
	BulletCollision->InitSphereRadius(5.0f);
	BulletCollision->SetCollisionProfileName(TEXT("BlockAllDynamic")); //阻挡一切动态物体

	//关闭模型自身碰撞
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//配置子弹飞行速度与重力
	ProjectileMovement->UpdatedComponent = BulletCollision; //设定移动组件对象
	ProjectileMovement->InitialSpeed = 3000.0f;           //初始发射速度
	ProjectileMovement->MaxSpeed = 3000.0f;               //最大飞行速度
	ProjectileMovement->bRotationFollowsVelocity = true;  //子弹弹头朝向跟随飞行轨迹
	ProjectileMovement->ProjectileGravityScale = 0.0f;    //不受重力影响

	//3秒后自动销毁
	InitialLifeSpan = 3.0f;

}

// Called when the game starts or when spawned
void AGPProjectile::BeginPlay()
{
	Super::BeginPlay();
	//注册碰撞回调
	BulletCollision->OnComponentHit.AddDynamic(this, &AGPProjectile::OnHit);
}


void AGPProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//撞到的目标不能为空，且不能是子弹自己，也不能是开枪的玩家自己
	if (OtherActor && (OtherActor != this) && (OtherActor != GetInstigator()))
	{
		UE_LOG(LogGP, Display, TEXT("子弹命中了: %s! 造成 %f 点伤害"), *GetNameSafe(OtherActor), Damage);

		//施加伤害
		//如果打中的Actor挂了GPHealthComponent，就会触发扣血
		UGameplayStatics::ApplyDamage(
			OtherActor,                  //受击者
			Damage,                      //伤害数值
			GetInstigatorController(),   //开枪者的Controller
			this,                        //造成伤害的物理Actor
			UDamageType::StaticClass()   //伤害类型
		);

		//销毁子弹
		Destroy();
	}
}