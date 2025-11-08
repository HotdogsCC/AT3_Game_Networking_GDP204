// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapshotTransformComponent.h"

#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USnapshotTransformComponent::USnapshotTransformComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USnapshotTransformComponent::MulticastSnapshot_Implementation(FTransformSnapshot NewSnapshot)
{
	// Ignore if we are authority
	if (GetOwner()->GetLocalRole() == ROLE_Authority) { return; }
	
	LastWriteIndex = TransformSnapshots.GetNextIndex(LastWriteIndex);
	TransformSnapshots[LastWriteIndex] = NewSnapshot;
	++WriteCount;
}


void USnapshotTransformComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* OwningActor = GetOwner();
	double ServerTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	
	// record and send
	if (OwningActor->GetLocalRole() == ENetRole::ROLE_Authority)
	{
		double RealTime = UGameplayStatics::GetRealTimeSeconds(this);
		if (RealTime - LastSendTime > SendInterval)
		{
			FTransformSnapshot NewSnapshot;
			NewSnapshot.ServerTime = ServerTime;
			NewSnapshot.Transform = OwningActor->GetActorTransform();
			MulticastSnapshot(NewSnapshot);
			LastSendTime = ServerTime;
		}
	}
	
	// autonomous proxy - interpolate
	if (OwningActor->GetLocalRole() == ENetRole::ROLE_SimulatedProxy && WriteCount > 0)
	{
		// also the maximum range of values to search within
		uint32 MaxIndexOffset = FMath::Min(TransformSnapshots.Capacity(), WriteCount);

		// we want the snapshot before and after this
		double TargetTime = ServerTime - InterpTime;
		uint32 BlendStartIndex = 0;
		uint32 BlendEndIndex = 0;
		bool BlendOK = false;
		
		// search
		// TODO: optimize as binary search
		for (uint32 IndexOffset = 1; IndexOffset < MaxIndexOffset; ++IndexOffset)
		{
			uint32 CheckIndex = (LastWriteIndex - IndexOffset) % TransformSnapshots.Capacity();
			FTransformSnapshot Snapshot = TransformSnapshots[CheckIndex];

			// accept first snapshot before target
			if (Snapshot.ServerTime < TargetTime)
			{
				BlendStartIndex = CheckIndex;
				BlendOK = true;
				break;
			}
		}

		// failed; cannot find start point to interpolate from; use latest
		if (!BlendOK)
		{
			OwningActor->SetActorTransform(TransformSnapshots[LastWriteIndex].Transform);
			return;
		}
		
		BlendOK = false;
		for (uint32 IndexOffset = 0; IndexOffset < MaxIndexOffset; ++IndexOffset)
		{
			uint32 CheckIndex = (BlendStartIndex + IndexOffset) % TransformSnapshots.Capacity();
			uint32 NextIndex = TransformSnapshots.GetNextIndex(CheckIndex);
			if (TransformSnapshots[NextIndex].ServerTime > TargetTime)
			{
				BlendEndIndex = NextIndex;
				BlendOK = true;
				break;
			}
		}

		// failed to find snapshot after target time
		if (!BlendOK)
		{
			BlendEndIndex = LastWriteIndex;
			BlendOK = true;
		}

		// ready to blend
		if (BlendOK)
		{
			ensure(TransformSnapshots[BlendStartIndex].ServerTime < TransformSnapshots[BlendEndIndex].ServerTime);
			FTransform ApplyTransform;
			float BlendAlpha = FMath::GetRangePct(
				TransformSnapshots[BlendStartIndex].ServerTime,
				TransformSnapshots[BlendEndIndex].ServerTime,
				TargetTime);
			ApplyTransform.Blend(TransformSnapshots[BlendStartIndex].Transform, TransformSnapshots[BlendEndIndex].Transform, BlendAlpha);
			OwningActor->SetActorTransform(ApplyTransform);
		}
	}
}

