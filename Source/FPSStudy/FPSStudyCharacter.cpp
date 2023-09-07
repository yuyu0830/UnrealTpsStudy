// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSStudyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// AFPSStudyCharacter

AFPSStudyCharacter::AFPSStudyCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	isCrouch = false;
	isRun = false;
	isAiming = false;
	canShot = true;

	yaw = 0.f;
	pitch = 0.f;
}

void AFPSStudyCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// 유진 코드
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABulletObjectPool::StaticClass(), foundActors); //ABulletObjectPool 클래스의 액터를 찾아서 배열에 저장

	if (!foundActors.IsEmpty())
	{
		bulletPool = Cast<ABulletObjectPool>(foundActors[0]); //AActor* 형을 ABulletObjectPool형태로 형변환 후 bulletPool 변수에 할당
		UE_LOG(LogTemp, Log, TEXT("Success to refer bullet pool"));
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFPSStudyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSStudyCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSStudyCharacter::Look);

		//Pose
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AFPSStudyCharacter::CrouchFunction);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AFPSStudyCharacter::Run);
		EnhancedInputComponent->BindAction(UnRunAction, ETriggerEvent::Triggered, this, &AFPSStudyCharacter::RunCancle);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AFPSStudyCharacter::Aim);
		EnhancedInputComponent->BindAction(UnAimAction, ETriggerEvent::Triggered, this, &AFPSStudyCharacter::UnAim);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AFPSStudyCharacter::Fire);

		//Debug
		EnhancedInputComponent->BindAction(Debug_MakeFast, ETriggerEvent::Triggered, this, &AFPSStudyCharacter::WorldSlow);
		EnhancedInputComponent->BindAction(Debug_MakeSlow, ETriggerEvent::Triggered, this, &AFPSStudyCharacter::WorldFast);
	}

}

void AFPSStudyCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	PlayerMove = MovementVector;

	float speed;


	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		if (isAiming) { // 에임 땡기고 있으면 느리게
			speed = 0.5f;
		}

		else { // 에임 안땡기고 있으면 빠르게
			speed = 1.f;
		}

		//UE_LOG(LogTemp, Log, TEXT("ForwardDirection %s"), *ForwardDirection.ToString());
		//UE_LOG(LogTemp, Log, TEXT("RightDirection %s"), *RightDirection.ToString());
		// 
		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y * speed);
		AddMovementInput(RightDirection, MovementVector.X * speed);
	}
}

void AFPSStudyCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		/*yaw += LookAxisVector.X;
		pitch += LookAxisVector.Y;

		if (yaw > 90) yaw = 90;
		else if (yaw < -90) yaw = -90;

		if (pitch > 90) pitch = 90;
		else if (pitch < -90) pitch = -90;*/
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFPSStudyCharacter::CrouchFunction(const FInputActionValue& Value) {
	// 반 구현
	UE_LOG(LogTemp, Log, TEXT("Crouch"));
	if (isCrouch) {
		UE_LOG(LogTemp, Log, TEXT("UnCrouch"));
		isCrouch = false;
		UnCrouch();
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("Crouch"));
		isCrouch = true;
		Crouch();
	}
	FollowCamera->GetForwardVector();
}

void AFPSStudyCharacter::Run(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Log, TEXT("Run"));
	bUseControllerRotationYaw = false;
}

void AFPSStudyCharacter::RunCancle(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Log, TEXT("RunCancle"));
	bUseControllerRotationYaw = true;
}

void AFPSStudyCharacter::Aim(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Log, TEXT("Aim"));
	isAiming = true;
	FTransform LocalTransform = FollowCamera->GetRelativeTransform();
	LocalTransform.AddToTranslation(FVector(300, 35, 0));
	FollowCamera->SetRelativeTransform(LocalTransform);
	bUseControllerRotationYaw = true;
}

void AFPSStudyCharacter::UnAim(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Log, TEXT("UnAim"));
	isAiming = false;
	FTransform LocalTransform = FollowCamera->GetRelativeTransform();
	LocalTransform.AddToTranslation(FVector(-300, -35, 0));
	FollowCamera->SetRelativeTransform(LocalTransform);
	bUseControllerRotationYaw = false;
}

void AFPSStudyCharacter::Fire(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Log, TEXT("Fire"));
	if (Controller != nullptr)
	{
		if (canShot && isAiming)
		{
			//FVector position = GetActorLocation() + GetActorForwardVector() * 50; // 월드 내 위치
			const FRotator rotation(pitch, yaw, 0);

			canShot = false;

			ABullet* spawnedBullet = Cast<ABullet>(bulletPool->GetBullet()); //bulletPool은 AActor* 형태로 bullet 오브젝트를 반환해주므로 Cast함수를 통해 Abullet*으로 형변환
			spawnedBullet->BulletInit(this, rotation); //bullet의 forward Vector, shooter 설정

			FTimerHandle delay;
			GetWorldTimerManager().SetTimer(delay, FTimerDelegate::CreateLambda([&]()
				{
					canShot = true;
				}), shotDelay, false);
		}
	}
}


void AFPSStudyCharacter::WorldSlow(const FInputActionValue& Value) {
	GetWorldSettings()->SetTimeDilation(0.25f);
}

void AFPSStudyCharacter::WorldFast(const FInputActionValue& Value) {
	GetWorldSettings()->SetTimeDilation(1);
}

void AFPSStudyCharacter::ChangeCamera(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Log, TEXT("ChangeCamera"));
	
}