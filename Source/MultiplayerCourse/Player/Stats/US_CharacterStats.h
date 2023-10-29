#pragma once

#include "US_CharacterStats.generated.h"

USTRUCT(BlueprintType)
struct MULTIPLAYERCOURSE_API : FUS_CharacterStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float WalkSpeed = 200.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SprintSpeed = 400.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DamageMultipler = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NextLevelXp = 10.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StealthMultipler = 1.f;
};
