//#include "SoundManagerSubsystem.h"
//
//#include "Kismet/GameplayStatics.h"
//
//#include "Components/AudioComponent.h"
//
//#include "Sound/SoundMix.h"
//#include "Sound/SoundClass.h"
//#include "Sound/SoundAttenuation.h" 
//
//void USoundManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
//{
//    Super::Initialize(Collection);
//
//    BGMAudioComponent = NewObject<UAudioComponent>(this);
//    BGMAudioComponent->RegisterComponent();
//    BGMAudioComponent->bAutoActivate = false;
//    BGMAudioComponent->SetVolumeMultiplier(1.0f);
//    BGMAudioComponent->SetIntParameter(FName("Loop"), 0);
//
//    bIsBGMPaused = false;
//
//    for (int32 i = 0; i < MaxSFXPoolSize; ++i)
//    {
//        UAudioComponent* NewSFXComponent = NewObject<UAudioComponent>(this);
//        NewSFXComponent->RegisterComponent();
//        NewSFXComponent->bAutoActivate = false;
//        NewSFXComponent->SetVolumeMultiplier(1.0f);
//        // Corrected: OnAudioFinished delegate has no parameters.
//        NewSFXComponent->OnAudioFinished.AddDynamic(this, &USoundManagerSubsystem::OnSFXFinished);
//        NewSFXComponent->SetIntParameter(FName("Loop"), 0);
//        SFXAudioComponentPool.Add(NewSFXComponent);
//    }
//
//    if (DefaultSoundMix)
//    {
//        ApplySoundMix(DefaultSoundMix);
//    }
//}
//
//void USoundManagerSubsystem::Deinitialize()
//{
//    for (UAudioComponent* Component : SFXAudioComponentPool)
//    {
//        if (Component && Component->IsRegistered())
//        {
//            Component->DestroyComponent();
//        }
//    }
//    SFXAudioComponentPool.Empty();
//
//    if (BGMAudioComponent && BGMAudioComponent->IsRegistered())
//    {
//        BGMAudioComponent->Stop();
//        BGMAudioComponent->DestroyComponent();
//    }
//    BGMAudioComponent = nullptr;
//
//    Super::Deinitialize();
//}
//
//UAudioComponent* USoundManagerSubsystem::PlaySound(
//    USoundWave* InSound,
//    ESoundChannel Channel,
//    bool bLoopHint,
//    float VolumeMultiplier,
//    float PitchMultiplier,
//    FVector Location,
//    bool bAttachToActor,
//    AActor* AttachActor,
//    USoundAttenuation* AttenuationSettings
//)
//{
//    if (!InSound)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("USoundManagerSubsystem::PlaySound: Input Sound is null for channel %s."), *UEnum::GetValueAsString(Channel));
//        return nullptr;
//    }
//
//    USoundClass** FoundSoundClass = SoundClassMap.Find(Channel);
//    if (!FoundSoundClass || !*FoundSoundClass)
//    {
//        UE_LOG(LogTemp, Warning, TEXT("USoundManagerSubsystem::PlaySound: SoundClass not mapped for channel %s. Ensure sound assets for this channel have a Sound Class assigned in the editor."), *UEnum::GetValueAsString(Channel));
//    }
//
//    UAudioComponent* TargetAudioComponent = nullptr;
//
//    if (Channel == ESoundChannel::BGM)
//    {
//        TargetAudioComponent = BGMAudioComponent;
//        if (TargetAudioComponent->IsPlaying())
//        {
//            TargetAudioComponent->Stop();
//        }
//        CurrentBGM = InSound;
//        bIsBGMPaused = false;
//        TargetAudioComponent->SetAttenuationSettings(nullptr);
//
//    }
//    else
//    {
//        TargetAudioComponent = GetOrCreateSFXAudioComponent();
//        if (!TargetAudioComponent)
//        {
//            UE_LOG(LogTemp, Error, TEXT("USoundManagerSubsystem::PlaySound: Failed to get or create SFX AudioComponent for channel %s."), *UEnum::GetValueAsString(Channel));
//            return nullptr;
//        }
//
//        TargetAudioComponent->SetAttenuationSettings(AttenuationSettings);
//
//        if (bAttachToActor && AttachActor)
//        {
//            if (!TargetAudioComponent->IsAttachedTo(AttachActor->GetRootComponent()))
//            {
//                TargetAudioComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
//                TargetAudioComponent->AttachToComponent(AttachActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
//                TargetAudioComponent->SetRelativeLocation(FVector::ZeroVector);
//            }
//        }
//        else
//        {
//            TargetAudioComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
//            TargetAudioComponent->SetWorldLocation(Location);
//        }
//    }
//
//    if (TargetAudioComponent)
//    {
//        TargetAudioComponent->SetSound(InSound);
//        TargetAudioComponent->SetVolumeMultiplier(VolumeMultiplier);
//        TargetAudioComponent->SetPitchMultiplier(PitchMultiplier);
//
//        TargetAudioComponent->Play();
//        return TargetAudioComponent;
//    }
//
//    return nullptr;
//}
//
//void USoundManagerSubsystem::PauseBGM()
//{
//    if (BGMAudioComponent && BGMAudioComponent->IsPlaying() && !bIsBGMPaused)
//    {
//        BGMAudioComponent->SetPaused(true);
//        bIsBGMPaused = true;
//    }
//}
//
//void USoundManagerSubsystem::ResumeBGM()
//{
//    if (BGMAudioComponent && bIsBGMPaused)
//    {
//        BGMAudioComponent->SetPaused(false);
//        bIsBGMPaused = false;
//    }
//}
//
//void USoundManagerSubsystem::StopBGM()
//{
//    if (BGMAudioComponent && (BGMAudioComponent->IsPlaying() || bIsBGMPaused))
//    {
//        BGMAudioComponent->Stop();
//        CurrentBGM = nullptr;
//        bIsBGMPaused = false;
//    }
//}
//
//void USoundManagerSubsystem::SetMasterVolume(float Volume)
//{
//    USoundClass** MasterSC = SoundClassMap.Find(ESoundChannel::BGM);
//    if (MasterSC && *MasterSC)
//    {
//        (*MasterSC)->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
//        UE_LOG(LogTemp, Warning, TEXT("USoundManagerSubsystem::SetMasterVolume: Directly modifying SoundClass volume. This is often overridden by active Sound Mixes. Consider using ApplySoundMix for dynamic changes."));
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("USoundManagerSubsystem::SetMasterVolume: Master Sound Class (typically BGM) not set in SoundClassMap."));
//    }
//}
//
//void USoundManagerSubsystem::SetChannelVolume(ESoundChannel Channel, float Volume)
//{
//    USoundClass** FoundSoundClass = SoundClassMap.Find(Channel);
//    if (FoundSoundClass && *FoundSoundClass)
//    {
//        (*FoundSoundClass)->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
//        UE_LOG(LogTemp, Warning, TEXT("USoundManagerSubsystem::SetChannelVolume: Directly modifying SoundClass volume. This is often overridden by active Sound Mixes. Consider using ApplySoundMix for dynamic changes."));
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("USoundManagerSubsystem::SetChannelVolume: SoundClass not found for channel %s in SoundClassMap."), *UEnum::GetValueAsString(Channel));
//    }
//}
//
//void USoundManagerSubsystem::ApplySoundMix(USoundMix* InSoundMix)
//{
//    if (InSoundMix)
//    {
//        UGameplayStatics::PushSoundMixModifier(GetGameInstance(), InSoundMix);
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("USoundManagerSubsystem::ApplySoundMix: Input SoundMix is null."));
//    }
//}
//
//void USoundManagerSubsystem::RemoveSoundMix(USoundMix* InSoundMix)
//{
//    if (InSoundMix)
//    {
//        UGameplayStatics::PopSoundMixModifier(GetGameInstance(), InSoundMix);
//    }
//    else
//    {
//        UE_LOG(LogTemp, Warning, TEXT("USoundManagerSubsystem::RemoveSoundMix: Input SoundMix is null."));
//    }
//}
//
//UAudioComponent* USoundManagerSubsystem::GetOrCreateSFXAudioComponent()
//{
//    for (UAudioComponent* Component : SFXAudioComponentPool)
//    {
//        // A component is considered free if it's not currently playing and its sound is cleared.
//        if (!Component->IsPlaying() && Component->GetSound() == nullptr)
//        {
//            return Component;
//        }
//    }
//
//    if (SFXAudioComponentPool.Num() < MaxSFXPoolSize)
//    {
//        UAudioComponent* NewSFXComponent = NewObject<UAudioComponent>(this);
//        NewSFXComponent->RegisterComponent();
//        NewSFXComponent->bAutoActivate = false;
//        NewSFXComponent->SetVolumeMultiplier(1.0f);
//        NewSFXComponent->SetIntParameter(FName("Loop"), 0);
//        NewSFXComponent->OnAudioFinished.AddDynamic(this, &USoundManagerSubsystem::OnSFXFinished);
//        SFXAudioComponentPool.Add(NewSFXComponent);
//        return NewSFXComponent;
//    }
//
//    UE_LOG(LogTemp, Warning, TEXT("USoundManagerSubsystem::GetOrCreateSFXAudioComponent: SFX pool is full and no free components. Consider increasing MaxSFXPoolSize."));
//    return nullptr;
//}
//
//// Since OnAudioFinished doesn't provide the AudioComponent, we need a way to find it.
//// This is a simplistic approach; a more robust pool would track active components.
//void USoundManagerSubsystem::OnSFXFinished()
//{
//    // Find the first AudioComponent in the pool that is no longer playing.
//    // This assumes only one component will finish at a time, or that order doesn't matter.
//    // For more complex scenarios, you might need a map of active sounds to their components.
//    for (UAudioComponent* Component : SFXAudioComponentPool)
//    {
//        if (Component && !Component->IsPlaying())
//        {
//            ReturnSFXAudioComponentToPool(Component);
//            return; // Found and returned one, exit.
//        }
//    }
//    UE_LOG(LogTemp, Warning, TEXT("USoundManagerSubsystem::OnSFXFinished: Could not find a finished SFX AudioComponent to return to pool."));
//}
//
//
//void USoundManagerSubsystem::ReturnSFXAudioComponentToPool(UAudioComponent* AudioComponent)
//{
//    if (AudioComponent && SFXAudioComponentPool.Contains(AudioComponent))
//    {
//        AudioComponent->Stop();
//        AudioComponent->SetSound(nullptr); // Clear the sound to mark as available
//        AudioComponent->SetAttenuationSettings(nullptr);
//        AudioComponent->SetIntParameter(FName("Loop"), 0);
//        if (AudioComponent->IsAttachedTo(nullptr) == false)
//        {
//            AudioComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
//        }
//    }
//}