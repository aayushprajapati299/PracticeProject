


#include "BreakableActor.h"

#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "MyProject/Actors/Treasure.h"


ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometyCollection=CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Collection"));
	SetRootComponent(GeometyCollection);
	GeometyCollection->SetGenerateOverlapEvents(true);
	GeometyCollection->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GeometyCollection->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);

	CapsuleComponent= CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->SetupAttachment(GetRootComponent());
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);
}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (Bbroken) return;
	Bbroken=true;
	IHitInterface::GetHit_Implementation(ImpactPoint);
	UWorld * World = GetWorld();
	if (World && TreasureClasses.Num()>0)
	{
		FVector Location=GetActorLocation();
		Location.Z+=75;
		int32 Seletion =FMath::RandRange(0,TreasureClasses.Num()-1);
		World->SpawnActor<ATreasure>(TreasureClasses[Seletion],Location,GetActorRotation());
	}
	
}


void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

