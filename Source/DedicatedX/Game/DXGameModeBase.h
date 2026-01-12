#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DXGameModeBase.generated.h"

class ADXPlayerController;

UCLASS()
class DEDICATEDX_API ADXGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
#pragma region AGameModeBase Override

public:
	ADXGameModeBase();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	void OnCharacterDead(ADXPlayerController* InController);

private:
	UFUNCTION()
	void OnMainTimerElapesd();
	void RefreshPlayerControllers();
	void NotifyToAllPlayer(const FString& NotificationString);

public:
	FTimerHandle MainTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WaitingTime = 15;
	int32 RemainWaitingTimeForPlaying = 15;
	int32 MinimumPlayerCountForPlaying = 2;

protected:
	UPROPERTY()
	TArray<TWeakObjectPtr<ADXPlayerController>> AlivePlayerControllers;
	UPROPERTY()
	TArray<TWeakObjectPtr<ADXPlayerController>> DeadPlayerControllers;

#pragma endregion

};
