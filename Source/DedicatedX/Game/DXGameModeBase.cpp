#include "Game/DXGameModeBase.h"
#include "Player/DXPlayerController.h"

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
