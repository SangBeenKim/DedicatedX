// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DXPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class DEDICATEDX_API ADXPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region ACharacter Override

public:
	ADXPlayerCharacter();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Owner() override;
	virtual void PostNetInit() override;
	virtual void OnRep_Controller() override;

#pragma endregion

#pragma region DXPlayerCharacter Components

public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Components")
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Components")
	TObjectPtr<UCameraComponent> Camera;

#pragma endregion

#pragma region Input
private:
	void HandleMoveInput(const FInputActionValue& InValue);
	void HandleLookInput(const FInputActionValue& InValue);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Input")
	TObjectPtr<UInputAction> JumpAction;

#pragma endregion

};
