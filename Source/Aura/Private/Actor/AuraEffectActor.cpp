// Copyright NeuroGames Inc.


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"


AAuraEffectActor::AAuraEffectActor()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(Mesh);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");	
	Sphere->SetupAttachment(GetRootComponent());
	

}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		// TODO: Change this to apply a gameplay effect
		const UAuraAttributeSet* AuraAttributeSet =
			Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		UAuraAttributeSet* MutAuraAttributeSet = const_cast<UAuraAttributeSet*>(AuraAttributeSet);
		MutAuraAttributeSet->SetHealth(AuraAttributeSet->GetHealth() + 25.f);
		MutAuraAttributeSet->SetMana(AuraAttributeSet->GetMana() - 25.f);
		Destroy();
	}
}

void AAuraEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::EndOverlap);
	
}


