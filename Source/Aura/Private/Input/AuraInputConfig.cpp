// Neurolancer81's Games


#include "Input/AuraInputConfig.h"

UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
                                                                   const bool bLogNotFound) const
{
	for (const FAuraInputAction& AuraInput : AbilityInputActions)
	{
		if (AuraInput.InputTag == InputTag)
		{
			return AuraInput.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"),
			*InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}
