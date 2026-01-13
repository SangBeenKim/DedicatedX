#include "Player/DXPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/UserWidget.h"
#include "Game/DXGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/UW_GameResult.h"
#include "Components/TextBlock.h"

ADXPlayerController::ADXPlayerController()
{
}

void ADXPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocalController()) return;

	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);

	if (IsValid(NotificationTextUIClass))
	{
		UUserWidget* NotificationTextUI = CreateWidget<UUserWidget>(this, NotificationTextUIClass);
		if (IsValid(NotificationTextUI))
		{
			NotificationTextUI->AddToViewport(1);
			NotificationTextUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ADXPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void ADXPlayerController::OnCharacterDead()
{
	ADXGameModeBase* GM = Cast<ADXGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (HasAuthority() && IsValid(GM))
	{
		GM->OnCharacterDead(this);
	}
}

void ADXPlayerController::ClientRPCShowGameResultWidget_Implementation(int32 InRanking)
{
	if (!IsLocalController()) return;

	if (IsValid(GameResultUIClass))
	{
		UUW_GameResult* GameResultUI = CreateWidget<UUW_GameResult>(this, GameResultUIClass);
		if (IsValid(GameResultUI))
		{
			GameResultUI->AddToViewport(3);

			FString GameResultString = FString::Printf(TEXT("%s"), InRanking == 1 ? TEXT("Winner!") : TEXT("Looser..."));
			GameResultUI->ResultText->SetText(FText::FromString(GameResultString));

			FString RankingString = FString::Printf(TEXT("#%02d"), InRanking);
			GameResultUI->RankingText->SetText(FText::FromString(RankingString));

			FInputModeUIOnly Mode;
			Mode.SetWidgetToFocus(GameResultUI->GetCachedWidget());
			SetInputMode(Mode);

			bShowMouseCursor = true;
		}
	}
}
