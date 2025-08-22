// Neurolancer81's Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "AuraGameMode.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AURA_API AAuraGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character class defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	
};
