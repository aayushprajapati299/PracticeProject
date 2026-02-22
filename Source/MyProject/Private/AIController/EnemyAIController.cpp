// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController/EnemyAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 2000.f;
	// depends on your game;
	SightConfig->PeripheralVisionAngleDegrees = 360.f;
	
	GetAIPerceptionComponent()->ConfigureSense(*SightConfig);
	GetAIPerceptionComponent()->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	GetAIPerceptionComponent()->OnTargetPerceptionUpdated.AddUniqueDynamic(this,&ThisClass::OnTargetPerceptionUpdate);
	
}

void AEnemyAIController::OnTargetPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	
}
