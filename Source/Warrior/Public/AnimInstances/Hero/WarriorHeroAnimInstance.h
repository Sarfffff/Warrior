// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/WarriorCharacterAnimInstance.h"   // 基类：角色动画实例（负责通用移动数据）
#include "WarriorHeroAnimInstance.generated.h"

class AWarriorHeroCharacter;   // 前向声明：头文件里只需指针，具体 include 放 cpp

// 英雄角色的动画实例：在通用角色动画实例基础上，扩展英雄专属的"放松待机"逻辑
UCLASS()
class WARRIOR_API UWarriorHeroAnimInstance : public UWarriorCharacterAnimInstance
{
	GENERATED_BODY()

public:
	// 动画实例初始化：缓存角色引用（基类缓存 + 英雄专属缓存）
	virtual void NativeInitializeAnimation() override;

	// 每帧线程安全更新：基类更新移动数据 + 英雄专属的放松状态判断
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	// 英雄角色引用：初始化时缓存，用于读取英雄专属数据
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|References", meta = (AllowPrivateAccess = "true"))
	AWarriorHeroCharacter* OwningHeroCharacter;

	// 是否进入"放松待机"状态：长时间不动后切到放松待机动画（供动画蓝图读取）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|LocomotionData", meta = (AllowPrivateAccess = "true"))
	bool bShouldEnterRelaxState = false;

	// 进入放松待机的阈值（秒）：待机累计超过该时长则进入放松状态
	// 如需在编辑器里调整该值，把 VisibleAnywhere 改成 EditAnywhere 即可
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData", meta = (AllowPrivateAccess = "true"))
	float EnterRelaxStateThreshold = 5.f;

	// 待机累计时长（秒）：每帧累加，超过阈值后 bShouldEnterRelaxState 置 true
	UPROPERTY(BlueprintReadOnly, Category = "AnimData|LocomotionData", meta = (AllowPrivateAccess = "true"))
	float IdleElapsedTime = 0.f;
};
