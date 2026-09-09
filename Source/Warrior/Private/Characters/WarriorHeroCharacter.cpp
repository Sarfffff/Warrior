// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/WarriorHeroCharacter.h"
#include "Camera/CameraComponent.h"                    // UCameraComponent：相机组件
#include "Components/CapsuleComponent.h"               // UCapsuleComponent：角色胶囊体
#include "GameFramework/CharacterMovementComponent.h"  // UCharacterMovementComponent：角色移动参数
#include "GameFramework/SpringArmComponent.h"          // USpringArmComponent：弹簧臂组件
#include "GameFramework/PlayerController.h"            // APlayerController：用于获取本地玩家
#include "EnhancedInputSubsystems.h"                   // UEnhancedInputLocalPlayerSubsystem：增强输入本地玩家子系统
#include "InputActionValue.h"                          // FInputActionValue：增强输入的输入值结构体
#include "InputTriggers.h"                             // ETriggerEvent：输入触发时机
#include "Components/Input/WarriorInputComponent.h"    // UWarriorInputComponent：自定义输入组件（按标签绑定）
#include "WarriorGamePlayTags.h"                       // WarriorGameplayTags::InputTag_Move / InputTag_Look
#include "WarriorsDebugHelper.h"                       // Debug::Print：屏幕调试输出

// 构造函数：创建组件并设置角色默认属性
AWarriorHeroCharacter::AWarriorHeroCharacter()
{
	// 设置胶囊体尺寸：半径 42cm、半高 96cm（第三人称标准体型）
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	// 禁用控制器旋转对角色朝向的影响（鼠标转视角时，角色身体不跟着转）
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 创建弹簧臂组件（角色与相机之间的"弹性杆"）
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());                    // 挂到根组件（胶囊体）上
	CameraBoom->TargetArmLength = 200.f;                                // 弹簧臂长度 200cm（相机离角色距离）
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);                // 相机挂点偏移（右偏+抬高，形成越肩视角）
	CameraBoom->bUsePawnControlRotation = true;                         // 弹簧臂跟随鼠标转向旋转

	// 创建跟随相机组件
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // 挂到弹簧臂末端
	FollowCamera->bUsePawnControlRotation = false;                      // 相机自身不叠加旋转（统一由弹簧臂控制）

	// 移动相关参数
	GetCharacterMovement()->bOrientRotationToMovement = true;           // 角色朝向自动跟随移动方向
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // 转身速率：绕 Yaw 轴每秒 500 度
	GetCharacterMovement()->MaxWalkSpeed = 400.f;                       // 最大行走速度 400cm/s
	GetCharacterMovement()->BrakingDecelerationWalking = 0.2f;          // 松开方向键后的刹车减速率
}

// 游戏开始回调：角色进入世界后执行一次
void AWarriorHeroCharacter::BeginPlay()
{
	// 先执行父类逻辑
	Super::BeginPlay();

	// 调试输出：屏幕打印 "Working"，验证函数被正确调用
	Debug::Print(TEXT("Working"));
}

// 输入绑定入口：框架在输入组件准备好后调用，用于接入 Enhanced Input
void AWarriorHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// 先执行父类默认绑定逻辑
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 获取本地玩家对象，用于拿到增强输入子系统
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	// 1) 获取增强输入的本地玩家子系统（管理输入映射上下文的注册/移除）
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem); // 子系统必须存在

	// 2) 注册输入映射上下文，让 IA_Move / IA_Look 的按键绑定生效
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	// 3) 把默认输入组件强转成自定义输入组件（默认输入组件类需在蓝图中设为 UWarriorInputComponent）
	UWarriorInputComponent* WarriorInputComponent = CastChecked<UWarriorInputComponent>(PlayerInputComponent);

	// 4) 按标签绑定「移动」输入：持续触发时调用 Input_Move
	WarriorInputComponent->BindNativeInputAction(
		InputConfigDataAsset,
		WarriorGameplayTags::InputTag_Move,
		ETriggerEvent::Triggered,
		this,
		&AWarriorHeroCharacter::Input_Move);

	// 5) 按标签绑定「视角」输入：持续触发时调用 Input_Look
	WarriorInputComponent->BindNativeInputAction(
		InputConfigDataAsset,
		WarriorGameplayTags::InputTag_Look,
		ETriggerEvent::Triggered,
		this,
		&AWarriorHeroCharacter::Input_Look);
}

// 移动输入回调：接收 2D 输入向量（X=左右，Y=前后）
void AWarriorHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	// 从输入值取出 2D 向量
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// 取控制器当前的 Yaw（水平朝向），忽略 Pitch/Roll
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	// 前后方向（Y）：沿角色面朝方向移动
	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	// 左右方向（X）：沿角色右方移动
	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

// 视角输入回调：接收 2D 输入向量（X=水平转向，Y=俯仰）
void AWarriorHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	// 水平转向（鼠标左右）
	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	// 俯仰（鼠标上下）
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
