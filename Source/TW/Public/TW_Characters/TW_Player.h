// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TW_BaseCharacter.h"
#include "TW_Player.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateAmmoDelegate, int32, CurrentGunsAmmo, int32, TotalGunAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerDamagedDelegate, int32, CurrentPlayerHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeadEyeStartDelegate, int32, CurrentTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeadEyeEndDelegate, int32, CurrentTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIsAimingDelegate, bool, bAiming);

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UInputComponent;
class ATW_Gun;
class ATW_Hud;
class UCameraShakeBase;

UCLASS()
class ATW_Player : public ATW_BaseCharacter
{
	GENERATED_BODY()

public:
	ATW_Player();
	
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void Shoot(const FInputActionValue& Value);

	void StartAiming(const FInputActionValue& Value);

	void StopAiming(const FInputActionValue& Value);

	void DeadEye(const FInputActionValue& Value);

	void Reload(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAmmo(int32 CurrentGunsAmmo, int32 TotalGunAmmo);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerWasDamaged(int32 PlayersCurrentHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void DeadEyeInProgress(int32 CurrentTime);
	
	UFUNCTION(BlueprintImplementableEvent)
	void DeadEyeEnded(int32 CurrentTime);

	UFUNCTION(BlueprintImplementableEvent)
	void IsPlayerAiming(bool bAiming);

	UFUNCTION(BlueprintImplementableEvent)
	void GameStatusUpdate(const FString& Status);
	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(BlueprintAssignable)
	FUpdateAmmoDelegate UpdateAmmoDelegate;

	UPROPERTY(BlueprintAssignable)
	FPlayerDamagedDelegate PlayerDamaged;

	UPROPERTY(BlueprintAssignable)
	FDeadEyeStartDelegate StartDeadEye;

	UPROPERTY(BlueprintAssignable)
	FDeadEyeEndDelegate EndDeadEye;

	UPROPERTY(BlueprintAssignable)
	FIsAimingDelegate PlayerAiming;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DeadEye")
	int32 MaxDeadEyeTime = 5;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DeadEye")
	int32 CurrentDeadEyeTime = 5;
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	virtual void ChangeHealth(int32 HealthValue) override;

	UFUNCTION(BlueprintCallable)
	virtual void ChangeAmmo(int32 AmmoValue) override;

	virtual void FillAmmo() override;

	virtual void SetShootingFlag(bool ShootingFlag) override;

	void ShootDeadEyeTargets();
	
	UFUNCTION()
	void UpdateDeadEyeMeter();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> ShootingCameraShakeClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float TurnRate = 20.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* StartAimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* StopAimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DeadEyeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;
	
	FTimerHandle UpdateDeadEyeMeterHandle;
	
	bool bDeadEyeInProgress = false;

	bool bShootingDeadEyeTargets = false;

	UPROPERTY()
	TArray<ATW_BaseCharacter*> DeadEyeTargets;

	int32 DeadEyeIndex = 0;
};

