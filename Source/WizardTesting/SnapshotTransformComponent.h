// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SnapshotTransformComponent.generated.h"


USTRUCT()
struct FTransformSnapshot
{
	GENERATED_BODY()

public:
	/** Transform data of root component at time of capture. */
	UPROPERTY()
	FTransform Transform;
	/** The time at which the transform was captured by the server. */
	UPROPERTY()
	double ServerTime;
};

UCLASS( ClassGroup=(Custom), NotBlueprintable, meta=(BlueprintSpawnableComponent) )
class WIZARDTESTING_API USnapshotTransformComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	/** For client-only use. Records into a rolling buffer of snapshots. */
	TCircularBuffer<FTransformSnapshot> TransformSnapshots = TCircularBuffer<FTransformSnapshot>(64);
	/** For client-only use. Records the last index written to in the circular buffer. */
	uint32 LastWriteIndex = 0;
	/** For client-only use. Records the overall number of snapshots received. */
	uint32 WriteCount = 0;
	
	/** For (listen) server-use only. Records the last time at which a snapshot was sent. */
	double LastSendTime = -1;

public:	
	USnapshotTransformComponent();

	/**  */
	UPROPERTY(Category = "SnapshotTransform", EditAnywhere, BlueprintReadOnly)
	double SendInterval = 0.01;

	UPROPERTY(Category = "SnapshotTransform", EditAnywhere, BlueprintReadOnly)
	double InterpTime = 1.0;
	
private:	
	//
	// AActor
	//

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION(Category = "SnapshotTransform", NetMulticast, Unreliable)
	void MulticastSnapshot(FTransformSnapshot NewSnapshot);
};