// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/Cosmetics/SGPawnComp_CharacterParts.h"
#include "GameFramework/Character.h"


USGPawnComp_CharacterParts::USGPawnComp_CharacterParts(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FSGCharacterPartHandle USGPawnComp_CharacterParts::AddCharacterPart(const FSGCharacterPart& NewPart)
{
	return FSGCharacterPartHandle();
}

