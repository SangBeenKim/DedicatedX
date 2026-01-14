#include "GameInstance/DXGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Online/OnlineSessionNames.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UDXGameInstance::Init()
{
	Super::Init();

	IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
	if (OnlineSubsystem != nullptr)
	{
		UKismetSystemLibrary::PrintString(
			this,
			FString::Printf(TEXT("[01] OnlineSubsystem found.")),
			true,
			true,
			FLinearColor::Blue,
			100.f
		);

		SessionInterface = OnlineSubsystem->GetSessionInterface();
		TSharedPtr<IOnlineSession> PinnedSessionInterface = SessionInterface.Pin();
		if (PinnedSessionInterface != nullptr)
		{
			UKismetSystemLibrary::PrintString(
				this,
				FString::Printf(TEXT("[02] SessionInterface found.")),
				true,
				true,
				FLinearColor::Blue,
				100.f
			);

			PinnedSessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionCompleted);
			PinnedSessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::OnFindSessionsComplete);
			PinnedSessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ThisClass::OnJoinSessionComplete);

			ENetMode NetMode = GetWorld()->GetNetMode();
			if (NetMode == NM_DedicatedServer)
			{
				CreateSession(true, true);
			}
		}
		else
		{
			UKismetSystemLibrary::PrintString(
				this,
				FString::Printf(TEXT("[02] SessionInterface not found.")),
				true,
				true,
				FLinearColor::Blue,
				100.f
			);
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(
			this,
			FString::Printf(TEXT("[01] OnlineSubsystem not found.")),
			true,
			true,
			FLinearColor::Blue,
			100.f
		);
	}
}

void UDXGameInstance::CreateSession(bool bUseLan, bool bIsDedicatedServer)
{
	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("[03] CreateServer(%s, %s) called."), 
			bUseLan ? TEXT("true") : TEXT("false"), 
			bIsDedicatedServer ? TEXT("true") : TEXT("false")),
		true,
		true,
		FLinearColor::Blue,
		100.f
	);

	IOnlineSubsystem* OnlineSubsystem = Online::GetSubsystem(GetWorld());
	FName OnlineSubsystemName = OnlineSubsystem ? OnlineSubsystem->GetSubsystemName() : NAME_None;
	bool bIsSteam = (OnlineSubsystemName == "STEAM");

	FOnlineSessionSettings SessionSettings;
	SessionSettings.NumPublicConnections = 4;
	SessionSettings.NumPrivateConnections = 0;

	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bAllowJoinInProgress = true;

	SessionSettings.bIsLANMatch = bIsSteam ? false : bUseLan;
	SessionSettings.bIsDedicated = bIsDedicatedServer;

	SessionSettings.bUsesStats = !bIsDedicatedServer;
	SessionSettings.bAllowInvites = !bIsDedicatedServer;

	SessionSettings.bUsesPresence = !bIsDedicatedServer;
	SessionSettings.bAllowJoinViaPresence = !bIsDedicatedServer;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = !bIsDedicatedServer;

	SessionSettings.bUseLobbiesIfAvailable = !bIsDedicatedServer;
	SessionSettings.bUseLobbiesVoiceChatIfAvailable = !bIsDedicatedServer;

	const FName SessionName = FName(TEXT("MySession"));
	TSharedPtr<IOnlineSession> PinnedSessionInterface = SessionInterface.Pin();
	bool bCreateResult;
	if (PinnedSessionInterface != nullptr)
	{
		bCreateResult = PinnedSessionInterface->CreateSession(0, SessionName, SessionSettings);
	}

	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("[03] CreateSession result: %s"), bCreateResult ? TEXT("true") : TEXT("false")),
		true,
		true,
		FLinearColor::Blue,
		100.f
	);
}

void UDXGameInstance::OnCreateSessionCompleted(FName InSessionName, bool bWasSuccessful)
{
	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("[04] OnCreateSessionCompleted(%s, %s)"),
			*InSessionName.ToString(), bWasSuccessful ? TEXT("true") : TEXT("false")),
		true,
		true,
		FLinearColor::Blue,
		100.f
	);

	if (bWasSuccessful)
	{
		UWorld* World = GetWorld();
		if (IsValid(World))
		{
			TSharedPtr<IOnlineSession> PinnedSessionInterface = SessionInterface.Pin();
			FOnlineSessionSettings* SessionSettings = nullptr;
			if (PinnedSessionInterface != nullptr)
			{
				SessionSettings = PinnedSessionInterface->GetSessionSettings(InSessionName);
			}

			if (SessionSettings == nullptr)
			{
				UKismetSystemLibrary::PrintString(
					this,
					FString::Printf(TEXT("[04] Failed to get SessionSettings for %s"), *InSessionName.ToString()),
					true,
					true,
					FLinearColor::Blue,
					100.f
				);
				return;
			}

			FString LevelURL = TEXT("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
			World->ServerTravel(LevelURL);
			UKismetSystemLibrary::PrintString(
				this,
				FString::Printf(TEXT("[04] ServerTravel(%s)"), *LevelURL),
				true,
				true,
				FLinearColor::Blue,
				100.f
			);
		}
	}
}

void UDXGameInstance::FindSession(bool bUseLan, bool bIsDedicatedServer)
{
	if (!SessionInterface.IsValid())
	{
		UKismetSystemLibrary::PrintString(
			this,
			FString::Printf(TEXT("[05] SessionInterface is invalid.")),
			true,
			true,
			FLinearColor::Blue,
			100.f
		);
		return;
	}

	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	FName SubsystemName = Subsystem ? Subsystem->GetSubsystemName() : NAME_None;
	bool bIsSteam = (SubsystemName == "STEAM");

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = bIsSteam ? false : bUseLan;
	SessionSearch->MaxSearchResults = 10000;
	
	if (bIsDedicatedServer)
	{
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	}
	else
	{
		SessionSearch->QuerySettings.Set(SEARCH_DEDICATED_ONLY, true, EOnlineComparisonOp::Equals);
	}

	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("[05] Searching for Sessions...")),
		true,
		true,
		FLinearColor::Blue,
		100.f
	);

	TSharedPtr<IOnlineSession> PinnedSessionInterface = SessionInterface.Pin();
	if (PinnedSessionInterface != nullptr)
	{
		PinnedSessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UDXGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("[06] OnFindSessionsComplete(%s)"), bWasSuccessful ? TEXT("true") : TEXT("false")),
		true,
		true,
		FLinearColor::Blue,
		100.f
	);

	if (bWasSuccessful == false)
	{
		UKismetSystemLibrary::PrintString(
			this,
			FString::Printf(TEXT("[06] FindSessions failed.")),
			true,
			true,
			FLinearColor::Blue,
			100.f
		);
		return;
	}

	if (!SessionSearch.IsValid())
	{
		UKismetSystemLibrary::PrintString(
			this,
			FString::Printf(TEXT("[06] SessionSearch invalid.")),
			true,
			true,
			FLinearColor::Blue,
			100.f
		);
		return;
	}

	SessionSearchResults = SessionSearch->SearchResults;

	int32 ServerCount = SessionSearchResults.Num();
	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("[06] SearchResults count = %d"), ServerCount),
		true,
		true,
		FLinearColor::Blue,
		100.f
	);

	for (int32 i = 0; i < ServerCount; ++i)
	{
		const FOnlineSessionSearchResult& Result = SessionSearchResults[i];
		
		if (!Result.IsValid()) continue;

		UKismetSystemLibrary::PrintString(
			this,
			FString::Printf(TEXT("[06] Server %d: SessionName=%s, Ping=%d"), i, *Result.GetSessionIdStr(), Result.PingInMs),
			true,
			true,
			FLinearColor::Blue,
			100.f
		);

		FSessionInfo SessionInfo;
		SessionInfo.SessionName = TEXT("TempName");
		int32 MaxPublicConnection = Result.Session.SessionSettings.NumPublicConnections;
		SessionInfo.MaxPlayerCount = MaxPublicConnection;
		int32 RemainPublicConnection = Result.Session.NumOpenPublicConnections;
		SessionInfo.CurrentPlayerCount = MaxPublicConnection - RemainPublicConnection;
		SessionInfo.bUseLan = Result.Session.SessionSettings.bIsLANMatch;
		SessionInfo.bIsDedicated = Result.Session.SessionSettings.bIsDedicated;
		SessionInfo.SessionIndex = i;

		OnSessionFound.Broadcast(SessionInfo);
	}
}

void UDXGameInstance::JoinSession(int32 InSessionNumber)
{
	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("[07] Joining first seesion...")),
		true,
		true,
		FLinearColor::Blue,
		100.f
	);

	TSharedPtr<IOnlineSession> PinnedSessionInterface = SessionInterface.Pin();
	if (PinnedSessionInterface != nullptr)
	{
		PinnedSessionInterface->JoinSession(0, FName("MySession"), SessionSearchResults[InSessionNumber]);
	}
}

void UDXGameInstance::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type InResult)
{
	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("[08] OnJoinSessionComplete(%s, %d)"), *InSessionName.ToString(), (int32)InResult),
		true,
		true,
		FLinearColor::Blue,
		100.f
	);

	if (InResult != EOnJoinSessionCompleteResult::Success)
	{
		UKismetSystemLibrary::PrintString(
			this,
			FString::Printf(TEXT("[08] JoinSession failed.")),
			true,
			true,
			FLinearColor::Blue,
			100.f
		);
		return;
	}

	FString JoinAddress;
	bool bGotAddress;
	TSharedPtr<IOnlineSession> PinnedSessionInterface = SessionInterface.Pin();
	if (PinnedSessionInterface != nullptr)
	{
		bGotAddress = PinnedSessionInterface->GetResolvedConnectString(InSessionName, JoinAddress);
	}

	if (!bGotAddress)
	{
		UKismetSystemLibrary::PrintString(
			this,
			FString::Printf(TEXT("[08] Could not ger resolved connect string")),
			true,
			true,
			FLinearColor::Blue,
			100.f
		);
		return;
	}

	UKismetSystemLibrary::PrintString(
		this,
		FString::Printf(TEXT("[08] Join address: %s"), *JoinAddress),
		true,
		true,
		FLinearColor::Blue,
		100.f
	);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC))
	{
		PC->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
	}

}