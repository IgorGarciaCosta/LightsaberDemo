#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "LightSaberParent.generated.h"

UCLASS()
class LIGHTSABERDEMO_API ALightSaberParent : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightSaberParent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSaberIgnited = false;

	// Main static mesh for the lightsaber
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightSaber")
	UStaticMeshComponent* LightSaberMain;

	// Scene component for the start of the laser
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightSaber")
	USceneComponent* LaserStartPoint;

	// Niagara system for the laser sparkles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* LaserSparkles;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "LightSaber")
	void IgniteLightsaber();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "LightSaber")
	void DisableLightsaber();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "LightSaber")
	void DrawDecal(FVector DecalSize, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "LightSaber")
	void PlayImpactSound();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "LightSaber")
	void StopImpactSound();


private:
	void TraceSaberCollider();
	// Handle to the Niagara component

	bool bCanPlayImpactSound = true;
	UPROPERTY()
	UNiagaraComponent* LaserSparklesComponent;

	UFUNCTION()
	float CalculateDecalSizeBasedOnDistance(float Distance);
};
	