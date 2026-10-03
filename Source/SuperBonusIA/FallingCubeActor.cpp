// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingCubeActor.h"

// Sets default values
AFallingCubeActor::AFallingCubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFallingCubeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFallingCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

