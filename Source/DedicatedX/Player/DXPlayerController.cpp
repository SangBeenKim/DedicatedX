// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DXPlayerController.h"
#include "DedicatedX.h"

ADXPlayerController::ADXPlayerController()
{
	DX_LOG_NET(LogDXNet, Log, TEXT(""));
}

void ADXPlayerController::PostNetInit()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::PostNetInit();

	if (IsLocalController())
	{
		UNetDriver* ClientNetDriver = GetNetDriver();
		if (IsValid(ClientNetDriver))
		{
			UNetConnection* ServerConnection = ClientNetDriver->ServerConnection;
			if (IsValid(ServerConnection))
			{
				DX_LOG_NET(LogDXNet, Log, TEXT("Server Connection: %s"), *ClientNetDriver->ServerConnection->GetName());
			}
			else
			{
				DX_LOG_NET(LogDXNet, Log, TEXT("There is no server connection."));
			}
		}
		else
		{
			DX_LOG_NET(LogDXNet, Log, TEXT("ClientNetDriver is invalid."));
		}
	}

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

void ADXPlayerController::OnActorChannelOpen(FInBunch& InBunch, UNetConnection* Connection)
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::OnActorChannelOpen(InBunch, Connection);

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

void ADXPlayerController::PostInitializeComponents()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::PostInitializeComponents();

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

void ADXPlayerController::BeginPlay()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::BeginPlay();

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

void ADXPlayerController::OnPossess(APawn* InPawn)
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::OnPossess(InPawn);

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}
