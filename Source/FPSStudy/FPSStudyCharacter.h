// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Kismet/GamePlayStatics.h"
#include "Bullet.h"
#include "BulletObjectPool.h"
#include "FPSStudyCharacter.generated.h"

UCLASS(config=Game)
class AFPSStudyCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;


	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Crouch Input Action */
	// 반 구현
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	/** Walk to Run Action */
	// 미구현
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RunAction;

	/** Run to Walk Action */
	// 미구현
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* UnRunAction;

	/** Make World Speed Slow */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Debug_MakeSlow;

	/** Make World Speed Slow */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Debug_MakeFast;

	/** Change Camera Component */
	// 미구현
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* Debug_ChangeCamera;

	/** Aim Action Enable */
	// 미구현
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimAction;

	/** Aim Action Disable */
	// 미구현
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* UnAimAction;

	/** Fire Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

public:
	AFPSStudyCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isAiming;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool canShot;

	UPROPERTY()
	float shotDelay = 0.5f;

	UPROPERTY()
	ABulletObjectPool* bulletPool = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float yaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pitch;

	UPROPERTY(BlueprintReadOnly)
	FVector2D PlayerMove;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Crouch */
	void CrouchFunction(const FInputActionValue& Value);

	/** Called for Set Run */
	void Run(const FInputActionValue& Value);

	/** Called for Cancle Run */
	void RunCancle(const FInputActionValue& Value);

	/** Called for Set Aim */
	void Aim(const FInputActionValue& Value);

	/** Called for Cancle Aim */
	void UnAim(const FInputActionValue& Value);
	
	/** Called for Fire Weapon */
	void Fire(const FInputActionValue& Value);



	/** for debug : Make World Slow **/
	void WorldSlow(const FInputActionValue& Value);

	/** for debug : Make World Fast **/
	void WorldFast(const FInputActionValue& Value);

	/** for debug : Change Camera View **/
	// 미구현
	void ChangeCamera(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

