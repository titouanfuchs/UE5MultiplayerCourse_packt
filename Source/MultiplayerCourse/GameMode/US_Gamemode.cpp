// Fill out your copyright notice in the Description page of Project Settings.


#include "US_Gamemode.h"
#include "UObject/ConstructorHelpers.h"

#include "MultiplayerCourse/GameState/US_GameState.h"
#include "MultiplayerCourse/Player/US_PlayerController.h"
#include "MultiplayerCourse/Player/US_PlayerState.h"

AUS_Gamemode::AUS_Gamemode()
{
	GameStateClass = AUS_GameState::StaticClass();
	PlayerStateClass = AUS_PlayerState::StaticClass();
	PlayerControllerClass = AUS_PlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn>	PlayerPawnBPClass(TEXT("/Game/Blueprints/Player/BP_Character"));

	if (PlayerPawnBPClass.Class != nullptr) DefaultPawnClass = PlayerPawnBPClass.Class;
}
