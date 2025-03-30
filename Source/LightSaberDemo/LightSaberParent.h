#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "LightSaber")
	void IgniteLightsaber();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "LightSaber")
	void DisableLightsaber();

private:
	void TraceSaberCollider();
};
	