// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyProject/Characters/CharacterStates.h"
#include "CharacterAnim.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
public: 
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	class ASlashCharacter* SlashCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(BlueprintReadOnly , Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly	, Category = "Movement")
	bool IsFalling;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement | CharacterStates")
	ECharacterState CharacterState;
};
