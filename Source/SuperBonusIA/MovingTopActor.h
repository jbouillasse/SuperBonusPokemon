#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingTopActor.generated.h"

class AFallingCubeActor;

UCLASS()
class SUPERBONUSIA_API AMovingTopActor : public AActor
{
    GENERATED_BODY()

public:
    AMovingTopActor();

protected:
    virtual void BeginPlay() override;

    // La limite de déplacement pour choisir une position aléatoire
    UPROPERTY(EditAnywhere, Category = "IA Top")
    float MovementRange = 1000.0f;

    // Indique à l'IA quel objet elle doit faire spawner
    UPROPERTY(EditAnywhere, Category = "IA Top")
    TSubclassOf<AFallingCubeActor> CubeClass;

public:
    virtual void Tick(float DeltaTime) override;

    // Le fameux Seek adapté
    FVector Seek(FVector TargetLocation);

    // Fonction pour lâcher le cube et repartir
    void ChooseNewTargetAndSpawnCube();

    UPROPERTY(EditAnywhere, Category = "IA Top")
    float MaxSpeed = 1000.0f; // On double la vitesse de déplacement

    FVector CurrentVelocity;
    FVector CurrentTargetLocation;
    FVector StartLocation; // Position de départ pour calculer la limite de déplacement
    
    UPROPERTY(EditAnywhere, Category = "IA Limites")
    float MinX = -1000.0f;

    UPROPERTY(EditAnywhere, Category = "IA Limites")
    float MaxX = 1000.0f;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
    int32 TotalObjetsLances = 0;
};