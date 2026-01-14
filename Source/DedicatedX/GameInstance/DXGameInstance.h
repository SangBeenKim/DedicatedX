#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "DXGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString SessionName;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayerCount;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayerCount;
	UPROPERTY(BlueprintReadOnly)
	bool bUseLan;
	UPROPERTY(BlueprintReadOnly)
	bool bIsDedicated;
	UPROPERTY(BlueprintReadOnly)
	int32 SessionIndex;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionFoundDelegate, FSessionInfo, FoundSessionInfo);

UCLASS()
class DEDICATEDX_API UDXGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
protected:
	UFUNCTION(BlueprintCallable)
	void CreateSession(bool bUseLan, bool bIsDedicatedServer);
	void OnCreateSessionCompleted(FName InSessionName, bool bWasSuccessful);
	UFUNCTION(BlueprintCallable)
	void FindSession(bool bUseLan, bool bIsDedicatedServer);
	void OnFindSessionsComplete(bool bWasSuccessful);
	UFUNCTION(BlueprintCallable)
	void JoinSession(int32 InSessionNumber);
	void OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type InResult);

public:
	UPROPERTY(BlueprintAssignable)
	FOnSessionFoundDelegate OnSessionFound;

protected:
	TWeakPtr<IOnlineSession> SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	TArray<FOnlineSessionSearchResult> SessionSearchResults;
	
};
