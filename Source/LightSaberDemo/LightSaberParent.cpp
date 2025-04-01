#include "LightSaberParent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
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
	FVector End = Start + LaserStartPoint->GetForwardVector() * 70.0f;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 1.0f);

	if (bHit)
	{
		if (!LaserSparklesComponent && LaserSparkles)
		{
			// Calculate the opposite direction of the hit normal for the forward vector
			FVector OppositeDirection = -HitResult.ImpactNormal;

			// Create a rotation from the opposite direction
			FRotator NewRotation = FRotationMatrix::MakeFromX(OppositeDirection).Rotator();

			// Spawn the Niagara component at the hit location with the calculated rotation
			LaserSparklesComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				LaserSparkles,
				HitResult.ImpactPoint,
				NewRotation
			);
		}

		if (LaserSparklesComponent)
		{
			// Update the position and orientation of the Niagara component
			LaserSparklesComponent->SetWorldLocation(HitResult.ImpactPoint);

			// Calculate the opposite direction of the hit normal for the forward vector
			FVector OppositeDirection = -HitResult.ImpactNormal;
			FRotator NewRotation = FRotationMatrix::MakeFromX(OppositeDirection).Rotator();
			LaserSparklesComponent->SetWorldRotation(NewRotation);
		}

		float DecalSize = CalculateDecalSizeBasedOnDistance((HitResult.ImpactPoint - Start).Size());
		FVector DecalSizeParameter = FVector(DecalSize, DecalSize, DecalSize);
		//DrawDecal(FVector(1, 1, 1), HitResult.ImpactPoint, FRotationMatrix::MakeFromX(HitResult.ImpactNormal).Rotator());
		DrawDecal(DecalSizeParameter, HitResult.ImpactPoint, FRotationMatrix::MakeFromX(HitResult.ImpactNormal).Rotator());

		
	}
	else if (LaserSparklesComponent)
	{
		// Destroy the Niagara component if nothing is hit
		LaserSparklesComponent->DestroyComponent();
		LaserSparklesComponent = nullptr;
	}
}

float ALightSaberParent::CalculateDecalSizeBasedOnDistance(float Distance)
{
	// Calculate the distance from start to hit point
	//(HitResult.ImpactPoint - Start).Size();

	// Map the distance to a decal size between 5 and 20
	float MinDistance = 0.0f;
	float MaxDistance = 70.0f; // Maximum length of the line trace
	float MinSize = 15.0f;
	float MaxSize = 3.0f;

	// Calculate the decal size inversely proportional to the distance
	float DecalSize = FMath::GetMappedRangeValueClamped(FVector2D(MinDistance, MaxDistance), FVector2D(MinSize, MaxSize), Distance);
	return DecalSize;
}
