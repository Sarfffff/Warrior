// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WarriorBaseCharacter.h"

// Sets default values
AWarriorBaseCharacter::AWarriorBaseCharacter()
{

	PrimaryActorTick.bCanEverTick = false;  //决定这个角色是否有资格被逐帧调用 Tick()
	PrimaryActorTick.bStartWithTickEnabled = false;

	//GetMesh() 返回 ACharacter 内置的骨骼网格体组件
	GetMesh()->bReceivesDecals = false;  //bReceivesDecals 控制该网格体是否接收贴花（Decal），比如弹孔、血迹、脚印等投射出来的贴花。
}

