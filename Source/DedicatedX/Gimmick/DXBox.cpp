#include "Gimmick/DXBox.h"
#include "DedicatedX.h"
#include "Components/TextRenderComponent.h"

ADXBox::ADXBox()
	: SceneRoot(nullptr)
	, Mesh(nullptr)
	, TextRender(nullptr)
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


