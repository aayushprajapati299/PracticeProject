


#include "SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "GroomComponent.h"
#include "MyProject/Actors/Weapons/Weapon.h"
#include "MyProject/Actors/Item.h"
#include "MyProject/Actors/Weapons/GreatSword.h"
#include "Animation/AnimMontage.h"


ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");



	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInput->BindAction(EquipAction,ETriggerEvent::Started,this, &ASlashCharacter::EKeyPressed);
		EnhancedInput->BindAction(AttackAction,ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
	}

}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMapping, 0);
		}
	}
	Tags.Add(FName("SlashCharacter"));
	
}

void ASlashCharacter::Move(const FInputActionValue& value)
{

	if (ActionState!=EActionState::EAS_Unoccupied) return;
	
	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawInput(0.f, ControlRotation.Yaw , 0.f);
	const FVector Direction = FRotationMatrix(YawInput).GetUnitAxis(EAxis::X);

	const FVector Right = FRotationMatrix(YawInput).GetUnitAxis(EAxis::Y);

	const FVector2D ActionValue = value.Get<FVector2D>();
	if (Controller && (ActionValue.X != 0 || ActionValue.Y!=0))
	{
		AddMovementInput(Direction, ActionValue.X);
		AddMovementInput(Right, ActionValue.Y);
	}
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D ActionValue = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(ActionValue.X);
		AddControllerPitchInput(ActionValue.Y);
	}
}

void ASlashCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(this->GetMesh(), FName("RightHandSocket"),this,this);
	CharacterState = ECharacterState::ECS_EquippedOnehandedWeapon;
	OverlappingItem=nullptr;
	EquipedWeapon=Weapon;
}

void ASlashCharacter::Disarm()
{
	PlayEquipMontage("Unequip");
	CharacterState= ECharacterState::ECS_UnEquipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::Arm()
{
	PlayEquipMontage("Equip");
	CharacterState= ECharacterState::ECS_EquippedOnehandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::EKeyPressed()
{
	
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);

	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
	}
	else
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm() )
		{
			Arm();
		}
	}
}

void ASlashCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void ASlashCharacter::AttackEnd()
 {
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanAttack()
{
	return CharacterState != ECharacterState::ECS_UnEquipped && ActionState == EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied
	&& CharacterState!=ECharacterState::ECS_UnEquipped;
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied
	&& CharacterState==ECharacterState::ECS_UnEquipped
	&& EquipedWeapon;
}

void ASlashCharacter::AttackWeaponToHand()
{
	if (EquipedWeapon)
	{
		EquipedWeapon->AttachMeshToSocket(GetMesh(),FName("RightHandSocket"));
	}
}

void ASlashCharacter::AttackWeaponToSpine()
{
	if (EquipedWeapon)
	{
		EquipedWeapon->AttachMeshToSocket(GetMesh(),FName("SpineSocket"));
	}
}

void ASlashCharacter::EquipEnd()
{
	ActionState=EActionState::EAS_Unoccupied;
}

void ASlashCharacter::SetCharacterSpeed(float MovementSpeed)
{
	if (UCharacterMovementComponent* MovementComponent= GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = MovementSpeed;
	}
}


















