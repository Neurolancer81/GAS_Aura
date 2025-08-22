// Neurolancer81's Games

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;

	/*End Combat Interface*/

	UPROPERTY(BlueprintAssignable, Category="Aura")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="Aura")
	FOnAttributeChangedSignature OnMaxHealthChanged;


protected:
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=" Character Class Defaults")
	int32 Level {1};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health Bar")
	TObjectPtr<UWidgetComponent> HealthBar;

private:

	void InitHealthBar();

	
	
};
