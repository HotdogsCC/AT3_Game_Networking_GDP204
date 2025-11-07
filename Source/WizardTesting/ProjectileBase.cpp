// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/PointLightComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set it to replicate (so it works online)
	bReplicates = true;
    AActor::SetReplicateMovement(true);

	//mesh component set up
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(MeshComponent);
	
	//adds a light as a sub object
	LightComponent = CreateDefaultSubobject<UPointLightComponent>("Light");
	LightComponent->SetupAttachment(RootComponent);

	MeshComponent->SetGenerateOverlapEvents(false);

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlap);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//are we a client?
	if(!HasAuthority())
	{
		//dont do anything
		return;
	}

	//decrement alive time
	DespawnTime -= DeltaTime;
		
	//has the projectile run out of alive time?
	if(DespawnTime <= 0.0f)
	{
		//destroy this projectile
		Destroy();
		return;
	}

	//get the distance from its target
	float CurDistanceToTarget = FVector::Dist(TargetLocation, PreviousLocation);

	//its current percentage of travel, from 0-1 (or 1-inf past target)
	float TravelCompletion = (InitDistance - CurDistanceToTarget) / InitDistance;

	//direction vector to add on current position
	FVector TravelVector = TargetDirection * MoveSpeed * DeltaTime;

	//travel toward the target
	SetActorLocation(PreviousLocation + TravelVector);

	//save previous location
	PreviousLocation = GetActorLocation();

	//have we reached the target?
	if(bTargetReached)
	{
		//move the projectile along the curve
		SetActorLocation(GetActorLocation() + (CurveDirection*GetCurveAdditive(2-TravelCompletion)));
	}
	else
	{
		//will we reach the target this frame?
		if(TravelVector.SquaredLength() > CurDistanceToTarget * CurDistanceToTarget)
		{
			//set as target reached
			bTargetReached = true;
		}

		//move the projectile along the curve
		SetActorLocation(GetActorLocation() + (CurveDirection*GetCurveAdditive(TravelCompletion)));
	}

}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void AProjectileBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//is this the client?
	if (!HasAuthority())
	{
		//do nothing
		return;
	}

	//is this us?
	if (OtherActor == WizardOwner)
	{
		//do nothing
		return;
	}

	//create particles
	GetWorld()->SpawnActor<AActor>(ExplodeParticles, GetActorLocation(), GetActorRotation());

	//destroy this
	Destroy();
}

void AProjectileBase::StartDetectingCollisions() const
{
	//enable collisions on the mesh
	MeshComponent->SetGenerateOverlapEvents(true);
}

void AProjectileBase::SetTarget(const FVector& InTargetLocation)
{
	//set the target
	TargetLocation = InTargetLocation;

	//save where we are now
	PreviousLocation = GetActorLocation();
	
	//set the initial distance from target
	const FVector CurrentToTarget = TargetLocation - GetActorLocation();
	InitDistance = CurrentToTarget.Length();

	//set target direction
	TargetDirection = CurrentToTarget.GetSafeNormal();

	//set initial curve direction
	FRotator DirectionRotator = CurrentToTarget.Rotation();
	DirectionRotator.Add(0, FMath::FRandRange(0.0f, 360.0f), 0);
	CurveDirection = DirectionRotator.Vector();
	CurveDirection.Normalize();

	//init target as not reached
	bTargetReached = false;
	
}

void AProjectileBase::SetWizardOwner(AActor* Wizard)
{
	//set the wizard owner
	WizardOwner = Wizard;
}

float AProjectileBase::GetTimeBetweenShots() const
{
	//return the time between shots
	return TimeBetweenShots;
}

float AProjectileBase::GetCurveAdditive(float Input) const
{
	//return a point on the parabola -(2x-1)^2 + 1, then multiplies by curviness
	return ((((Input * 2) - 1) * ((Input * 2) - 1) * -1) + 1) * Curviness;

	//when input = 0, output = 0
	//when input = 0.5, output = 1
	//when input = 1, output = 0
}



