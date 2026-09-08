// Fill out your copyright notice in the Description page of Project Settings.

#include "DataAssets/Input/DataAsset_InputConfig.h"

// 根据传入的 GameplayTag，在「标签→输入动作」映射表中查找对应的输入动作（给一个tag，在表里面翻一遍，找到了，把实际动作拿出来）
UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag)const
{
    for (const FWarriorInputActionConfig& InputActionConfig : NativeInputActions)
    {
        // 标签匹配 且 输入动作有效 时，返回该输入动作
        if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
        {
            return InputActionConfig.InputAction;
        }
    }

    // 未找到匹配项，返回空指针
    return nullptr;
}
