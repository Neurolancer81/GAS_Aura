// Neurolancer81's Games


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// Get the hit result for mouse hovering on an enemy 
	CursorTrace();

	// Adding Auto run behaviour
	if (bAutoRunning) AutoRun();
	
}

void AAuraPlayerController::AutoRun()
{
	if (APawn* ControlledPawn = GetPawn(); IsValid(ControlledPawn))
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);

		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
		ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(Direction.GetSafeNormal());

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();

		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Set up the input mapping context
	check(AuraContext);
	UEnhancedInputLocalPlayerSubsystem* SubSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(AuraContext, 0);
	}

	// Set up cursor and input modes
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this,
		&ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAuraAbilitySystemComponent()
{
	if (!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::Move(const FInputActionValue& ActionValue)
{
	const FVector2D InputAxisVector = ActionValue.Get<FVector2D>();
	const FRotator Rotation = FRotator(0.0, GetControlRotation().Yaw, 0.0);

	const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetAuraAbilitySystemComponent())
		{
			GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	// Tell the ASC that the key has been released
	if (GetAuraAbilitySystemComponent()) GetAuraAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
	
	if (!bTargeting && !bShiftDown)
	{
		if (const APawn* ControlledPawn = GetPawn<APawn>(); FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this,
				ControlledPawn->GetActorLocation(),
				CachedDestination);
			if (NavigationPath)
			{
				Spline->ClearSplinePoints();
				for (const FVector& PathPoint: NavigationPath->PathPoints)
				{
					Spline->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World);
					// DrawDebugSphere(GetWorld(), PathPoint, 5.0f, 12, FColor::Green, false, 5.f);					
				}
				if (NavigationPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavigationPath->PathPoints[NavigationPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
				
			}			
		}
		
		bTargeting = false;
		FollowTime = 0.0f;
	}
		
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Green, *InputTag.ToString());
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetAuraAbilitySystemComponent())
		{
			GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	
	if (bTargeting || bShiftDown)
	{
		if (GetAuraAbilitySystemComponent())
		{
			GetAuraAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		}
	}

	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn<APawn>())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}	
}

void AAuraPlayerController::CursorTrace()
{
	
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();
	}
	
}


