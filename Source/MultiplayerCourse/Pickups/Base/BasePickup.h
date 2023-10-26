// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

UCLASS()
class MULTIPLAYERCOURSE_API ABasePickup : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<class USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> Mesh;
	
public:
	// Sets default values for this actor's properties
	ABasePickup();

	FORCEINLINE USphereComponent* GetSphereCollision() const {return SphereCollision;}
	FORCEINLINE UStaticMeshComponent* GetMesh() const {return Mesh;}
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Pickup", meta=(DisplayName="Pickup"))
	void Pickup(class AUS_Character* OwningCharacter);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
