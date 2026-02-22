	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/Actors/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class MYPROJECT_API AWeapon : public AItem
{
	GENERATED_BODY()
protected:
	
	virtual void BeginPlay() override;
	

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields( const FVector &FieldLocation);
public:
	AWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName,AActor* NewOwner, APawn* NewInstigator);
	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName);
	void PlayEquipSound();
	void DisableSphereCollision();
	void DeactivateEmbers();

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

	

private:
	UPROPERTY(EditAnywhere , Category= "Weapon Properties")
	USoundBase* EquipSound;
	
	UPROPERTY(EditAnywhere , Category= "Weapon Properties")
    UBoxComponent* WeaponBox;

	UPROPERTY(visibleAnywhere, Category= "collision")
	USceneComponent* BoxTraceStart;

	UPROPERTY(visibleAnywhere, Category= "collision")
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere,Category="Weapon Properties")
	float Damage = 25.f;

public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
