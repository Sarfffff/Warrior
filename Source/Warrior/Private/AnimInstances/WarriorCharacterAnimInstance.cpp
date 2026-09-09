// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/WarriorCharacterAnimInstance.h"
#include "Characters/WarriorBaseCharacter.h"   // Cast 需要完整类型定义

void UWarriorCharacterAnimInstance::NativeInitializeAnimation()
{
	// 缓存拥有者角色：动画蓝图后续通过它读取移动速度、是否落地等状态
	OwningCharacter = Cast<AWarriorBaseCharacter>(TryGetPawnOwner());

	// 缓存移动组件：后续每帧读取 Velocity / Acceleration
	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UWarriorCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	// 每帧安全校验：角色或移动组件无效则直接返回
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	// 地面速度：只取水平速度（忽略 Z 轴），用于 Blend Space 的 Speed 轴
	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	// 是否有加速度输入：当前加速度向量长度 > 0 表示玩家在推摇杆/按键
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;
}
