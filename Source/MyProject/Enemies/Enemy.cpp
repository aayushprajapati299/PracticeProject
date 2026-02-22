


#include "Enemy.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "MyProject/Components/AttributeComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "MyProject/HUD/HealthBar.h"
#include "MyProject/Actors/Weapons/Weapon.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	GetMesh()->SetGenerateOverlapEvents(true);

	
	


	
	HealthBar=CreateDefaultSubobject<UHealthBar>(FName("HealthBar"));
	HealthBar->SetupAttachment(RootComponent);
	GetCharacterMovement()->bOrientRotationToMovement=true;
	// GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	bUseControllerRotationPitch=false;
	bUseControllerRotationYaw=false;
	bUseControllerRotationRoll=false;

	// PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	// PawnSensing->SetPeripheralVisionAngle(45.f);
	// PawnSensing->SightRadius=3000.f;
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead()) return;
	if (EnemyStates>EEnemyStates::EES_Patrolling) CheckCombatTarget();
	CheckPatrolTarget();
	
		
}

void AEnemy::Destroyed()
{
	Super::Destroyed();
	if (EquipedWeapon)
	{
		EquipedWeapon->Destroy();		
	}
	
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	StartChasing();
	return DamageAmount;
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	ShowHealthBar();
	if (IsAlive())
	{
		HitReactMontagePlay(ImpactPoint);
	}
	else
	{
		Die();
	}
	PlayHitSound(ImpactPoint);
	SpawnHitParticle(ImpactPoint);
}
 
void AEnemy::InitializeEnemy()
{
	AIController = Cast<AAIController>(GetController());
	HideHealthBar();
	MoveToTarget(PatrolTarget);
	SpawnDefaultWeapon();
}

void AEnemy::CheckCombatTarget()
{
	if (CombatTarget)
	{
		
		if (IsOutsideCombatRadius())
		{
			ClearAttackTimer();
			LoseInterest();
			if (!IsEngaged()) StartPatrolling();
		}
		else if (IsOutsideAttackRadius() && !IsChasing())
		{
			ClearAttackTimer();
			if (!IsEngaged()) StartChasing();
		}
		else if (CanAttack())
		{
			ClearAttackTimer();
			StartAttackTimer();
		}
		
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (!PatrolTarget) return;
	if (InTargetRange(PatrolTarget,PatrolRadius))
	{
		PatrolTarget=SelectPatrolTarget();
		double PatrolWaitTime= FMath::RandRange(PatrolWailMin,PatrolWailMax);
		GetWorldTimerManager().SetTimer(PatrolTimer,this,&AEnemy::PatrolTimerFinished, PatrolWaitTime);
	}
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::HideHealthBar()
{
	if (HealthBar)
	{
		HealthBar->SetVisibility(false);
	}
}

void AEnemy::ShowHealthBar()
{
	if (HealthBar)
	{
		HealthBar->SetVisibility(true);
	}
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	EnemyStates=EEnemyStates::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed=PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemy::StartChasing()
{
	EnemyStates=EEnemyStates::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed=ChasingSpeed;
	if (AIController && CombatTarget)
	{
		AIController->SetFocus(CombatTarget);
	}
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyStates == EEnemyStates::EES_Chasing;
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsAttacking()
{
	return EnemyStates == EEnemyStates::EES_Attacking;
}

void AEnemy::StartAttackTimer()
{
	EnemyStates=EEnemyStates::EES_Attacking;
	const float AttackTime=FMath::RandRange(AttackMin,AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer,this, &AEnemy::Attack,AttackTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

bool AEnemy::IsEngaged()
{
	return EnemyStates == EEnemyStates::EES_Engaged;
}

bool AEnemy::InTargetRange(AActor* Target, double TargetRadius)
{
	if (Target==nullptr) return false;
	const double DistanceToTarget=(Target->GetActorLocation()-GetActorLocation()).Size();
	return DistanceToTarget<=TargetRadius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (AIController== nullptr || Target== nullptr) return ;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(50.f);
	AIController->MoveTo(MoveRequest);
}

AActor* AEnemy::SelectPatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target!=PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	int32 NumPatrolTargets=ValidTargets.Num();
	if (NumPatrolTargets>0)
	{
		int32 Selection=FMath::RandRange(0,NumPatrolTargets-1);
		return ValidTargets[Selection];
	}
	return nullptr;
}

bool AEnemy::IsDead()
{
	return EnemyStates == EEnemyStates::EES_Dead;
}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(),FName("RightHandSocket"),this, this);
		EquipedWeapon=DefaultWeapon;
	}
}






void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	
	AIController->bSetControlRotationFromPawnOrientation = true;
	
	InitializeEnemy();
	
	
}

void AEnemy::Die()
{
	PlayDeathMontage();
	ClearAttackTimer();
	EnemyStates=EEnemyStates::EES_Dead;
	SetLifeSpan(DeathLifeSpan);
	DisableCapsule();
}

void AEnemy::Attack()
{
	Super::Attack();
	if (AIController)
	{
		AIController->StopMovement();
	}
	EnemyStates=EEnemyStates::EES_Engaged;
	PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
	return IsInsideAttackRadius() &&
		!IsAttacking() &&
			!IsDead();
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBar)
	{
		HealthBar->SetHealthBarPercentage(Attributes->GetHealthPercentage());
	}
}

int32 AEnemy::PlayDeathMontage()
{
	const int32 Selection = Super::PlayDeathMontage();
	TEnumAsByte<EDeathPose> pose(Selection);
	if (pose < EDeathPose::EDP_MAX) DeathPose = pose;
	return Selection;
}

void AEnemy::AttackEnd()
{
	Super::AttackEnd();
	EnemyStates=EEnemyStates::EES_NoState;
	CheckCombatTarget();
}
