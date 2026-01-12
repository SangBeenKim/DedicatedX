#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DXTitlePlayerController.generated.h"

UCLASS()
class DEDICATEDX_API ADXTitlePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	void JoinServer(const FString& InIPAddress);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ASUIPlayerController", meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> UIWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ASUIPlayerController", meta = (AllowPrivateAccess))
	TObjectPtr<UUserWidget> UIWidgetInstance;
	
};
