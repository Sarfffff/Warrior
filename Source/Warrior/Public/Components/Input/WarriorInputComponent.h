// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"    // UEnhancedInputComponent：增强输入组件基类，提供 BindAction 绑定能力
#include "InputAction.h"               // UInputAction：增强输入动作
#include "InputTriggers.h"             // ETriggerEvent：触发时机（按下/松开/持续 等）
#include "GameplayTagContainer.h"      // FGameplayTag：输入标签

#include "DataAssets/Input/DataAsset_InputConfig.h" // UDataAsset_InputConfig：标签→动作映射表，用于按标签查动作

#include "WarriorInputComponent.generated.h"  // UHT 生成代码，必须放最后

/**
 * 自定义增强输入组件
 * 在 UEnhancedInputComponent 基础上封装一层：
 * 绑定输入时可以直接传「标签」来查动作，不用手动拿 UInputAction 对象。
 */
UCLASS()
class WARRIOR_API UWarriorInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	/**
	 * 按标签绑定一个输入动作（模板函数，必须写在头文件里）
	 * @param InInputConfig 输入配置数据资产（存着 标签→动作 的映射表）
	 * @param InInputTag    要绑定的输入标签（如 InputTag.Move）
	 * @param TriggerEvent  触发时机（Started / Triggered / Completed 等）
	 * @param ContextObject 回调所属对象（通常是 this）
	 * @param Func          回调函数（成员函数指针）
	 */
	template<class UserObject, typename CallbackFunc>
	inline void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
	{
		// 配置资产不能为空，否则直接报错并给出提示信息
		checkf(InInputConfig, TEXT("Input config data asset is null"));

		// 用标签在配置表里找到对应的输入动作；找到了才进行绑定
		if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
		{
			// 调用父类的 BindAction，把「动作 + 触发时机 + 回调」绑定起来
			BindAction(FoundAction, TriggerEvent, ContextObject, Func);
		}
	}
};
