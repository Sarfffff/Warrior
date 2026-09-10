// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/WarriorBaseCharacter.h"          // 基类：AWarriorBaseCharacter（继承自 ACharacter）
#include "DataAssets/Input/DataAsset_InputConfig.h"   // 输入配置数据资产：存「标签→输入动作」映射表
#include "WarriorHeroCharacter.generated.h"            // UHT 生成代码，必须放最后

// 前向声明：组件类型这里只声明，具体 include 放到 .cpp 里即可
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;   // 前向声明：增强输入的"输入值"结构体（移动/视角回调的参数）

// 英雄角色：玩家操控的角色，挂载相机并负责输入绑定
UCLASS()
class WARRIOR_API AWarriorHeroCharacter : public AWarriorBaseCharacter
{
	GENERATED_BODY()

public:
	// 构造函数：创建组件并设置角色默认属性（胶囊体/相机/移动参数等）
	AWarriorHeroCharacter();

protected:
	// 输入绑定入口：游戏框架会在输入组件准备好后调用，用于接入 Enhanced Input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 游戏开始回调：角色进入世界后执行一次
	virtual void BeginPlay() override;


	virtual void PossessedBy(AController* NewController) override;
private:

#pragma region Components
	// 弹簧臂组件：角色与相机之间的"弹性杆"，负责控制相机距离、碰撞和旋转
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// 跟随相机组件：玩家视角的相机
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
#pragma endregion

	// 输入配置数据资产：存放「标签→输入动作」映射表，在蓝图里配置
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	// ---- 输入回调函数（由 Enhanced Input 触发）----
	// 移动输入回调：接收 2D 向量（WASD）
	void Input_Move(const FInputActionValue& InputActionValue);
	// 视角输入回调：接收 2D 向量（鼠标 XY）
	void Input_Look(const FInputActionValue& InputActionValue);
};
