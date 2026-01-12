#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DXPlayerController.generated.h"

UCLASS()
class DEDICATEDX_API ADXPlayerController : public APlayerController
{
	GENERATED_BODY()
	
#pragma region APlayerController Override

public:
	ADXPlayerController();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FText NotificationText;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ADXPlayerController", meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> NotificationTextUIClass;
#pragma endregion

};
