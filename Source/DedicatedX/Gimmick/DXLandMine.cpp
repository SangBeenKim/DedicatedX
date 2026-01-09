#include "Gimmick/DXLandMine.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"


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
