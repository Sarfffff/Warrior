// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/WarriorBaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"   // UCharacterMovementComponent：读取速度/加速度
#include "WarriorCharacterAnimInstance.generated.h"

class AWarriorBaseCharacter;   // 前向声明：头文件里只需指针声明，具体 include 放到 cpp
class UCharacterMovementComponent;
// 角色动画实例：挂到角色身上的"主动画蓝图"，负责缓存角色/移动组件并每帧更新移动数据
UCLASS()
class WARRIOR_API UWarriorCharacterAnimInstance : public UWarriorBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	// ---- 引用缓存（初始化时赋值一次）----
	// 拥有者角色引用：初始化时从 TryGetPawnOwner 缓存
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|References", meta = (AllowPrivateAccess = "true"))
	AWarriorBaseCharacter* OwningCharacter;

	// 角色移动组件引用：初始化时从角色身上缓存，用于读取 Velocity/Acceleration
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|References", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* OwningMovementComponent;

	// ---- 移动数据（每帧由 NativeThreadSafeUpdateAnimation 更新，供动画蓝图使用）----
	// 地面移动速度：只取水平速度，用于 Blend Space 的 Speed 轴（Idle/Walk/Run 混合）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData", meta = (AllowPrivateAccess = "true"))
	float GroundSpeed;

	// 是否有加速度输入：玩家是否在推摇杆/按键，用于 Idle ↔ Walk 过渡
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData", meta = (AllowPrivateAccess = "true"))
	bool bHasAcceleration;
};
