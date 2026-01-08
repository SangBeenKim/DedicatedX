#include "Gimmick/DXLandMine.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"


ADXLandMine::ADXLandMine()
{
	PrimaryActorTick.bCanEverTick = false;

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
	
	UKismetSystemLibrary::PrintString(
		this, 
		FString::Printf(TEXT("ADXLandMine::BeginPlay()")), 
		true, 
		true, 
		FLinearColor::Green, 
		5.f
	);
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
