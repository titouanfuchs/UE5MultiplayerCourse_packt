// Fill out your copyright notice in the Description page of Project Settings.


#include "US_PlayerState.h"

#include "US_Character.h"
#include "Net/UnrealNetwork.h"

void AUS_PlayerState::OnRep_Xp(int32 OldValue) const
{
	OnXPChanged.Broadcast(Xp);
}

void AUS_PlayerState::OnRep_CharacterLevelUp(int32 OldValue) const
{
	OnCharacterLevelUp.Broadcast(CharacterLevel);
}

void AUS_PlayerState::AddXp(int32 Value)
{
	Xp += Value;
	OnXPChanged.Broadcast(Xp);

	GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Yellow, FString::Printf(TEXT("Total Xp: %d"), Value));

	if (const auto Character = Cast<AUS_Character>(GetPawn()))
	{
		GEngine->AddOnScreenDebugMessage(3, 5.f, FColor::Red, TEXT("Level Up!"));

		CharacterLevel++;
		Character->UpdateCharacterStats(CharacterLevel);
		OnCharacterLevelUp.Broadcast(CharacterLevel);
	}
}

void AUS_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AUS_PlayerState, Xp, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AUS_PlayerState, CharacterLevel, COND_OwnerOnly);
}
