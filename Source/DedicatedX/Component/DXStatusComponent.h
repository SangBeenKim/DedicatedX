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
	float ApplyDamage(float InDamage);
	inline float GetCurrentHP() const { return CurrentHP; }
	void SetCurrentHP(float InCurrentHP);
	inline float GetMaxHP() const { return MaxHP; }
	void SetMaxHP(float InMaxHP);
	
	FOnOutOfCurrentHPDelegate OnOutOfCurrentHP;
	FOnCurrentHPChangedDelegate OnCurrentHPChanged;
	FOnMaxHPChangedDelegate OnMaxHPChanged;

private:
	UPROPERTY()
	float CurrentHP;
	UPROPERTY()
	float MaxHP;
};
