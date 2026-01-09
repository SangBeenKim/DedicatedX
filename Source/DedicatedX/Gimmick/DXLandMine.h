#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DXLandMine.generated.h"

class USceneComponent;
class UBoxComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;

UCLASS()
class DEDICATEDX_API ADXLandMine : public AActor
{
	GENERATED_BODY()
	
public:	
	ADXLandMine();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnLandMineBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCSpawnEffect();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> SceneRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UBoxComponent> BoxCollision;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UParticleSystemComponent> Particle;
};
