#include "Game/DXGameModeBase.h"
#include "Player/DXPlayerController.h"
#include "Game/DXGameStateBase.h"

ADXGameModeBase::ADXGameModeBase()
{
}

void ADXGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	RefreshPlayerControllers();

	ADXPlayerController* NewPlayerController = Cast<ADXPlayerController>(NewPlayer);
	if (IsValid(NewPlayerController))
	{
		AlivePlayerControllers.Add(NewPlayerController);
		NewPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
	}
}

void ADXGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	RefreshPlayerControllers();

	ADXPlayerController* ExitingPlayerController = Cast<ADXPlayerController>(Exiting);
	if (IsValid(ExitingPlayerController) == true && AlivePlayerControllers.Find(ExitingPlayerController) != INDEX_NONE)
	{
		AlivePlayerControllers.Remove(ExitingPlayerController);
		DeadPlayerControllers.Remove(ExitingPlayerController);
	}
}

void ADXGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &ThisClass::OnMainTimerElapesd, 1.f, true);
	RemainWaitingTimeForPlaying = WaitingTime;
}

void ADXGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	ADXGameStateBase* DXGS = GetGameState<ADXGameStateBase>();
	if (!IsValid(DXGS)) return;

	if (DXGS->MatchState != EMatchState::Waiting)
	{
		ErrorMessage = FString(TEXT("DXGameState->MatchState != EMatchState::Waiting"));
		return;
	}
}

void ADXGameModeBase::OnMainTimerElapesd()
{
	ADXGameStateBase* DXGS = GetGameState<ADXGameStateBase>();
	if (!IsValid(DXGS)) return;

	switch (DXGS->MatchState)
	{
	case EMatchState::None:
		break;
	case EMatchState::Waiting: 
	{
		FString NotificationString = FString::Printf(TEXT(""));

		if (AlivePlayerControllers.Num() < MinimumPlayerCountForPlaying)
		{
			NotificationString = FString::Printf(TEXT("Wait another players for playing."));

			RemainWaitingTimeForPlaying = WaitingTime;
		}
		else
		{
			NotificationString = FString::Printf(TEXT("Wait %d seconds for playing."), RemainWaitingTimeForPlaying);

			--RemainWaitingTimeForPlaying;
		}

		if (RemainWaitingTimeForPlaying <= 0)
		{
			NotificationString = FString::Printf(TEXT(""));

			DXGS->MatchState = EMatchState::Playing;
		}

		NotifyToAllPlayer(NotificationString);

		break;
	}
	case EMatchState::Playing:
		break;
	case EMatchState::Ending:
		break;
	case EMatchState::End:
		break;
	default:
		break;
	}
}

void ADXGameModeBase::RefreshPlayerControllers()
{
	for (int32 i = AlivePlayerControllers.Num() - 1; i >= 0; --i)
	{
		ADXPlayerController* DXPlayerController = AlivePlayerControllers[i].Get();
		if (!IsValid(DXPlayerController))
		{
			AlivePlayerControllers.RemoveAt(i);
			continue;
		}
	}

	for (int32 i = DeadPlayerControllers.Num() - 1; i >= 0; --i)
	{
		ADXPlayerController* DXPlayerController = DeadPlayerControllers[i].Get();
		if (!IsValid(DXPlayerController))
		{
			DeadPlayerControllers.RemoveAt(i);
			continue;
		}
	}
}

void ADXGameModeBase::NotifyToAllPlayer(const FString& NotificationString)
{
	for (auto AlivePlayerController : AlivePlayerControllers)
	{
		AlivePlayerController->NotificationText = FText::FromString(NotificationString);
	}

	for (auto DeadPlayerController : DeadPlayerControllers)
	{
		DeadPlayerController->NotificationText = FText::FromString(NotificationString);
	}
}
