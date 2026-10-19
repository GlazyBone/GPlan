//Copyright GPlan Project. All Rights Reserved.


#include "Combat/GPProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h" 
#include "GPlan.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/Pawn.h"

AGPProjectile::AGPProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;//关闭自身Tick

	//创建组件
	BulletCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Bullet"));
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	//把子弹碰撞框设为根节点
	RootComponent = BulletCollision;
	//子弹模型挂在子弹碰撞框下面
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

void AGPProjectile::InitProjectile(float InDamage)
{
	Damage = InDamage;
}

void AGPProjectile::BeginPlay()
{
	Super::BeginPlay();
	//注册碰撞回调
	BulletCollision->OnComponentHit.AddDynamic(this, &AGPProjectile::OnHit);
}


void AGPProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//无效目标或当前子弹本身
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	//不处理自己的武器和开火者
	if (OtherActor == GetOwner() || OtherActor == GetInstigator())
	{
		return;
	}

	//不允许子弹之间互相造成伤害
	if (OtherActor->IsA(AGPProjectile::StaticClass()))
	{
		return;
	}

	//正常命中目标
	UE_LOG(LogGP, Display, TEXT("子弹命中了: %s! 造成 %f 点伤害"), *GetNameSafe(OtherActor), Damage);

	//如果打中的Actor挂了GPHealthComponent，就会触发扣血
	//								受击者，	伤害数值	，	开枪者的Controller，	造成伤害的物理Actor，	伤害类型
	UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());

	//销毁子弹
	Destroy();
}
