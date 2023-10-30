// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "US_Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UUS_Interactable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPLAYERCOURSE_API IUS_Interactable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction", meta=(DisplayName="Interact"))
	void Interact(class AUS_Character* CharacterInstigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction", meta=(DisplayName="Can Interact"))
	bool CanInteract(AUS_Character* CharacterInstigator) const;
};
