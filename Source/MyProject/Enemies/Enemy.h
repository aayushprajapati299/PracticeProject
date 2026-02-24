

#pragma once

#include "CoreMinimal.h"
#include "MyProject/Characters/BaseCharacter.h"
#include "MyProject/Characters/CharacterStates.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;
class AAIController;
class UAnimMontage;
class AWeapon;

UCLASS()
class MYPROJECT_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/*  <AActor>*/
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,AController* EventInstigator, AActor* DamageCauser) override;
	/*  <AActor>*/

	/*  <IHitInterface>*/
	virtual void GetHit_Implementation(const FVector &ImpactPoint);
	/*  <IHitInterface>*/

private:

	/* AI Behavior*/
	void InitializeEnemy();
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void StartChasing();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsChasing();
	bool IsInsideAttackRadius();
	bool IsAttacking();
	void StartAttackTimer();
	void ClearAttackTimer();
	void ClearPatrolTimer();
	bool IsEngaged();
	bool InTargetRange(AActor* Target , double TargetRadius);
	void MoveToTarget(AActor* Target);
	AActor* SelectPatrolTarget();
	bool IsDead();
	void SpawnDefaultWeapon();

	
	


	UPROPERTY(EditAnywhere)
    class UHealthBar* HealthBar;
    
    UPROPERTY()
    AActor* CombatTarget;
 
    UPROPERTY(EditAnywhere)
    double CombatRadius=1000.f;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;
	
	

	UPROPERTY(EditAnywhere)
	double PatrolRadius=200.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius=150.f;

	//navigation
	UPROPERTY()
	TObjectPtr<AAIController> AIController;

	

	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	TArray<AActor*> PatrolTargets;

	FTimerHandle PatrolTimer;
	UPROPERTY(EditAnywhere)
	double PatrolWailMin=5.f;

	UPROPERTY(EditAnywhere)
	double PatrolWailMax=10.f;

	UPROPERTY(EditAnywhere,Category="Combat")
	float PatrollingSpeed=125.f;
	
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere,Category="Combat")
	float AttackMin=0.5f;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	float AttackMax=1.f;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	float ChasingSpeed=300.f;

	UPROPERTY(EditAnywhere,Category="Combat")
	float DeathLifeSpan=8.f;
	
protected:
	/*  <AActor>*/
	virtual void BeginPlay() override;
	/*  </AActor>*/

	/*  <ABaseCharacter>*/
	virtual void Die() override;
	virtual void Attack() override;
    virtual bool CanAttack() override;
    virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;
	virtual void AttackEnd() override;
	/*  </ABaseCharacter>*/
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte<EDeathPose> DeathPose=EDeathPose::EDP_Death1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EEnemyStates EnemyStates = EEnemyStates::EES_Patrolling;

	
};
