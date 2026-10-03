#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingUnderActor.generated.h"

// On prévient le compilo que cette classe existe sans l'inclure direct
class AFallingCubeActor; 

UCLASS()
class SUPERBONUSIA_API AMovingUnderActor : public AActor
{
	GENERATED_BODY()

public:
	AMovingUnderActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// La formule de ton prof adaptée pour Unreal
	FVector Seek(FVector TargetLocation);

	// Vitesse max modifiable direct dans l'éditeur Unreal
	UPROPERTY(EditAnywhere, Category = "IA")
	float MaxSpeed = 300.0f;

	// La vitesse actuelle de ton IA
	FVector CurrentVelocity;

	// Un pointeur vers le cube qui tombe
	UPROPERTY()
	AFallingCubeActor* TargetCube;
	
	// Limite gauche de l'écran
	UPROPERTY(EditAnywhere, Category = "IA Limites")
	float MinX = -1000.0f;

	// Limite droite de l'écran
	UPROPERTY(EditAnywhere, Category = "IA Limites")
	float MaxX = 1000.0f;
	
	// La vitesse actuelle convertie en float pour l'animation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	float Vitesse = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 TotalObjetsAttrapes = 0;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};