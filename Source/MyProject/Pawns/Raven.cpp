


#include "Raven.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"


ARaven::ARaven()
{
	PrimaryActorTick.bCanEverTick = true;
	
	
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;
	Capsule->SetCapsuleHalfHeight(15.f);
	Capsule->SetCapsuleRadius(20.f);

	RavenMesh= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RavenMesh"));
	RavenMesh->SetupAttachment(Capsule);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Capsule);
	SpringArm->TargetArmLength = 30.f;
	
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCamera->SetupAttachment(SpringArm);

	


}


void ARaven::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			LocalPlayerSubSystem->AddMappingContext(RavenMapping,0);
		}
	}
	
}

void ARaven::Move(const FInputActionValue& value)
{
	const float ActionValue = value.Get<float>();
	FVector Direction = GetActorForwardVector();
	if (Controller && ActionValue != 0.f)
	{
		AddMovementInput(Direction, ActionValue);
	}
}

void ARaven::Look(const FInputActionValue& value)
{
	const FVector2D LookValue = value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerPitchInput(LookValue.Y);
		AddControllerYawInput(LookValue.X);
	}
}

void ARaven::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARaven::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARaven::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARaven::Look);
	}

}


