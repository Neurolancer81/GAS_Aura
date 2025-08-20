// Neurolancer81's Games


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/Ability/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			FGameplayTagContainer& DynamicTags = AbilitySpec.GetDynamicSpecSourceTags();
			DynamicTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);		
		}		
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	// Check if the tag is valid
	if (!InputTag.IsValid()) return;

	// Check if this ASC has activatable abilities with this tag
	for (FGameplayAbilitySpec& AbilitySpec: GetActivatableAbilities())
	{
		FGameplayTagContainer& DynamicTags = AbilitySpec.GetDynamicSpecSourceTags();
		if (DynamicTags.HasTagExact(InputTag))
		{
			// Tell the ability that we are pressing it's input function
			AbilitySpecInputPressed(AbilitySpec);	
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	// Check if the tag is valid
	if (!InputTag.IsValid()) return;

	// Check if this ASC has activatable abilities with this tag
	for (FGameplayAbilitySpec& AbilitySpec: GetActivatableAbilities())
	{
		FGameplayTagContainer& DynamicTags = AbilitySpec.GetDynamicSpecSourceTags();
		if (DynamicTags.HasTagExact(InputTag))
		{
			// Tell the ability that we are pressing it's input function
			AbilitySpecInputReleased(AbilitySpec);			
		}
	}	
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::ClientEffectApplied);
}
