// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Hero/WarriorHeroAnimInstance.h"
#include "Characters/WarriorHeroCharacter.h"   // Cast 需要完整类型定义

void UWarriorHeroAnimInstance::NativeInitializeAnimation()
{
	// 先执行基类初始化：缓存 OwningCharacter / OwningMovementComponent
	Super::NativeInitializeAnimation();

	// 缓存英雄角色引用：用于读取英雄专属数据
	if (OwningCharacter)
	{
		OwningHeroCharacter = Cast<AWarriorHeroCharacter>(OwningCharacter);
	}
}

void UWarriorHeroAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	// 先执行基类每帧更新：计算 GroundSpeed / bHasAcceleration
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	// 放松待机判定：
	// 有加速度（正在移动）→ 清零待机计时、退出放松状态
	// 无加速度（站着不动）→ 累加待机时长，超过阈值则进入放松状态
	if (bHasAcceleration)
	{
		IdleElapsedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else
	{
		IdleElapsedTime += DeltaSeconds;
		bShouldEnterRelaxState = (IdleElapsedTime >= EnterRelaxStateThreshold);
	}
}
