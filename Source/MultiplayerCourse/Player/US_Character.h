// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "US_Character.generated.h"

struct FInputActionValue;
class UInputAction;
struct FUS_CharacterStats;

UCLASS()
class MULTIPLAYERCOURSE_API AUS_Character : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess = true))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = true))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SprintAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Data", meta=(AllowPrivateAccess = true))
	class UDataTable* CharacterDataTable;

	UPROPERTY()
	AActor* InteractableActor;
	
	struct FUS_CharacterStats* CharacterStats;
	
public:
	// Sets default values for this character's properties
	AUS_Character();

	void UpdateCharacterStats(int32 CharacterLevel);

	FORCEINLINE FUS_CharacterStats* GetCharacterStats() const {return CharacterStats;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move (const FInputActionValue& Value);
	void Look (const FInputActionValue& Value);
	
	void SprintStart (const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void SprintStart_Server();
	
	void SprintEnd (const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void SprintEnd_Server();

	void Interact (const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void Interact_Server();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE USpringArmComponent* GetCameraBoom() const {return CameraBoom;}
	FORCEINLINE UCameraComponent* GetCamera() const {return Camera;}
};
