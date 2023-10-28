// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AN_PlayRandomSound.h"

#include "Kismet/GameplayStatics.h"

void UAN_PlayRandomSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	int32 RandomInt = FMath::RandRange(0, RandomSoundArray.Num() - 1);
	if (RandomSoundArray.IsValidIndex(RandomInt) == false || RandomSoundArray[RandomInt] == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAN_PlayRandomSound::Notify, RandomSoundArray.IsValidIndex(RandomInt) == false || RandomSoundArray[RandomInt] == nullptr"));
		return;
	}

	USoundBase* SoundForPlay = RandomSoundArray[RandomInt];

	// Don't call super to avoid call back in to blueprints
	if (SoundForPlay && MeshComp)
	{
		if (!SoundForPlay->IsOneShot())
		{
			UE_LOG(LogAudio, Warning, TEXT("PlaySound notify: Anim %s tried to play a sound asset which is not a one-shot: '%s'. Spawning suppressed."), *GetNameSafe(Animation), *GetNameSafe(SoundForPlay));
			return;
		}

#if WITH_EDITORONLY_DATA
		UWorld* World = MeshComp->GetWorld();
		if (bPreviewIgnoreAttenuation && World && World->WorldType == EWorldType::EditorPreview)
		{
			UGameplayStatics::PlaySound2D(World, SoundForPlay, VolumeMultiplier, PitchMultiplier);
		}
		else
#endif
		{
			if (bFollow)
			{
				UGameplayStatics::SpawnSoundAttached(SoundForPlay, MeshComp, AttachName, FVector(ForceInit), EAttachLocation::SnapToTarget, false, VolumeMultiplier, PitchMultiplier);
			}
			else
			{
				UGameplayStatics::PlaySoundAtLocation(MeshComp->GetWorld(), SoundForPlay, MeshComp->GetComponentLocation(), VolumeMultiplier, PitchMultiplier);
			}
		}
	}
}
