// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DXGameStateBase.h"
#include "DedicatedX.h"

ADXGameStateBase::ADXGameStateBase()
{
	DX_LOG_NET(LogDXNet, Log, TEXT(""));
}

void ADXGameStateBase::HandleBeginPlay()
{
	DX_LOG_NET(LogDXNet, Error, TEXT("Begin"));

	Super::HandleBeginPlay();

	DX_LOG_NET(LogDXNet, Error, TEXT("End"));
}

void ADXGameStateBase::OnRep_ReplicatedHasBegunPlay()
{
	DX_LOG_NET(LogDXNet, Error, TEXT("Begin"));

	Super::OnRep_ReplicatedHasBegunPlay();

	DX_LOG_NET(LogDXNet, Error, TEXT("End"));
}
