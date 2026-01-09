#include "Gimmick/DXLandMine.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

ADXLandMine::ADXLandMine()
	: NetCullDistance(1000.f)
	, bIsExploded(false)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	//bAlwaysRelevant = true;

	SetNetCullDistanceSquared(NetCullDistance * NetCullDistance);

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

void ADXLandMine::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsExploded);
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
	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	if (bIsExploded == false && World->GetNetMode() != NM_DedicatedServer)
	{
		Particle->Activate(true);
	}

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

		if (bIsExploded == false)
		{
			bIsExploded = true;
		}
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

void ADXLandMine::OnRep_IsExploded()
{
	if (bIsExploded == true && IsValid(ExplodedMaterial))
	{
		Mesh->SetMaterial(0, ExplodedMaterial);
	}
}

void ADXLandMine::MulticastRPCSpawnEffect_Implementation()
{
	//if (IsValid(ExplodedMaterial))
	//{
	//	Mesh->SetMaterial(0, ExplodedMaterial);
	//}
}
