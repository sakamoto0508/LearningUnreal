// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureCharacter.h"

#include <rapidjson/document.h>

// Sets default values
AAdventureCharacter::AAdventureCharacter()
{
	// Tick()を毎フレーム呼び出す
	PrimaryActorTick.bCanEverTick = true;
	// 一人称視点用カメラを生成
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FirstPersonCameraComponent != nullptr);
	// 一人称視点用の腕メッシュを生成
	FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FirstPersonMeshComponent != nullptr);
	// 腕メッシュをカメラの子にする
	// カメラが動けば腕も一緒に動く
	FirstPersonMeshComponent->SetupAttachment(FirstPersonCameraComponent);
	// このメッシュは一人称視点専用として扱う
	FirstPersonMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	// 本体メッシュも一人称視点用設定
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	// 自分だけに腕メッシュを表示する
	FirstPersonMeshComponent->SetOnlyOwnerSee(true);
	// カメラを頭のソケットにアタッチ
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMeshComponent, FName("head"));
	// カメラの位置と回転を調整
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FirstPersonCameraOffset, FRotator(0.0f, 90.0f, -90.0f));
	// コントローラーの回転をカメラに反映
	// AddControllerYawInput() や
	// AddControllerPitchInput() が効くようになる
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	// 一人称視点専用FOVを有効化
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	// 一人称視点専用スケールを有効化
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	// FOV設定
	FirstPersonCameraComponent->FirstPersonFieldOfView = FirstPersonFieldOfView;
	// スケール設定
	FirstPersonCameraComponent->FirstPersonScale = FirstPersonScale;
}

// Called when the game starts or when spawned
void AAdventureCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	// Get the player controller for this character
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Get the enhanced input local player subsystem and add a new input mapping context to it
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FirstPersonContext, 0);
		}
	}
	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using AdventureCharacter."));
}

// Called every frame
void AAdventureCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AAdventureCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(
		PlayerInputComponent))
	{
		//バインドムーブアクション
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAdventureCharacter::Move);
		//バインドジャンプアクション
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//バインドルックアクション
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAdventureCharacter::Look);
	}
}

void AAdventureCharacter::Move(const FInputActionValue& value)
{
	const FVector2D MovementValue = value.Get<FVector2D>();

	if (Controller)
	{
		const FVector Right = GetActorRightVector();
		AddMovementInput(Right, MovementValue.X);

		const FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, MovementValue.Y);
	}
}

void AAdventureCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookAxisValue = value.Get<FVector2D>();

	GEngine->AddOnScreenDebugMessage(
		0,
		0.0f,
		FColor::Green,
		FString::Printf(TEXT("X=%f Y=%f"),
		                LookAxisValue.X,
		                LookAxisValue.Y)
	);

	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}
