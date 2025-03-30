#include "LightSaberParent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
ALightSaberParent::ALightSaberParent()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the LightSaberMain component and set it as the root
	LightSaberMain = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightSaberMain"));
	RootComponent = LightSaberMain;

	// Create the LaserStartPoint component and attach it to the LightSaberMain
	LaserStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("LaserStartPoint"));
	LaserStartPoint->SetupAttachment(LightSaberMain);
}

// Called when the game starts or when spawned
void ALightSaberParent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALightSaberParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSaberIgnited)
	{
		TraceSaberCollider();
	}
}

void ALightSaberParent::TraceSaberCollider()
{
	if (!LaserStartPoint) return;

	FHitResult HitResult;
	FVector Start = LaserStartPoint->GetComponentLocation();
	FVector End = Start + LaserStartPoint->GetForwardVector() * 100.0f; // 100 units in the forward direction

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore self to prevent false positives

	// Perform the line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	// Draw a debug line for visualization
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);

	// Log the hit result if something is hit
	if (bHit)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Hit: %s"), *HitResult.GetActor()->GetName()));
	}
}
