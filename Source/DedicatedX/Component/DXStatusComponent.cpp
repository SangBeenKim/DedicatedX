#include "Component/DXStatusComponent.h"
#include "Net/UnrealNetwork.h"

UDXStatusComponent::UDXStatusComponent()
	: CurrentHP(100.f)
	, MaxHP(100.f)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UDXStatusComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentHP);
	DOREPLIFETIME(ThisClass, MaxHP);
}

float UDXStatusComponent::ApplyDamage(float InDamage)
{
	const float PreviousHP = CurrentHP;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, PreviousHP);

	SetCurrentHP(PreviousHP - ActualDamage);

	return ActualDamage;
}

void UDXStatusComponent::SetCurrentHP(float InCurrentHP)
{
	CurrentHP = InCurrentHP;
	if (CurrentHP <= KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.f;
		OnOutOfCurrentHP.Broadcast();
	}

	OnCurrentHPChanged.Broadcast(CurrentHP);
}

void UDXStatusComponent::SetMaxHP(float InMaxHP)
{
	MaxHP = InMaxHP;
	if (MaxHP < KINDA_SMALL_NUMBER)
	{
		MaxHP = 0.1f;
	}

	OnMaxHPChanged.Broadcast(MaxHP);

}

void UDXStatusComponent::OnRep_CurrentHP()
{
	OnCurrentHPChanged.Broadcast(CurrentHP);
}

void UDXStatusComponent::OnRep_MaxHP()
{
	OnMaxHPChanged.Broadcast(MaxHP);
}

