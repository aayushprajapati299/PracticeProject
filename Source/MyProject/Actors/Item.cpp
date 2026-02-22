

#include "Item.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include"MyProject/Characters/SlashCharacter.h"

AItem::AItem()
{
    PrimaryActorTick.bCanEverTick = true;
    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh Name"));
    SetRootComponent(ItemMesh);

    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    Sphere->SetupAttachment(GetRootComponent());
    Sphere->SetSphereRadius(300.f);

    EmberEffect=CreateDefaultSubobject<UNiagaraComponent>(TEXT("Ember Effect"));
    EmberEffect->SetupAttachment(GetRootComponent());
}


void AItem::BeginPlay()
{
    Super::BeginPlay();
    
    Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);

    Sphere->OnComponentEndOverlap.AddDynamic(this,&AItem::OnSphereEndOverlap);
}

float AItem::TransformedSin()
{
    return Amplitude * FMath::Sin(Runtime * TimeConstant);
}
 

float AItem::TransformedCos()
{
    return Amplitude * FMath::Cos(Runtime * TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
    if (SlashCharacter)
    {
        SlashCharacter->SetOverlappingItem(this);
    }
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
    if (SlashCharacter)
    {
        SlashCharacter->SetOverlappingItem(nullptr);
    }
}

void AItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    Runtime += DeltaTime;

    if (ItemState==EItemState::EIS_Hovering) AddActorWorldOffset(FVector(0.0f,0.f,TransformedSin()));
}
