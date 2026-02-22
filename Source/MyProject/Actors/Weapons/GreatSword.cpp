// Fill out your copyright notice in the Description page of Project Settings.


#include "GreatSword.h"

#include "MyProject/Characters/SlashCharacter.h"


void AGreatSword::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AGreatSword::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AGreatSword::Equip(USceneComponent* InParent, FName Socket)
{
	FAttachmentTransformRules TransformRules(
		EAttachmentRule::SnapToTarget, // Location rule
		EAttachmentRule::SnapToTarget, // Roxtation rule
		EAttachmentRule::KeepWorld,    // Scale rule
		true
	);
	ItemState=EItemState::EIS_Equipped;
	AttachToComponent(InParent,TransformRules,Socket);
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(InParent->GetOwner());
	if (SlashCharacter)
	{
		SlashCharacter->SetCharacterSpeed(300.f);
	}
}
