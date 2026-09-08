// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"          // 数据资产基类：允许本类作为资产在编辑器中创建/配置
#include "GameplayTagContainer.h"     // FGameplayTag：用于给输入动作打标签
#include "InputAction.h"               // UInputAction：增强输入里的一个具体输入动作（移动/视角等）
#include "InputMappingContext.h"       // UInputMappingContext：按键与输入动作的映射集合

#include "DataAsset_InputConfig.generated.h"  // UHT 生成的反射代码，必须放在所有 include 最后

class UInputAction;
class UInputMappingContext;
// 单条配置：把一个 GameplayTag 与一个 UInputAction 绑定起来
// BlueprintType：允许此结构体被蓝图识别（否则无法暴露字段/数组给蓝图）
USTRUCT(BlueprintType)
struct FWarriorInputActionConfig  //配置表的一行
{
	GENERATED_BODY()

public:
	// 输入标签（如 InputTag.Move / InputTag.Look）
	// meta Categories 限制编辑器下拉框只显示 "InputTag" 分类下的标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;   //这一行的名字

	// 该标签对应的增强输入动作（WASD 移动 / 鼠标视角 等）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;  //这一行对应的实际动作
};

// 输入配置数据资产：在编辑器里配置「标签 → 输入动作」的映射表
UCLASS()
class WARRIOR_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// 输入映射上下文：定义按键与输入动作的绑定关系
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext; // 按键映射（按 W 等于触发"移动"）

	// 原生输入动作映射表：每个元素绑定「一个标签 + 一个输入动作」
	// TitleProperty 让数组元素在编辑器里以 InputAction 名称显示
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputAction"))
	TArray<FWarriorInputActionConfig> NativeInputActions; // 上面那张表的数组

	// 根据标签查找对应的输入动作（找不到返回 nullptr）
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag)const;
};
