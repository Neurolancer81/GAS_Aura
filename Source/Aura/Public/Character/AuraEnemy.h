// Copyright NeuroGames Inc.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();

	/* Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* End enemy interface*/

	/** Combat Interface **/
	virtual int32 GetPlayerLevel() override;

	/* End Combat Interface*/

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults", meta=(AllowPrivateAccess=true))
	int32 Level = 1;
	
};
