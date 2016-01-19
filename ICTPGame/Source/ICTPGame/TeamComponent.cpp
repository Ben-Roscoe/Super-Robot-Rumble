// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "TeamComponent.h"



//
// UTeamComponent::AreEnemies
//
bool UTeamComponent::AreEnemies( const UTeamComponent* t1, const UTeamComponent* t2 )
{
	bool AreSame = ( t1->TeamName == t2->TeamName );
	bool AreAny	 = ( t1->TeamName == ETeamName::Any || t2->TeamName == ETeamName::Any );
	return !AreSame || AreAny;
}


//
// UTeamComponent::UTeamComponent
//
UTeamComponent::UTeamComponent()
{
	TeamName = ETeamName::Enemy;
}


