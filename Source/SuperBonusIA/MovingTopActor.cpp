#include "MovingTopActor.h"
#include "FallingCubeActor.h"
#include "Math/UnrealMathUtility.h"

AMovingTopActor::AMovingTopActor()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentVelocity = FVector::ZeroVector;
}

void AMovingTopActor::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetActorLocation(); // On mémorise le point de départ
    ChooseNewTargetAndSpawnCube();      // On lance la machine
}

void AMovingTopActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector MyLocation = GetActorLocation();

    // Si on est presque arrivé à la destination (ex: à moins de 15 unités)
    if (FVector::DistXY(MyLocation, CurrentTargetLocation) < 15.0f)
    {
        // On lâche un objet et on choisit une nouvelle destination
        ChooseNewTargetAndSpawnCube();
    }
    else
    {
        // Sinon, on continue d'avancer vers la cible avec Seek
        FVector SteeringForce = Seek(CurrentTargetLocation);
        CurrentVelocity += SteeringForce * DeltaTime;
        SetActorLocation(MyLocation + (CurrentVelocity * DeltaTime));
    }
}

FVector AMovingTopActor::Seek(FVector TargetLocation)
{
    FVector MyLocation = GetActorLocation();

    // On bloque l'axe Z (hauteur) et Y (profondeur) pour rester sur l'axe X
    TargetLocation.Z = MyLocation.Z;
    TargetLocation.Y = MyLocation.Y;

    FVector DesiredVelocity = TargetLocation - MyLocation;

    if (DesiredVelocity.IsNearlyZero()) return FVector::ZeroVector;

    DesiredVelocity.Normalize();
    DesiredVelocity *= MaxSpeed;

    FVector Steering = DesiredVelocity - CurrentVelocity;
    return Steering.GetClampedToMaxSize(MaxSpeed);
}

void AMovingTopActor::ChooseNewTargetAndSpawnCube()
{
    // 1. Spawner le cube
    if (CubeClass != nullptr)
    {
        AFallingCubeActor* NouveauCube = GetWorld()->SpawnActor<AFallingCubeActor>(CubeClass, GetActorLocation(), FRotator::ZeroRotator);
        
        // AJOUT : Si le cube a bien été créé, on augmente le compteur de l'IA du haut
        if (NouveauCube)
        {
            TotalObjetsLances++;
        }
    }

    // 2. On ajoute une marge de sécurité (ex: 100 unités) pour ne jamais lâcher de cube tout au bord
    float Margin = 100.0f;
    float SafeMinX = MinX + Margin;
    float SafeMaxX = MaxX - Margin;

    // 3. Choisir une cible aléatoire bien sécurisée à l'intérieur de l'écran
    CurrentTargetLocation = GetActorLocation();
    CurrentTargetLocation.X = FMath::RandRange(SafeMinX, SafeMaxX);
}