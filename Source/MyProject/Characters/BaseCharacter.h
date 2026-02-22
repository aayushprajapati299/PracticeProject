

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyProject/Actors/Weapons/Weapon.h"
#include "MyProject/Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
UCLASS()
class MYPROJECT_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	//Combat
	virtual void Attack();
	virtual bool CanAttack();
	bool IsAlive();
	virtual void Die();
	void HitReactMontagePlay(const FVector& ImpactPoint);
    void PlayHitSound(const FVector& ImpactPoint);
    void SpawnHitParticle(const FVector& ImpactPoint);
    virtual void HandleDamage(float DamageAmount);
	void DisableCapsule();

	UFUNCTION(BlueprintCallable)
	void SetBoxCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
    virtual void AttackEnd();

	//Play Montage
	
	void PlayHitReactMontage(const FName &SectionName);	
	int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();

	AWeapon* EquipedWeapon;
	
	UPROPERTY(EditAnywhere)
	class UAttributeComponent* Attributes;

private:
	void PlayMontageSection (UAnimMontage* Montage ,const FName &SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage,const TArray<FName>& SectionNames);
	
	//sounds
	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;

	// Animation Montages
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FName> DeathMontageSections;

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* DeathMontage;

	//particles
	UPROPERTY(EditAnywhere)
	UParticleSystem* HitParticles;
};
