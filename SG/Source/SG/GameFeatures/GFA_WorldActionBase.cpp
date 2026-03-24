// Fill out your copyright notice in the Description page of Project Settings.


#include "SG/GameFeatures/GFA_WorldActionBase.h"

#include "GameFeaturesSubsystem.h"

void UGFA_WorldActionBase::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	// 모든 월드를 순회하면서 WorldContext를 가져온다. 
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		// ExperienceManagerComp에서 GameFeatureAction을 활성화하면서, Context에 World를 넣는다.
		if (Context.ShouldApplyToWorldContext(WorldContext))
		{
			// WorldActionBase의 Interface인 AddToWorld() 호출 
			AddToworld(WorldContext, Context);
		}
	}
}
