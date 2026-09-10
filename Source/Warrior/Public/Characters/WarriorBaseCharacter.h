// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"        // ACharacter：角色基类（胶囊体 + 移动组件 + 骨骼网格体）
#include "AbilitySystemInterface.h"         // IAbilitySystemInterface：GAS 接口，要求实现 GetAbilitySystemComponent()
#include "WarriorBaseCharacter.generated.h"

// 前向声明：只需指针，具体 include 放到 .cpp 里避免头文件过度耦合
class UWarriorAbilitySystemComponent;
class UWarriorAttributeSet;

/**
 * 角色基类：所有角色的公共底座
 * - 继承 ACharacter：拥有胶囊体、移动、骨骼网格体等角色能力
 * - 继承 IAbilitySystemInterface：接入 GAS（Gameplay Ability System），让角色拥有技能/属性能力
 *
 * 关键设计：
 * 每个角色身上都会挂一个 UWarriorAbilitySystemComponent（能力系统组件）和一个
 * UWarriorAttributeSet（属性集），这两个是 GAS 的两大核心，子类（英雄/敌人）复用这套结构。
 */
UCLASS()
class WARRIOR_API AWarriorBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// 构造函数：创建组件并设置角色默认属性
	AWarriorBaseCharacter();

	// GAS 接口实现：返回本角色的能力系统组件，供 GAS 框架和外部系统调用
	// （IAbilitySystemInterface 要求实现，缺了它本类会变成抽象类无法实例化）
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// 被控制器占有（Possess）时回调：这是 GAS 初始化能力系统组件 OwnerInfo/AvatarInfo 的时机
	virtual void PossessedBy(AController* NewController) override;

	// 能力系统组件（ASC）：负责发放/激活技能、管理 GameplayEffect、处理标签等
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UWarriorAbilitySystemComponent* WarriorAbilitySystemComponent;

	// 属性集（AttributeSet）：存放生命值、攻击力等属性数据，供 GameplayEffect 修改
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	UWarriorAttributeSet* WarriorAttributeSet;

public:
	// 便捷访问器：返回能力系统组件
	FORCEINLINE UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponent() const { return WarriorAbilitySystemComponent; }

	// 便捷访问器：返回属性集
	FORCEINLINE UWarriorAttributeSet* GetWarriorAttributeSet() const { return WarriorAttributeSet; }
};
