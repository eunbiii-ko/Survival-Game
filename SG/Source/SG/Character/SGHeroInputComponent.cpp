// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Character/SGHeroInputComponent.h"

#include "SG/SGGameplayTags.h"
#include "SG/Player/SGPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "SGPawnExtensionComponent.h"
#include "SG/Input/SGEnhancedInputComponent.h"
#include "SG/Input/SGMappableConfigPair.h"
#include "EnhancedInputSubsystemInterface.h"
#include "PlayerMappableInputConfig.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Framework/Text/ShapedTextCache.h"
#include "SG/SGLogChannels.h"
#include "SG/AbilitySystem/SGAbilitySystemComponent.h"
#include "SG/Cosmetics/SGControllerComp_CharacterParts.h"
#include "AbilitySystemBlueprintLibrary.h"

const FName USGHeroInputComponent::NAME_ActorFeatureName("HeroInput");
const FName USGHeroInputComponent::NAME_BindInputsNow("BindInputsNow");

USGHeroInputComponent::USGHeroInputComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void USGHeroInputComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	Super::OnActorInitStateChanged(Params);
}

bool USGHeroInputComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	return Super::CanChangeInitState(Manager, CurrentState, DesiredState);
}

void USGHeroInputComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	Super::HandleChangeInitState(Manager, CurrentState, DesiredState);

	APawn* Pawn = GetPawn<APawn>();
	
	// DataAvailable -> DataInitialized
	if (CurrentState == SGGameplayTags::InitState_DataAvailable && DesiredState == SGGameplayTags::InitState_DataInitialized)
	{
		// Input에 대한 핸들링
		if (ASGPlayerController* LccPC = GetController<ASGPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
}

void USGHeroInputComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	// LocalPlayer를 가져오기 위해
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	// EnhancedInputLocalPlayerSubsystem을 가져오기 위해
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	// EnhancedInputLocalPlayerSubsystem의 MappingContext를 비워준다.
	Subsystem->ClearAllMappings();

	// PawnExtComp -> PawnData -> InputConfig 존재 유무를 판단한다.
	if (const USGPawnExtensionComponent* PawnExtComp = USGPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const USGPawnData* PawnData = PawnExtComp->GetPawnData<USGPawnData>())
		{
			if (const USGInputConfig* InputConfig = PawnData->InputConfig)
			{
				// 멤버변수(DefaultInputConfigs)로 갖고있는 IMC를 순회하며, EnhancedInputLocalPlayerSubsystem에 추가한다.
				for (const FSGMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;

						// 내부적으로 IMC를 추가한다.
						// Config.LoadSynchronous() 로딩 후, 설정한다.
						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
					}
				}

				USGEnhancedInputComponent* SGIC = CastChecked<USGEnhancedInputComponent>(PlayerInputComponent);
				{
					// InputTag_Move와 InputTag_Look_Mouse에 대해 각각 Input_Move(), Input_LookMouse() 멤버함수에 바인딩한다.
					// -> 바인딩 후, InputEvent에 따라 멤버 함수가 트리거된다.
					SGIC->BindNativeAction(InputConfig, SGGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);
					SGIC->BindNativeAction(InputConfig, SGGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
					SGIC->BindNativeAction(InputConfig, SGGameplayTags::InputTag_CosmeticTest, ETriggerEvent::Triggered, this, &ThisClass::Input_CosmeticTest, false);
					SGIC->BindNativeAction(InputConfig, SGGameplayTags::InputTag_Test_Z, ETriggerEvent::Triggered, this, &ThisClass::Input_CosmeticTop, false);
					SGIC->BindNativeAction(InputConfig, SGGameplayTags::InputTag_Test_X, ETriggerEvent::Triggered, this, &ThisClass::Input_CosmeticBottom, false);
					SGIC->BindNativeAction(InputConfig, SGGameplayTags::InputTag_Test_C, ETriggerEvent::Triggered, this, &ThisClass::Input_C, false);
					SGIC->BindNativeAction(InputConfig, SGGameplayTags::InputTag_Test_V, ETriggerEvent::Triggered, this, &ThisClass::Input_V, false);
					SGIC->BindNativeAction(InputConfig, SGGameplayTags::InputTag_QuickBar_Weapon_0, ETriggerEvent::Triggered, this, &ThisClass::Input_2, false);

					TArray<uint32> BindHnaldes;
					SGIC->BindAbilityActions(InputConfig, this,
						&ThisClass::Input_AbilityInputTagStarted,
						&ThisClass::Input_AbilityInputTagPressed,
						&ThisClass::Input_AbilityInputTagReleased, BindHnaldes);
				}
			}
		}
	}

	// GameFeatureAction_AddInputConfig의 HandlePawnExtension 콜백 함수 전달
	// SendGameFrameworkComponentExtensionEvent(): 이벤트 요청 가능
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(
	const_cast<APawn*>(Pawn), 
	NAME_BindInputsNow);
}

void USGHeroInputComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	check(Pawn);

	AController* Controller = Pawn->GetController<AController>();
	check(Controller);

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

		if (Value.X != 0.f)
		{
			// Left/Right -> X 값에 있음
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		// 우리는 Forward 적용을 위해 Swizzle Input Modifier를 사용했다.
		if (Value.Y != 0.f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void USGHeroInputComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	check(Pawn);

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.f)
	{
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}
}

void USGHeroInputComponent::Input_CosmeticTest(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Payload;
	if (TopIndex == 0)
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Clothes_Top_1);
	}
	else if (TopIndex == 1)
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Clothes_Top_2);
	}
	else if (TopIndex == 2)
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Clothes_Top_3);
	}
	else if (TopIndex == 3)
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Clothes_Top_4);
	}

	if (bBottomIndexPlus)
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Clothes_Bottoms_1);
	}
	else 
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Clothes_Bottoms_2);
	}

	if (bHand)
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Hand_1);
	}
	else 
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Hand_2);
	}

	if (bFoot)
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Foot_1);
	}
	else 
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Foot_2);
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwner(),
		SGGameplayTags::Event_Equip_Cosmetic,
		Payload
	);	
}

void USGHeroInputComponent::Input_CosmeticTop(const FInputActionValue& InputActionValue)
{
	if (bTopIndexPlus)
	{
		TopIndex++;
		
		if (TopIndex > 3)
		{
			bTopIndexPlus = false;
			TopIndex = 2;
		}
	}
	else
	{
		TopIndex--;

		if (TopIndex < 0)
		{
			bTopIndexPlus = true;
			TopIndex = 0;
		}
	}

	FGameplayEventData Payload;
	if (TopIndex == 0)
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Clothes_Top_1);
	}
	else if (TopIndex == 1)
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Clothes_Top_2);
	}
	else if (TopIndex == 2)
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Clothes_Top_3);
	}
	else if (TopIndex == 3)
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Clothes_Top_4);
	}


	// UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
	// 	GetOwner(),
	// 	SGGameplayTags::Event_Equip_Cosmetic,
	// 	Payload
	// );
}

void USGHeroInputComponent::Input_CosmeticBottom(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Payload;
	bBottomIndexPlus = !bBottomIndexPlus;


	if (bBottomIndexPlus)
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Clothes_Bottoms_1);
	}
	else 
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Cosmetic_Female_Clothes_Bottoms_2);
	}

	// UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
	// 	GetOwner(),
	// 	SGGameplayTags::Event_Equip_Cosmetic,
	// 	Payload
	// );
}

void USGHeroInputComponent::Input_C(const FInputActionValue& InputActionValue)
{
	bHand = !bHand;
}

void USGHeroInputComponent::Input_V(const FInputActionValue& InputActionValue)
{
	bFoot = !bFoot;
}

void USGHeroInputComponent::Input_2(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Payload;
	bWeaponFlag = !bWeaponFlag;

	if (!bWeaponFlag)
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Weapon_Sword_OneHand_Big);
	}
	else 
	{
		Payload.InstigatorTags.AddTag(SGGameplayTags::Weapon_Sword_OneHand_Small);
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwner(),
		SGGameplayTags::Event_Equip_Weapon,
		Payload
	);
}

void USGHeroInputComponent::Input_AbilityInputTagStarted(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const USGPawnExtensionComponent* PawnExtComp = USGPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (USGAbilitySystemComponent* LyraASC = PawnExtComp->GetSGAbilitySystemComponent())
			{
				LyraASC->AbilityInputTagStarted(InputTag);
			}
		}
	}
}

void USGHeroInputComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const USGPawnExtensionComponent* PawnExtComp = USGPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (USGAbilitySystemComponent* SGASC = PawnExtComp->GetSGAbilitySystemComponent())
			{
				SGASC->AbilityInputTagPressed(InputTag);
			}
		}
	}
}

void USGHeroInputComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const USGPawnExtensionComponent* PawnExtComp = USGPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (USGAbilitySystemComponent* SGASC = PawnExtComp->GetSGAbilitySystemComponent())
			{
				SGASC->AbilityInputTagReleased(InputTag);
			}
		}
	}
}
