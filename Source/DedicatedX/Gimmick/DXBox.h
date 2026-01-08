#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DXBox.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class DEDICATEDX_API ADXBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ADXBox();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UFUNCTION()
	void OnRep_ServerRotationYaw();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTextRenderComponent> TextRender;
	UPROPERTY(ReplicatedUsing = OnRep_ServerRotationYaw)
	float ServerRotationYaw;
	float RotationSpeed;
	float NetUpdatePeriod;
	float AccDeltaSecondSinceReplicated;
};
