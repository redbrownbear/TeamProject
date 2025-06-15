#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundManagerSubsystem.generated.h"

class USoundMix;
class USoundWave;
class UAudioComponent;

UENUM(BlueprintType)
enum class ESoundChannel : uint8
{
    BGM             UMETA(DisplayName = "Background Music"),
    PlayerSFX       UMETA(DisplayName = "Player Sound Effect"),
    MonsterSFX      UMETA(DisplayName = "Monster Sound Effect"),
    UISFX           UMETA(DisplayName = "UI Sound Effect"),
    EnvironmentalSFX UMETA(DisplayName = "Environmental Sound Effect"),
};

UCLASS()
class TEAMPROJECT_API USoundManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
//
//public:
//    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
//    virtual void Deinitialize() override;
//
//    UPROPERTY(EditAnywhere, Category = "Sound Manager|Configuration")
//    TMap<ESoundChannel, USoundClass*> SoundClassMap;
//
//    UPROPERTY(EditAnywhere, Category = "Sound Manager|Configuration")
//    USoundMix* DefaultSoundMix;
//
//    UPROPERTY()
//    UAudioComponent* BGMAudioComponent;
//
//    UPROPERTY()
//    USoundWave* CurrentBGM;
//
//    // Track BGM pause state
//    UPROPERTY(BlueprintReadOnly, Category = "Sound Manager")
//    bool bIsBGMPaused;
//
//    UPROPERTY(EditAnywhere, Category = "Sound Manager|SFX Pooling")
//    int32 MaxSFXPoolSize = 30;
//
//    UPROPERTY()
//    TArray<UAudioComponent*> SFXAudioComponentPool;
//
//    UFUNCTION(BlueprintCallable, Category = "Sound Manager")
//    UAudioComponent* PlaySound(
//        USoundWave* InSound,
//        ESoundChannel Channel,
//        bool bLoopHint = false, // This parameter signifies the intent for looping, primarily handled by the sound asset itself.
//        float VolumeMultiplier = 1.0f,
//        float PitchMultiplier = 1.0f,
//        FVector Location = FVector::ZeroVector,
//        bool bAttachToActor = false,
//        AActor* AttachActor = nullptr,
//        USoundAttenuation* AttenuationSettings = nullptr
//    );
//
//    UFUNCTION(BlueprintCallable, Category = "Sound Manager")
//    void PauseBGM();
//
//    UFUNCTION(BlueprintCallable, Category = "Sound Manager")
//    void ResumeBGM();
//
//    UFUNCTION(BlueprintCallable, Category = "Sound Manager")
//    void StopBGM();
//
//    UFUNCTION(BlueprintCallable, Category = "Sound Manager")
//    void SetMasterVolume(float Volume);
//
//    UFUNCTION(BlueprintCallable, Category = "Sound Manager")
//    void SetChannelVolume(ESoundChannel Channel, float Volume);
//
//    UFUNCTION(BlueprintCallable, Category = "Sound Manager")
//    void ApplySoundMix(USoundMix* InSoundMix);
//
//    UFUNCTION(BlueprintCallable, Category = "Sound Manager")
//    void RemoveSoundMix(USoundMix* InSoundMix);
//
//protected:
//    // This function must now have no parameters to match OnAudioFinished delegate signature
//    UFUNCTION()
//    void OnSFXFinished();
//
//    //// Helper to find the component that just finished playback
//    //UAudioComponent* GetSFXComponentBySound(USoundWave* InSound);
//
//    // Get or Create SFX Audio Component
//    UAudioComponent* GetOrCreateSFXAudioComponent();
//    // Return SFX Audio Component to pool
//    void ReturnSFXAudioComponentToPool(UAudioComponent* AudioComponent);
};