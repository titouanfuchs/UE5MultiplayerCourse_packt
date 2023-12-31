// Fill out your copyright notice in the Description page of Project Settings.


#include "US_Character.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MultiplayerCourse/Interfaces/US_Interactable.h"
#include "Stats/US_CharacterStats.h"


// Sets default values
AUS_Character::AUS_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(60.f, 96.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -91.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Game/KayKit/Characters/rogue"));

	if (SkeletalMeshAsset.Succeeded()) GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(.0f, 500.f, .0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void AUS_Character::UpdateCharacterStats(int32 CharacterLevel)
{
	auto IsSprinting = false;

	if (GetCharacterStats())
	{
		IsSprinting = GetCharacterMovement()->MaxWalkSpeed == GetCharacterStats()->SprintSpeed;
	}
	
	if (!CharacterDataTable) return;

	TArray<FUS_CharacterStats*> CharacterStatsRow;
	CharacterDataTable->GetAllRows<FUS_CharacterStats>(TEXT("US_Character"), CharacterStatsRow);

	if (CharacterStatsRow.Num() < 0) return;

	const auto NewCharacterLevel = FMath::Clamp(CharacterLevel, 1, CharacterStatsRow.Num());

	CharacterStats = CharacterStatsRow[NewCharacterLevel - 1];

	GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->WalkSpeed;
	if (IsSprinting) SprintStart_Server();
}

// Called when the game starts or when spawned
void AUS_Character::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	UpdateCharacterStats(1);
}

void AUS_Character::Move(const FInputActionValue& Value)
{
	const auto MovementVector = Value.Get<FVector2D>();

	if (!Controller) return;

	const auto Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const auto ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const auto RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AUS_Character::Look(const FInputActionValue& Value)
{
	const auto LookAxisVector = Value.Get<FVector2D>();

	if(!Controller) return;

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AUS_Character::SprintStart(const FInputActionValue& Value)
{
	SprintStart_Server();
}

void AUS_Character::SprintStart_Server_Implementation()
{
	if (!GetCharacterStats()) return;
	
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->SprintSpeed;
}

void AUS_Character::SprintEnd(const FInputActionValue& Value)
{
	SprintEnd_Server();
}

void AUS_Character::SprintEnd_Server_Implementation()
{
	if (!GetCharacterStats()) return;
    
    GetCharacterMovement()->MaxWalkSpeed = GetCharacterStats()->WalkSpeed;
}

void AUS_Character::Interact(const FInputActionValue& Value)
{
	//GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, TEXT("Interact"));
	Interact_Server();
}

void AUS_Character::Interact_Server_Implementation()
{
	if (!InteractableActor) return;

	IUS_Interactable::Execute_Interact(InteractableActor, this);
}

// Called every frame
void AUS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ROLE_Authority) return;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this);

	auto SphereRadius = 50.f;
	auto StartLocation = GetActorLocation() + GetActorForwardVector() * 150.f;
	auto EndLocation = StartLocation + GetActorForwardVector() * 500.f;

	auto IsHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		SphereRadius,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true);

	if (IsHit && HitResult.GetActor()->GetClass()->ImplementsInterface(UUS_Interactable::StaticClass()))
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactNormal, SphereRadius, 12, FColor::Magenta, false, 1.f);
		InteractableActor = HitResult.GetActor();
	}else
	{
		InteractableActor = nullptr;
	}
}

// Called to bind functionality to input
void AUS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUS_Character::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUS_Character::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AUS_Character::Interact);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AUS_Character::SprintStart);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AUS_Character::SprintEnd);
	}
}

