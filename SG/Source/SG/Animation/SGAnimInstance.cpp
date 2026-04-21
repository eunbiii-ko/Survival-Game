// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Animation/SGAnimInstance.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "KismetAnimationLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SG/SGLogChannels.h"

void USGAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner()))
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerCharacter))
		{
			InitializeWithAbilitySystem(ASC);
		}

		OwnerMovementComp = OwnerCharacter->GetCharacterMovement();
	}
}

void USGAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter)
	{
		Velocity = OwnerCharacter->GetVelocity();
		GroundSpeed = Velocity.Size2D();
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, OwnerCharacter->GetActorRotation());
		Rotation = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(Velocity), OwnerCharacter->GetActorRotation()).Yaw;
		bShouldMove = (GroundSpeed > 0.f);
		//UE_LOG(LogSG, Display, TEXT("Speed: %f in %d"), GroundSpeed, OwnerCharacter->HasAuthority());
	}
	
	if (OwnerMovementComp)
	{
		bIsFalling = OwnerMovementComp->IsFalling();
		bIsCrouching = OwnerMovementComp->IsCrouching();
		bShouldMove = true;
	}

}

void USGAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	// ASC 내부 관리하는 GameplayTag와 Animation의 멤버 Property와 Delegate를 연결하여
	// 값 변화에 대한 반영을 진행한다. 
	GameplayTagPropertyMap.Initialize(this, ASC);
}
