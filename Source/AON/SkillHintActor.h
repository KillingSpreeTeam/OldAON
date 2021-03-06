﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Actor.h"
#include "SkillHintActor.generated.h"


UENUM(BlueprintType)
enum class ESkillHintEnum : uint8
{
	NoneHint,
	DirectionSkill_CanBlock,
	DirectionSkill,
	RangeSkill,
	EarmarkHeroSkill,
	EarmarkNonHeroSkill,
	EarmarkAnyoneSkill,
};

UCLASS()
class AON_API ASkillHintActor : public AActor
{
	GENERATED_UCLASS_BODY()
public:	

	// Component
	UPROPERTY(Category = "SkillHint", EditAnywhere, BlueprintReadOnly)
	FVector2D MouseIconOffset;

	UPROPERTY(Category = "FlySkill", VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Scene;

	UPROPERTY(Category = "FlySkill", VisibleAnywhere, BlueprintReadOnly)
	UPaperSpriteComponent* BodySprite;

	UPROPERTY(Category = "FlySkill", VisibleAnywhere, BlueprintReadOnly)
	UPaperSpriteComponent* HeadSprite;

	UPROPERTY(Category = "FlySkill", VisibleAnywhere, BlueprintReadOnly)
	UPaperSpriteComponent* FootSprite;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostInitProperties() override;
#endif
	UFUNCTION(Category = "FlySkill", BlueprintCallable)
	void SetLength(float len);

	void UpdateLength();

	void UpdatePos(FVector PlayerPos, FVector MousePos);

	UPROPERTY(Category = "SkillHint", EditAnywhere, BlueprintReadOnly)
	ESkillHintEnum SkillType;

	UPROPERTY(Category = "SkillHint", EditAnywhere, BlueprintReadOnly)
	FVector SkillPos;

	UPROPERTY(Category = "SkillHint", VisibleAnywhere, BlueprintReadOnly)
	uint32 UseDirectionSkill: 1;

	UPROPERTY(Category = "SkillHint", VisibleAnywhere, BlueprintReadOnly)
	uint32 UseRangeSkill: 1;
	
	UPROPERTY(Category = "SkillHint", EditAnywhere, BlueprintReadOnly)
	UTexture2D* MouseIcon;
	
	// 是否固定長度
	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "UseDirectionSkill"))
	uint32 IsFixdLength: 1;
	
	//範圍技直徑
	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "UseRangeSkill"))
	float SkillDiameter;

	//最大施法距離
	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "UseRangeSkill"))
	float MaxCastRange;

	//最小施法距離
	UPROPERTY(Category = "FlySkill", EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "UseRangeSkill"))
	float MinCastRange;
};
