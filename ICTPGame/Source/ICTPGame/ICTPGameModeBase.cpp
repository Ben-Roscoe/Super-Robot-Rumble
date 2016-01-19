// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "ICTPGameModeBase.h"

#include "AudioDevice.h"




//
// AICTPGameModeBase::ChangeSoundClassVolume
//
void AICTPGameModeBase::ChangeSoundClassVolume( FString SoundClassName, float Value )
{
	auto Device = GEngine->GetMainAudioDevice();
	if( Device == nullptr )
	{
		return;
	}

	for( TMap<USoundClass*, FSoundClassProperties>::TIterator It( Device->SoundClasses ); It; ++It )
	{
		USoundClass* SoundClass = It.Key();
		if( SoundClass != nullptr && SoundClass->GetFullName().Find( SoundClassName ) != INDEX_NONE )
		{
			SoundClass->Properties.Volume = Value;
			break;
		}
	}
}


//
// AICTPGameModeBase::GetSoundClassVolume
//
float AICTPGameModeBase::GetSoundClassVolume( FString SoundClassName )
{
	auto Device = GEngine->GetMainAudioDevice();
	if( Device == nullptr )
	{
		return 0.0f;
	}

	for( TMap<USoundClass*, FSoundClassProperties>::TIterator It( Device->SoundClasses ); It; ++It )
	{
		USoundClass* SoundClass = It.Key();
		if( SoundClass != nullptr && SoundClass->GetFullName().Find( SoundClassName ) != INDEX_NONE )
		{
			return SoundClass->Properties.Volume;
		}
	}

	return 0.0f;
}