// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "US_Minion.generated.h"

class USphereComponent;
class UPawnSensingComponent;

UCLASS()
class MULTIPLAYERCOURSE_API AUS_Minion : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUS_Minion();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Minion AI")
	float PatrolSpeed = 150.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Minion AI")
	float ChaseSpeed = 350.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Minion AI")
	float PatrolRadius = 50000.f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category="Minion AI")
	void SetNextPatrolLocation();

	UFUNCTION(BlueprintCallable, Category="Minion AI")
	void Chase(APawn* Pawn);

	virtual void PostInitializeComponents() override;

	FORCEINLINE UPawnSensingComponent* GetPawnSense() const {return PawnSense;}
	FORCEINLINE USphereComponent* GetCollision() const {return Collision;}

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Minion Perception", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UPawnSensingComponent> PawnSense;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Minion Perception", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY()
	FVector PatrolLocation;
	
	
	UFUNCTION()
	void OnPawnDetected(APawn* Pawn);

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	void PatrolLoop();
};
