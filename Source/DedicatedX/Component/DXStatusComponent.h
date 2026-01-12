#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DXStatusComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnOutOfCurrentHPDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentHPChangedDelegate, float /*InCurrentHP*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxHPChangedDelegate, float /*InMaxHP*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEDICATEDX_API UDXStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDXStatusComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	float ApplyDamage(float InDamage);
	inline float GetCurrentHP() const { return CurrentHP; }
	void SetCurrentHP(float InCurrentHP);
	inline float GetMaxHP() const { return MaxHP; }
	void SetMaxHP(float InMaxHP);

protected:
	UFUNCTION()
	void OnRep_CurrentHP();
	UFUNCTION()
	void OnRep_MaxHP();

public:
	FOnOutOfCurrentHPDelegate OnOutOfCurrentHP;
	FOnCurrentHPChangedDelegate OnCurrentHPChanged;
	FOnMaxHPChangedDelegate OnMaxHPChanged;

private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHP)
	float CurrentHP;
	UPROPERTY(ReplicatedUsing = OnRep_MaxHP)
	float MaxHP;
};
