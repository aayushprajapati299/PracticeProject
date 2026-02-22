// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProject/Interfaces/HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class MYPROJECT_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()

public:

	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	
protected:

	virtual void BeginPlay() override;

	virtual void GetHit_Implementation(const FVector &ImpactPoint) override;


	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UGeometryCollectionComponent* GeometyCollection;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UCapsuleComponent* CapsuleComponent; 
private:
	UPROPERTY(EditAnywhere, Category= Pickups)
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	


	bool Bbroken=false;

};
