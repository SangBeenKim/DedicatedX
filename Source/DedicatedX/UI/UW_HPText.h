#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_HPText.generated.h"

class UDXStatusComponent;
class UTextBlock;

UCLASS()
class DEDICATEDX_API UUW_HPText : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUW_HPText(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	void InitializeHPTextWidget(UDXStatusComponent* InStatusComponent);
	inline AActor* GetOwningActor() const { return OwningActor; }
	inline void SetOwningActor(AActor* InOwningActor) { OwningActor = InOwningActor; }
	UFUNCTION()
	void OnMaxHPChange(float InMaxHP);
	UFUNCTION()
	void OnCurrentHPChange(float InCurrentHP);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentHPText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxHPText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> OwningActor;
};
