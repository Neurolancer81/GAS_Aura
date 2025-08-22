// Neurolancer81's Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility, public ICombatInterface
{
	GENERATED_BODY()

protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ProjectileProperties")
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ProjectileProperties")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
};
