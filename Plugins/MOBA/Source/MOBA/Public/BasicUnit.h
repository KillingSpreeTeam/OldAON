﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"
#include "AIController.h"
#include "HeroAction.h"
#include "BasicUnit.generated.h"

class ABulletActor;
class ADamageEffect;
class AEquipment;
class AHeroSkill;
class AHeroBuff;
class ASkillHintActor;
class AMOBAPlayerController;
class UWebInterfaceJsonValue;

UCLASS()
class MOBA_API ABasicUnit : public ACharacter
{
	GENERATED_UCLASS_BODY()

public:
	// Sets default values for this character's properties
	ABasicUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// for Game Logic
	UFUNCTION(BlueprintCallable, Category = "MOBA")
	float GetSkillCDPercent(int32 n);

	//得到HP百分比
	UFUNCTION(BlueprintCallable, Category = "MOBA")
	float GetHPPercent();

	//得到MP百分比
	UFUNCTION(BlueprintCallable, Category = "MOBA")
	float GetMPPercent();

	//依等級更新血魔攻速
	UFUNCTION(BlueprintCallable, Category = "MOBA")
	void UpdateHPMPAS();

	//英雄選擇中
	UFUNCTION(BlueprintCallable, Category = "MOBA")
	void SelectionOn();

	//取消英雄選擇中
	UFUNCTION(BlueprintCallable, Category = "MOBA")
	void SelectionOff();

	UFUNCTION(BlueprintCallable, Category = "MOBA")
	bool TriggerSkill(int32 index, FVector Pos, ABasicUnit* CurrentTarget);

	UFUNCTION(BlueprintCallable, Category = "MOBA")
	bool ShowSkillHint(int32 index);

	UFUNCTION(BlueprintCallable, Category = "MOBA")
	void HideSkillHint();

	UFUNCTION(BlueprintCallable, Category = "MOBA")
	bool UseSkill(EHeroActionStatus SpellType, int32 index, FVector VFaceTo, FVector Pos, ABasicUnit* victim);

	UFUNCTION(BlueprintCallable, Category = "MOBA")
	int32 GetCurrentSkillIndex();

	UFUNCTION(BlueprintCallable, Category = "MOBA")
	AHeroSkill* GetCurrentSkill();

	UFUNCTION(NetMulticast, WithValidation, Unreliable, BlueprintCallable, Category = "MOBA")
	void ServerPlayAttackStartSFX();

	UFUNCTION(NetMulticast, WithValidation, Unreliable, BlueprintCallable, Category = "MOBA")
	void ServerPlayAttackLandedSFX();

	// 確定當前動作做完了沒
	bool CheckCurrentActionFinish();

	// 做動作
	UFUNCTION(Server, WithValidation, Reliable, Category = "MOBA")
	void DoAction(const FHeroAction& _CurrentAction);

	// 停止目前所有動作
	void DoNothing();

	// 做移動攻擊到指定位置
	void DoAction_MovingAttackToPosition(const FHeroAction& _CurrentAction);
	// 做移動到指定位置
	void DoAction_MoveToPosition(const FHeroAction& _CurrentAction);
	void DoAction_MoveToPositionImpl(const FHeroAction& _CurrentAction);
	
	// 推出做完的動作
	void PopAction();
	// 使用打人
	void DoAction_AttackActor(const FHeroAction& _CurrentAction);
	// 使用指定技
	void DoAction_SpellToActor(const FHeroAction& _CurrentAction);
	// 使用指向技
	void DoAction_SpellToDirection(const FHeroAction& _CurrentAction);
	//持續施法中
	void DoAction_SpellChannelling(const FHeroAction& _CurrentAction);
	//撿拾物品
	void DoAction_MoveToPickup(const FHeroAction& _CurrentAction);
	//丟棄物品
	void DoAction_MoveToThrowEqu(const FHeroAction& _CurrentAction);

	//播放攻擊動畫
	UFUNCTION(NetMulticast, Unreliable, WithValidation)
	void ServerPlayAttack(float duraction, float rate);
	
	//播放持續施法動畫
	UFUNCTION(BlueprintImplementableEvent)
	void BP_PlayChannelling(float duraction, float rate);
	
	//加上buff
	UFUNCTION(BlueprintCallable, Category = "MOBA")
	void AddBuff(AHeroBuff* buff);
	
	//使用buff名稱得到buff指標
	UFUNCTION(BlueprintCallable, Category = "MOBA")
	AHeroBuff* GetBuffByName(FString name);

	// 加入Buff時取代同名的Buff
	UFUNCTION(BlueprintCallable, Category = "MOBA")
	void AddUniqueBuff(AHeroBuff* buff);
	
	// 移除所有同名的Buff
	UFUNCTION(BlueprintCallable, Category = "MOBA")
	void RemoveBuffByName(FString name);

	// 移除Buff
	UFUNCTION(BlueprintCallable, Category = "MOBA")
	void RemoveBuff(AHeroBuff* buff);

	UFUNCTION(NetMulticast, WithValidation, Unreliable, BlueprintCallable)
	void ServerShowDamageEffect(FVector pos, FVector dir, float Damage);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_PlayAttack(float duraction, float rate);

	bool Pickup(AEquipment* equ);

	bool ThrowEquipment(AEquipment* equ, FVector pos);

	bool HasEquipment(AEquipment* equ);

	// Particle特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	TMap<FString, UParticleSystemComponent*> Particles;

	// Debug information
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool IsDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	EMOBAType UnitType = EMOBAType::BasicUnit;

	// 選人的地版光環
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	UDecalComponent * SelectionDecal;

	// 手動設定的頭頂位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	UArrowComponent * PositionOnHead;

	// 手動設定的腳底位置
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	UArrowComponent * PositionUnderFoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	UAudioComponent * AttackStartSFX;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	UAudioComponent * AttackLandedSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TSubclassOf<ABulletActor> HeroBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TSubclassOf<ADamageEffect> ShowDamageEffect;

	// 英雄名/單位名
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	FString UnitName;

	// 血條長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float HPBarLength;
	
	// set by HUD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	FVector2D	ScreenPosition;

	// 移動攻擊時，遇到敵人開始追敵人時的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	FVector	StartFollowPosition;

	// 移動攻擊時，遇到敵人開始移動的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float	MovingAttackRange = 1000;

	// 移動攻擊時，遇到想攻擊的敵人
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	ABasicUnit* MovingAttackTarget = nullptr;
	
	// 技能實體
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	TArray<AHeroSkill*> Skills;
	
	// 技能
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	TArray<TSubclassOf<AHeroSkill>>	Skill_Classes;

	// 基礎攻擊距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseAttackRange;

	// 原始攻擊秒數
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseAttackSpeedSecond;
	
	// 基礎攻擊動畫時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseAttackingAnimationTimeLength;
	// 基礎攻擊前搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseAttackingBeginingTimeLength;
	// 基礎攻擊後搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseAttackingEndingTimeLength;

	// 基礎施法前等待時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseSpellingWatingTimeLength;
	// 基礎施法動畫時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseSpellingAnimationTimeLength;
	// 基礎施法前搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseSpellingBeginingTimeLength;
	// 基礎施法後搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseSpellingEndingTimeLength;
	
	// 追踨目標更新時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float FollowActorUpdateTimeGap;

	// 基礎魔法受傷倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseMagicInjuredRatio;
	// 基礎物理受傷倍率
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BasePhysicsInjuredRatio;
	// 基礎裝甲
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseArmor;
	// 基礎攻擊力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseAttack;
	// 基礎移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseMoveSpeed;

	// 基礎回血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseRegenHP;
	// 基礎回魔
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseRegenMP;
	// 基礎血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseHP;
	// 基礎魔力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseMP;

	// 基礎掉落金錢
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float BaseBountyGold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	int32 Frame = 0;

	// 當前技能提示
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Current")
	ASkillHintActor* CurrentSkillHint;

	// 當前技能指向
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Current")
	FVector CurrentSkillDirection;

	// 準備要用的技能索引
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	int32 CurrentSkillIndex;
	
	

	// 目前經驗值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	int32 CurrentEXP;

	// 死亡給敵經驗值
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	int32 BountyEXP;

	// 死亡給敵金錢
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	int32 BountyGold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	bool isSelection;

	// 撿東西的距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float PickupObjectDistance;

	// 最小移動距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	float MinimumDontMoveDistance;


	// 隊伍id
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	int32 TeamId;
	// 目前是否攻擊
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	bool PlayAttack;
	// 當前普攻是否打出來了
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	bool IsAttacked;

	// 目前是否活著
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	bool IsAlive;

	/*

	                             Each Attacking Time gap
	|---------------------------------------------------------------------------------------|
	                                                                 waiting for next attack
	                                                                |-----------------------|
	CurrentAttackingAnimationTimeLength
	|---------------------------------------------------------------|
	CurrentAttackingBeginingTimeLength
	|--------------------------------|
	  								 CurrentAttackingEndingTimeLength
									 |------------------------------|
									 ^
							    Cause Damage
	*/
	// 目前攻擊動畫時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentAttackingAnimationTimeLength;
	// 目前攻擊動畫播放速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	float CurrentAttackingAnimationRate;
	// 目前攻擊前搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	float CurrentAttackingBeginingTimeLength;
	// 目前攻擊後搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	float CurrentAttackingEndingTimeLength;

	/*

    waiting for next attack
    |---------------|
	                CurrentSpellingAnimationTimeLength
	                |---------------------------------------------------------------|
	                CurrentSpellingBeginingTimeLength
	                |--------------------------------|
	  						                         CurrentSpellingEndingTimeLength
							                         |------------------------------|
							                         ^
                                               Spell cold down
	*/

	// 目前施法前等待時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentSpellingWatingTimeLength;
	// 目前施法動畫時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentSpellingAnimationTimeLength;
	// 施法速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentSpellingRate;
	// 目前施法前搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentSpellingBeginingTimeLength;
	// 目前施法後搖時間長度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentSpellingEndingTimeLength;
	//當前剩餘持續施法時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float ChannellingTime = 0;

	// 目前攻擊計時器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	float AttackingCounting;
	// 目前施法計時器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Counting", Replicated)
	float SpellingCounting;
	// 追踨目標計時器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Counting")
	float FollowActorUpdateCounting;
	// 暈炫倒數計時器
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Counting")
	float StunningLeftCounting;

	// 移動速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentMoveSpeed;
	// 血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentMaxHP;
	// 魔力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentMaxMP;
	// 血量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	float CurrentHP;
	// 魔力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	float CurrentMP;
	// 回血
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentRegenHP;
	// 回魔
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentRegenMP;
	// 攻速
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentAttackSpeed;
	// 攻速秒數
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	float CurrentAttackSpeedSecond;
	// 攻擊力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentAttack;
	// 防禦力
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentArmor;
	// 當前魔法減傷
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentMagicInjured;
	// 目前攻擊距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	float CurrentAttackRange;
	// 裝備
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	TArray<AEquipment*> Equipments;
	// 暫存mesh材質的地方
	UMaterialInterface* BaseMaterial;

	// 當前模型混色
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	FLinearColor BlendingColor = FLinearColor::White;

	FLinearColor LastBlendingColor = FLinearColor::White;

	// 動畫狀態
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	int32 AnimaStatus;

	int32 LastAnimaStatus;

	// 依序做完裡面的動作
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	TArray<FHeroAction> ActionQueue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	FHeroAction CurrentAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	EHeroBodyStatus BodyStatus;

	// 所有的buff
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	TArray<AHeroBuff*> BuffQueue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	TArray<float> BuffDuration;

	// 當前法球
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MOBA")
	AHeroBuff* CurrentOrb;

	// 當前狀態
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	TMap<EHeroBuffState, bool> BuffStateMap;

	// 預設狀態
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	TMap<EHeroBuffState, bool> DefaultBuffState;

	// 當前加成 可能可以用TArray替代
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	TMap<EHeroBuffProperty, float> BuffPropertyMap;
	
	// 預設加成
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current")
	TMap<EHeroBuffProperty, float> DefaultBuffProperty;
	
	// 最後一次移動的位置
	FVector LastMoveTarget;

	// 最後一次要使用的技能
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	FHeroAction LastUseSkillAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Current", Replicated)
	AHeroSkill* LastUseSkill;

	static AMOBAPlayerController* localPC;
};