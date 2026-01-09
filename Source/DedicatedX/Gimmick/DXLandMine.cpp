#include "Gimmick/DXLandMine.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"

ADXLandMine::ADXLandMine()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetRootComponent());

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BoxCollision);

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(GetRootComponent());
	Particle->SetAutoActivate(false);
}

void ADXLandMine::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		UKismetSystemLibrary::PrintString(
			this,
			FString::Printf(TEXT("Run on server.")),
			true,
			true,
			FLinearColor::Green,
			5.f
		);
	}
	else
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (IsValid(OwnerPawn))
		{
			if (OwnerPawn->IsLocallyControlled())
			{
				UKismetSystemLibrary::PrintString(
					this,
					FString::Printf(TEXT("Run on owning client.")),
					true,
					true,
					FLinearColor::Green,
					5.f
				);
			}
			else
			{
				UKismetSystemLibrary::PrintString(
					this,
					FString::Printf(TEXT("Run on other client.")),
					true,
					true,
					FLinearColor::Green,
					5.f
				);
			}
		}
	}
	
	if (!OnActorBeginOverlap.IsAlreadyBound(this, &ThisClass::OnLandMineBeginOverlap))
	{
		OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnLandMineBeginOverlap);
	}
}

void ADXLandMine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("ADXLandMine::EndPlay()")),
		true,
		true,
		FLinearColor::Green,
		5.f
	);
}

void ADXLandMine::OnLandMineBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (HasAuthority())
	{
		UKismetSystemLibrary::PrintString(
			this,
			FString::Printf(TEXT("Run on server.")),
			true,
			true,
			FLinearColor::Green,
			5.f
		);

		MulticastRPCSpawnEffect();
	}
	else
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (IsValid(OwnerPawn))
		{
			if (OwnerPawn->IsLocallyControlled())
			{
				UKismetSystemLibrary::PrintString(
					this,
					FString::Printf(TEXT("Run on owning client.")),
					true,
					true,
					FLinearColor::Green,
					5.f
				);
			}
			else
			{
				UKismetSystemLibrary::PrintString(
					this,
					FString::Printf(TEXT("Run on other client.")),
					true,
					true,
					FLinearColor::Green,
					5.f
				);
			}
		}
	}
}

void ADXLandMine::MulticastRPCSpawnEffect_Implementation()
{
	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	if (World->GetNetMode() != NM_DedicatedServer)
	{
		Particle->Activate(true);
	}
}
