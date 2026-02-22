// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/Characters/CharacterStates.h"
#include "MyProject/Actors/Item.h"
#include "GreatSword.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AGreatSword : public AItem
{
	GENERATED_BODY()
public:
	ECharacterState CharacterState;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	void Equip(USceneComponent* InParent, FName Socket);
};
