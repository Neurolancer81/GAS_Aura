// Neurolancer81's Games


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();	

	check(AttributeInfo);
	for (auto Info: AttributeInfo->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Info.AttributeGetter).AddLambda(
			[this, Info](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Info);
			});
	}
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	check(AttributeInfo);
	for (auto Info: AttributeInfo->AttributeInformation)
	{
		BroadcastAttributeInfo(Info);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FAuraAttributeInfo& Info) const
{
	//Getting info from the Data Asset
	FAuraAttributeInfo NewInfo = Info;
	// Set the hidden attribute value in the dataset
	NewInfo.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet);
	// Broadcast the values
	AttributeInfoDelegate.Broadcast(NewInfo);
}
