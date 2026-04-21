// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Character/SGCharacter.h"

#include "SG/AbilitySystem/SGAbilitySystemComponent.h"
#include "SG/Camera/SGCameraComponent.h"
#include "SG/Character/SGPawnExtensionComponent.h"

ASGCharacter::ASGCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	NetCullDistanceSquared = 900000000.0f;
	
	PawnExtComp = CreateDefaultSubobject<USGPawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	CameraComp = CreateDefaultSubobject<USGCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));

	// Dedicated Server에서도 애니메이션 업데이트 (AnimNotify 발동을 위해 필요)
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void ASGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASGCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	// 클라이언트에서 호출된다.
	PawnExtComp->HandleControllerChanged();
}

void ASGCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// 클라이언트에서 호출된다.
	PawnExtComp->HandlePlayerStateReplicated();
}

void ASGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Pawn이 Possess로서, Controller와 PlayerState 정보 접근이 가능한 상태가 되었다.
	PawnExtComp->SetupPlayerInputComponent();
}

void ASGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// 서버에서 호출된다.
	PawnExtComp->HandleControllerChanged();
}

UAbilitySystemComponent* ASGCharacter::GetAbilitySystemComponent() const
{
	return PawnExtComp->GetSGAbilitySystemComponent();
}

