// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WarriorBaseCharacter.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"   // UWarriorAbilitySystemComponent：自定义 ASC
#include "AbilitySystem/WarriorAttributeSet.h"             // UWarriorAttributeSet：自定义属性集

// 构造函数：创建组件并设置角色默认属性
AWarriorBaseCharacter::AWarriorBaseCharacter()
{
	// 决定这个角色是否有资格被逐帧调用 Tick()；纯数据驱动时关闭可省性能
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// GetMesh() 返回 ACharacter 内置的骨骼网格体组件
	// bReceivesDecals=false：该网格体不接收贴花（弹孔/血迹/脚印等），避免角色身上出现不想要的投影
	GetMesh()->bReceivesDecals = false;

	// 创建 GAS 两大核心组件：能力系统组件 + 属性集
	WarriorAbilitySystemComponent = CreateDefaultSubobject<UWarriorAbilitySystemComponent>(TEXT("WarriorAbilitySystemComponent"));
	WarriorAttributeSet = CreateDefaultSubobject<UWarriorAttributeSet>(TEXT("WarriorAttributeSet"));
}

// 被控制器占有（Possess）时回调
void AWarriorBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 关键：GAS 的 ASC 需要知道「谁拥有能力（Owner）」和「谁表现为这个角色（Avatar）」
	// 这里 Owner 和 Avatar 都传 this（玩家角色自己拥有并表现能力）
	// 必须在 Possess 之后调用，否则 ASC 拿不到 PlayerState/Controller 等上下文
	if (WarriorAbilitySystemComponent)
	{
		WarriorAbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

// GAS 接口实现：返回能力系统组件（注意这里不能写 override，override 只出现在类内声明处）
UAbilitySystemComponent* AWarriorBaseCharacter::GetAbilitySystemComponent() const
{
	return GetWarriorAbilitySystemComponent();
}
