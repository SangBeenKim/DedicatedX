#include "Gimmick/DXBox.h"
#include "DedicatedX.h"
#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"

ADXBox::ADXBox()
	: SceneRoot(nullptr)
	, Mesh(nullptr)
	, TextRender(nullptr)
	, ServerRotationYaw(0.f)
	, RotationSpeed(30.f)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneRoot);
	Mesh->SetRelativeLocation(FVector(-50.f, -50.f, 0.f));

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetupAttachment(SceneRoot);
	TextRender->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	TextRender->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRender->SetText(FText::FromString(TEXT("Box")));
}

void ADXBox::BeginPlay()
{
	Super::BeginPlay();
	
	DX_LOG_ROLE(LogDXNet, Log, TEXT(""));
}

void ADXBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ServerRotationYaw);
}

void ADXBox::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaSeconds, 0.f));
		ServerRotationYaw = RootComponent->GetComponentRotation().Yaw;
	}
	else
	{
		//SetActorRotation(FRotator(0.f, ServerRotationYaw, 0.f));
	}
}

void ADXBox::OnRep_ServerRotationYaw()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("OnRep_ServerRotationYaw(): %f"), ServerRotationYaw);

	SetActorRotation(FRotator(0.f, ServerRotationYaw, 0.f));
}


