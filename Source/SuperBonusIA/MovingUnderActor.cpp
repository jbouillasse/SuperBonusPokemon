#include "MovingUnderActor.h"
#include "FallingCubeActor.h"
#include "Kismet/GameplayStatics.h"
#include "MovingTopActor.h"


AMovingUnderActor::AMovingUnderActor()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentVelocity = FVector::ZeroVector; // On démarre à l'arrêt
}

void AMovingUnderActor::BeginPlay()
{
    Super::BeginPlay();
}

void AMovingUnderActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 1. On cherche d'abord s'il y a un cube qui tombe
    TArray<AActor*> FoundCubes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFallingCubeActor::StaticClass(), FoundCubes);

    AActor* BestCube = nullptr;
    float LowestZ = 999999.0f;

    for (AActor* Cube : FoundCubes)
    {
        if (Cube->GetActorLocation().Z < LowestZ)
        {
            LowestZ = Cube->GetActorLocation().Z;
            BestCube = Cube;
        }
    }

    FVector TargetLocation;
    bool bHasTarget = false;

    // 2. Si on a un cube, c'est la priority absolue
    if (BestCube)
    {
        TargetLocation = BestCube->GetActorLocation();
        bHasTarget = true;
    }
    // ON DÉSACTIVE LE SUIVI DE L'IA DU HAUT
    /* 
    else
    {
        AActor* TopAI = UGameplayStatics::GetActorOfClass(GetWorld(), AMovingTopActor::StaticClass());
        if (TopAI)
        {
            TargetLocation = TopAI->GetActorLocation();
            bHasTarget = true;
        }
    }
    */
    

    // 4. Mouvement fluide et anti-tremblement
    if (bHasTarget)
    {
        // On vérifie à quelle distance horizontale on est de la cible
        float DistanceX = FMath::Abs(GetActorLocation().X - TargetLocation.X);

        // Si on est à moins de 20 unités (pile en dessous), on arrête le moteur et on freine
        if (DistanceX < 20.0f) 
        {
            CurrentVelocity = FMath::VInterpTo(CurrentVelocity, FVector::ZeroVector, DeltaTime, 15.0f);
        }
        else
        {
            // Sinon, on continue de se diriger vers la cible normalement
            FVector SteeringForce = Seek(TargetLocation);
            CurrentVelocity += (SteeringForce * 3.0f) * DeltaTime;
        }
    }
    else 
    {
        CurrentVelocity = FMath::VInterpTo(CurrentVelocity, FVector::ZeroVector, DeltaTime, 5.0f);
    }

    // 5. On applique la position finale avec tes limites d'écran
    FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
    NewLocation.X = FMath::Clamp(NewLocation.X, MinX, MaxX);
    SetActorLocation(NewLocation);
    
    // 6. Rotation fluide (naturelle) selon la direction
    if (FMath::Abs(CurrentVelocity.X) > 1.0f)
    {
        // On prépare la rotation cible
        FRotator TargetRotation;
        
        if (CurrentVelocity.X > 0.0f)
        {
            // Cible vers la DROITE
            TargetRotation = FRotator(0.f, 0.f, 0.f); 
        }
        else
        {
            // Cible vers la GAUCHE
            TargetRotation = FRotator(0.f, 180.f, 0.f); 
        }

        // Magie d'Unreal : on interpole (adoucit) la rotation actuelle vers la cible.
        FRotator SmoothRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 12.0f);
        
        // On applique la rotation adoucie
        SetActorRotation(SmoothRotation); 
    }
    
    // On met à jour le float de vitesse pour l'AnimBP
    Vitesse = CurrentVelocity.Size();
}

FVector AMovingUnderActor::Seek(FVector TargetLocation)
{
    FVector MyLocation = GetActorLocation();

    // LE SECRET DE L'AXE HORIZONTAL (Axe X) :
    TargetLocation.Z = MyLocation.Z; 
    TargetLocation.Y = MyLocation.Y; 

    // 1. Calcul de la vélocité désirée
    FVector DesiredVelocity = TargetLocation - MyLocation;
    
    // Petite sécu : si on est pile dessus, on bouge plus
    if (DesiredVelocity.IsNearlyZero()) return FVector::ZeroVector;

    // 2. On normalise
    DesiredVelocity.Normalize();

    // 3. On multiplie par la vitesse max
    DesiredVelocity *= MaxSpeed;

    // 4. Force de direction
    FVector Steering = DesiredVelocity - CurrentVelocity;
    
    // 5. On limite la force
    return Steering.GetClampedToMaxSize(MaxSpeed);
}

// AJOUT : La fonction de collision pour attraper le cube et compter les points
void AMovingUnderActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    // On vérifie si l'acteur touché est bien un FallingCubeActor
    AFallingCubeActor* CubeTouche = Cast<AFallingCubeActor>(OtherActor);
    
    if (CubeTouche)
    {
        // On détruit le cube
        CubeTouche->Destroy();
        
        // On incrémente notre compteur d'objets attrapés
        TotalObjetsAttrapes++;
    }
}