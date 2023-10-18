// Fill out your copyright notice in the Description page of Project Settings.


#include "US_Gamemode.h"

#include "MultiplayerCourse/GameState/US_GameState.h"

AUS_Gamemode::AUS_Gamemode()
{
	GameStateClass = AUS_GameState::StaticClass();
}
