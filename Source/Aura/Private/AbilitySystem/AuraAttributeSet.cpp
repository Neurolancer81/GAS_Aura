// Neurolancer81's Games


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMana(20.f);
	InitMaxHealth(100.f);
	InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data,
	FEffectProperties& EffectProperties) const
{
	EffectProperties.ContextHandle =  Data.EffectSpec.GetContext();
	

	EffectProperties.SourceProperties->AbilitySystemComponent = EffectProperties.ContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (EffectProperties.SourceProperties->AbilitySystemComponent &&
		EffectProperties.SourceProperties->AbilitySystemComponent->AbilityActorInfo.IsValid() &&
		EffectProperties.SourceProperties->AbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.SourceProperties->AvatarActor = EffectProperties.SourceProperties->AbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();
		EffectProperties.SourceProperties->Controller = EffectProperties.SourceProperties->AbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
		if (EffectProperties.SourceProperties->Controller && !EffectProperties.SourceProperties->AvatarActor)
		{
			if (const APawn* Pawn = Cast<APawn>(EffectProperties.SourceProperties->AvatarActor))
			{
				EffectProperties.SourceProperties->Controller = Pawn->GetController();
			}
		}
		if (EffectProperties.SourceProperties->Controller)
		{
			EffectProperties.SourceProperties->Character = EffectProperties.SourceProperties->Controller->GetCharacter();
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		EffectProperties.TargetProperties->AvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		EffectProperties.TargetProperties->Controller = Data.Target.AbilityActorInfo->PlayerController.Get();
		EffectProperties.TargetProperties->Character = Data.Target.AbilityActorInfo->PlayerController->GetCharacter();
		EffectProperties.TargetProperties->AbilitySystemComponent =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectProperties.TargetProperties->AvatarActor);
	}
	
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties EffectProperties;
	SetEffectProperties(Data, EffectProperties);

}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}


