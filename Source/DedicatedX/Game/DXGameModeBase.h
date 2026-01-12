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

private:
	void RefreshPlayerControllers();

protected:
	UPROPERTY()
	TArray<TWeakObjectPtr<ADXPlayerController>> AlivePlayerControllers;
	UPROPERTY()
	TArray<TWeakObjectPtr<ADXPlayerController>> DeadPlayerControllers;

#pragma endregion

};
