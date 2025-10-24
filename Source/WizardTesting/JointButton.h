// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JointButton.generated.h"

class AOpeningDoor;

UCLASS()
class WIZARDTESTING_API AJointButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJointButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	bool GetIsActive() const;

	UFUNCTION()
	void OnButtonHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* CubeStaticMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ButtonStaticMesh;

	UPROPERTY(EditAnywhere)
	UMaterial* OnMaterial = nullptr;

	UPROPERTY(EditAnywhere)
	UMaterial* OffMaterial = nullptr;

	//how long the button is active for
	UPROPERTY(EditAnywhere)
	float ActiveTime = 10.0f;

private:
	//tell unreal what to replicate
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	//reference to the other button
	UPROPERTY()
	AJointButton* OtherButton = nullptr;

	//reference to the opening door
	UPROPERTY()
	AOpeningDoor* OpeningDoor = nullptr;
	
	//countdown for how long this button is active for
	UPROPERTY()
	float ElapsedActiveTime = 0.0f;

	//is this active?
	UPROPERTY(ReplicatedUsing = OnRepIsActive)
	bool bIsActive = false;

	//handles active ticking
	UFUNCTION()
	void TickButtonActive(float DeltaTime);

	//when this button is On
	UFUNCTION()
	void OnButtonOn();

	//when this button is Off
	UFUNCTION()
	void OnButtonOff();

	//change material of button
	UFUNCTION()
	void SetButtonMaterial(bool bIsOn);

	UFUNCTION()
	void OnRepIsActive();
};
