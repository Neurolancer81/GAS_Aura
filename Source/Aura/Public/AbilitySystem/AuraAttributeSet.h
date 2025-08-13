// Neurolancer81's Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
USTRUCT()
struct FEffectPropertiesBase
{
	GENERATED_BODY()
 
	UPROPERTY(Transient)
	UAbilitySystemComponent* AbilitySystemComponent;
 
	UPROPERTY(Transient)
	AActor* AvatarActor;
 
	UPROPERTY(Transient)
	AController* Controller;
 
	UPROPERTY(Transient)
	ACharacter* Character;
};

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
 
	FEffectProperties()
	{
		SourceProperties = MakeShared<FEffectPropertiesBase>();
		TargetProperties = MakeShared<FEffectPropertiesBase>();
	}
    
	FGameplayEffectContextHandle ContextHandle;
    
	TSharedPtr<FEffectPropertiesBase> SourceProperties;
 
	TSharedPtr<FEffectPropertiesBase> TargetProperties;
    
};

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Health
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="VitalAttributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	// Mana
	UPROPERTY(ReplicatedUsing=OnRep_Mana, BlueprintReadOnly, Category="VitalAttributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	// MaxHealth
	UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, BlueprintReadOnly, Category="VitalAttributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	// MaxMana
	UPROPERTY(ReplicatedUsing=OnRep_MaxMana, BlueprintReadOnly, Category="VitalAttributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

private:
	
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const;
	
	
};
