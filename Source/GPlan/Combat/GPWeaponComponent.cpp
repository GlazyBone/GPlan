//Copyright GPlan Project. All Rights Reserved.


#include "Combat/GPWeaponComponent.h"
#include "Combat/GPWeaponBase.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GPlan.h"

// Sets default values for this component's properties
UGPWeaponComponent::UGPWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UGPWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	EquipDefaultWeapon();
	
}


// Called every frame
void UGPWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGPWeaponComponent::StartFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StartFire();
    }
}

void UGPWeaponComponent::StopFire()
{
    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
    }
}

void UGPWeaponComponent::EquipDefaultWeapon()
{
    if (!DefaultWeaponClass)
    {
        UE_LOG(LogGP, Warning, TEXT("[%s] 未配置 DefaultWeaponClass"), *GetNameSafe(GetOwner()));
        return;
    }

    //武器要挂到骨骼上，所以宿主必须是有 Mesh 的角色
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (!OwnerCharacter || !OwnerCharacter->GetMesh())
    {
        UE_LOG(LogGP, Warning, TEXT("[%s] 宿主不是有效角色"), *GetNameSafe(GetOwner()));
        return;
    }

    if (!OwnerCharacter->GetMesh()->DoesSocketExist(AttachSocketName))
    {
        UE_LOG(LogGP, Warning, TEXT("[%s] 未找到武器挂点: %s"), *GetNameSafe(GetOwner()), *AttachSocketName.ToString());
        return;
    }

    UWorld* World = GetWorld();
    if (!World) return;

    //归属链：Weapon.Owner = Character，Weapon.Instigator = Character（Pawn）
    //这样子弹从武器取 GetInstigator() 才能一路回到玩家 Pawn
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();
    SpawnParams.Instigator = OwnerCharacter;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    CurrentWeapon = World->SpawnActor<AGPWeaponBase>(DefaultWeaponClass, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation(), SpawnParams);

    if (!CurrentWeapon)
    {
        UE_LOG(LogGP, Warning, TEXT("[%s] 武器生成失败"), *GetNameSafe(GetOwner()));
        return;
    }

    const bool bAttached = CurrentWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketName);

    if (!bAttached)
    {
        UE_LOG(LogGP, Warning, TEXT("[%s] 武器附着失败"), *GetNameSafe(GetOwner()));
        CurrentWeapon->Destroy();
        CurrentWeapon = nullptr;
    }
}

