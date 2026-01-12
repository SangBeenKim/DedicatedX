#include "UI/UW_TitleLayout.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Controller/DXTitlePlayerController.h"
#include "Kismet/GameplayStatics.h"

UUW_TitleLayout::UUW_TitleLayout(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUW_TitleLayout::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnPlayButtonClicked);
	HostButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnHostButtonClicked);
	ExitButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnExitButtonClicked);
}

void UUW_TitleLayout::OnPlayButtonClicked()
{
	ADXTitlePlayerController* PC = GetOwningPlayer<ADXTitlePlayerController>();
	if (IsValid(PC))
	{
		FText ServerIP = ServerIPEditableText->GetText();
		PC->JoinServer(ServerIP.ToString());
	}
}

void UUW_TitleLayout::OnHostButtonClicked()
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false || World->GetNetMode() != NM_Standalone)
	{
		return;
	}

	const FName LevelName(TEXT("ThirdPersonMap"));
	const FString Options(TEXT("listen"));

	UGameplayStatics::OpenLevel(World, LevelName, true, Options);
}

void UUW_TitleLayout::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
