#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DXPlayerController.generated.h"

class UUW_GameResult;

UCLASS()
class DEDICATEDX_API ADXPlayerController : public APlayerController
{
	GENERATED_BODY()
	
#pragma region APlayerController Override

public:
	ADXPlayerController();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void OnCharacterDead();
	UFUNCTION(Client, Reliable)
	void ClientRPCShowGameResultWidget(int32 InRanking);
	UFUNCTION(Client, Reliable)
	void ClientRPCReturnToTitle();

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FText NotificationText;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ADXPlayerController", meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> NotificationTextUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ADXPlayerController", meta = (AllowPrivateAccess))
	TSubclassOf<UUW_GameResult> GameResultUIClass;

#pragma endregion

};
