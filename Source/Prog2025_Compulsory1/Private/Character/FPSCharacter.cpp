#include "Character/FPSCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("StartPlay"));
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AFPSCharacter::MoveInput);
		Input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AFPSCharacter::LookInput);
		Input->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &AFPSCharacter::JumpInput);
	}
}

void AFPSCharacter::MoveInput(const FInputActionValue& InputValue)
{
	//Debug message for MoveInput
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("MOVE"));

	const FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		//Forward Direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation (0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, InputVector.Y);
		AddMovementInput(RightDirection, InputVector.X);
	}
}

void AFPSCharacter::LookInput(const FInputActionValue& InputValue)
{
	//Debug message for LookInput
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("Look"));

	const FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void AFPSCharacter::JumpInput()
{
	//Debug message for Jumping
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("JUMP"));

	AFPSCharacter::Jump();
}

