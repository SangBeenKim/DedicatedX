#include "Player/DXPlayerCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Gimmick/DXLandMine.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Component/DXStatusComponent.h"
#include "Component/DXHPTextWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/UW_HPText.h"

ADXPlayerCharacter::ADXPlayerCharacter()
	: CurrentAimPitch(0.f)
	, PrevioutAimPitch(0.f)
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	StatusComponent = CreateDefaultSubobject<UDXStatusComponent>(TEXT("StatusComponent"));

	HPTextWidgetComponent = CreateDefaultSubobject<UDXHPTextWidgetComponent>(TEXT("HPTextWidgetComponent"));
	HPTextWidgetComponent->SetupAttachment(GetRootComponent());
	HPTextWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	HPTextWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	HPTextWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADXPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocallyControlled())
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		checkf(IsValid(PC), TEXT("PlayerController is invalid."));

		UEnhancedInputLocalPlayerSubsystem* EILPS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		EILPS->AddMappingContext(InputMappingContext, 0);

		
	}
}

void ADXPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::HandleMoveInput);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::HandleLookInput);
	EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EIC->BindAction(LandMineAction, ETriggerEvent::Started, this, &ThisClass::HandleLandMineInput);
	EIC->BindAction(MeleeAttackAction, ETriggerEvent::Started, this, &ThisClass::HandleMeleeAttackInput);

}

void ADXPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentAimPitch);
}

void ADXPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(GetController()))
	{
		PrevioutAimPitch = CurrentAimPitch;

		FRotator ControlRotation = GetController()->GetControlRotation();
		float NormalizedPitch = FRotator::NormalizeAxis(ControlRotation.Pitch);
		CurrentAimPitch = FMath::Clamp(NormalizedPitch, -90.f, 90.f);
	}

	if (IsLocallyControlled() == true && PrevioutAimPitch != CurrentAimPitch)
	{
		ServerRPCUpdateAimValue(CurrentAimPitch);
	}

	if (IsValid(HPTextWidgetComponent) == true && GetNetMode() != NM_DedicatedServer)
	{
		FVector WidgetComponentLocation = HPTextWidgetComponent->GetComponentLocation();
		FVector LocalPlayerCameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();
		HPTextWidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(WidgetComponentLocation, LocalPlayerCameraLocation));
	}
}

void ADXPlayerCharacter::ServerRPCSpawnLandMine_Implementation()
{
	if (IsValid(LandMineClass))
	{
		FVector SpawnedLocation = (GetActorLocation() + GetActorForwardVector() * 300.f) - FVector(0.f, 0.f, 90.f);
		ADXLandMine* SpawnedLandMine = GetWorld()->SpawnActor<ADXLandMine>(LandMineClass, SpawnedLocation, FRotator::ZeroRotator);

		SpawnedLandMine->SetOwner(this);
	}
}

bool ADXPlayerCharacter::ServerRPCSpawnLandMine_Validate()
{
	return true;
}

void ADXPlayerCharacter::ServerRPCUpdateAimValue_Implementation(const float& InAimPitchValue)
{
	CurrentAimPitch = InAimPitchValue;
}

void ADXPlayerCharacter::HandleMoveInput(const FInputActionValue& InValue)
{
	if (!IsValid(Controller))
	{
		UE_LOG(LogTemp, Error, TEXT("Controller is invalid."));
		return;
	}

	const FVector2D InMovementVector = InValue.Get<FVector2D>();
	
	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator ControlYawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InMovementVector.X);
	AddMovementInput(RightDirection, InMovementVector.Y);
}

void ADXPlayerCharacter::HandleLookInput(const FInputActionValue& InValue)
{
	if (!IsValid(Controller))
	{
		UE_LOG(LogTemp, Error, TEXT("Controller is invalid."));
		return;
	}

	const FVector2D InLookVector = InValue.Get<FVector2D>();

	AddControllerYawInput(InLookVector.X);
	AddControllerPitchInput(InLookVector.Y);
}

void ADXPlayerCharacter::HandleLandMineInput(const FInputActionValue& InValue)
{
	if (IsLocallyControlled())
	{
		ServerRPCSpawnLandMine();
	}
}

void ADXPlayerCharacter::HandleMeleeAttackInput(const FInputActionValue& InValue)
{
	ServerRPCMeleeAttack();

	PlayMeleeAttackMontage();
}

float ADXPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UKismetSystemLibrary::PrintString(
		GetWorld(), 
		FString::Printf(TEXT("TakeDamage: %f"), DamageAmount), 
		true, 
		true, 
		FLinearColor::Red, 
		5.f
	);

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	StatusComponent->ApplyDamage(ActualDamage);

	return ActualDamage;
}

void ADXPlayerCharacter::CheckMeleeAttackHit()
{
	if (!HasAuthority()) return;

	TArray<FHitResult> OutHitResults;
	TSet<ACharacter*> DamagedCharacters;
	FCollisionQueryParams Params(NAME_None, false, this);

	const float MeleeAttackRange = 50.f;
	const float MeleeAttackRadius = 50.f;
	const float MeleeAttackDamage = 10.f;
	const FVector Forward = GetActorForwardVector();
	const FVector Start = GetActorLocation() + Forward * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * MeleeAttackRange;

	bool bIsHitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_Camera,
		FCollisionShape::MakeSphere(MeleeAttackRadius),
		Params
	);
	
	if (bIsHitDetected == true)
	{
		for (auto const& OutHitResult : OutHitResults)
		{
			ACharacter* DamagedCharacter = Cast<ACharacter>(OutHitResult.GetActor());
			if (IsValid(DamagedCharacter))
			{
				DamagedCharacters.Add(DamagedCharacter);
			}
		}

		FDamageEvent DamageEvent;
		for (auto const& DamagedCharacter : DamagedCharacters)
		{
			DamagedCharacter->TakeDamage(MeleeAttackDamage, DamageEvent, GetController(), this);
		}
	}

	FColor DrawColor = bIsHitDetected ? FColor::Green : FColor::Red;

	DrawDebugMeleeAttack(DrawColor, Start, End, Forward);
}

void ADXPlayerCharacter::ServerRPCMeleeAttack_Implementation()
{
	MulticastRPCMeleeAttack();

	PlayMeleeAttackMontage();
}

void ADXPlayerCharacter::MulticastRPCMeleeAttack_Implementation()
{
	if (HasAuthority()) return;

	if (IsLocallyControlled()) return;

	PlayMeleeAttackMontage();
}

void ADXPlayerCharacter::HandleMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) && AnimInstance->OnMontageEnded.IsAlreadyBound(this, &ThisClass::HandleMontageEnd))
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &ThisClass::HandleMontageEnd);
	}
}

void ADXPlayerCharacter::PlayMeleeAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance)) return;

	if (AnimInstance->Montage_IsPlaying(MeleeAttackMontage)) return;

	if (GetCharacterMovement()->IsFalling()) return;

	AnimInstance->StopAllMontages(0.f);
	AnimInstance->Montage_Play(MeleeAttackMontage);

	GetCharacterMovement()->SetMovementMode(MOVE_None);

	if (!AnimInstance->OnMontageEnded.IsAlreadyBound(this, &ThisClass::HandleMontageEnd))
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::HandleMontageEnd);
	}
}

void ADXPlayerCharacter::DrawDebugMeleeAttack(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward)
{
	const float MeleeAttackRange = 50.f;
	const float MeleeAttackRadius = 50.f;
	FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
	float CapsuleHalfHeight = MeleeAttackRange * 0.5f;
	DrawDebugCapsule(
		GetWorld(),
		CapsuleOrigin,
		CapsuleHalfHeight,
		MeleeAttackRadius,
		FRotationMatrix::MakeFromZ(Forward).ToQuat(),
		DrawColor,
		false,
		5.f
	);
}

void ADXPlayerCharacter::SetHPTextWidget(UUW_HPText* InHPTextWidget)
{
	UUW_HPText* HPWidget = Cast<UUW_HPText>(InHPTextWidget);
	if (IsValid(HPWidget))
	{
		HPWidget->InitializeHPTextWidget(StatusComponent);
		StatusComponent->OnCurrentHPChanged.AddUObject(HPWidget, &UUW_HPText::OnCurrentHPChange);
		StatusComponent->OnMaxHPChanged.AddUObject(HPWidget, &UUW_HPText::OnMaxHPChange);
	}
}

void ADXPlayerCharacter::TakeBuff(float InBuffValue)
{
	if (IsValid(StatusComponent))
	{
		StatusComponent->SetMaxHP(StatusComponent->GetMaxHP() + InBuffValue);
		StatusComponent->SetCurrentHP(StatusComponent->GetCurrentHP() + InBuffValue);
	}
}

