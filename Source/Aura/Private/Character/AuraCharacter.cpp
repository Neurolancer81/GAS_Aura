// Neurolancer81's Games


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"


AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor for the server
	InitAbilityActorInfo();

	// Add any starting abilities
	AddCharacterAbilities();	
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor for the client
	InitAbilityActorInfo();
	
}

int32 AAuraCharacter::GetPlayerLevel()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState)
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState)
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	
	AttributeSet = AuraPlayerState->GetAttributeSet();

	AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(GetController());
	if (!PlayerController) return;
	
	AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD());
	if (!AuraHUD) return;
	
	AuraHUD->InitOverlay(FWidgetControllerParams(PlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet));

	InitializeDefaultAttributes();
	
}
