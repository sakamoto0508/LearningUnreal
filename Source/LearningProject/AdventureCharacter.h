// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AdventureCharacter.generated.h"

class UAnimBlueprint;
class UInputMappingContext;
class UInputAction;
class UInputComponent;

UCLASS()
class LEARNINGPROJECT_API AAdventureCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAdventureCharacter();

	UPROPERTY(EditAnywhere, Category = Animation)
	UAnimBlueprint* FirstPersonDefaultAnim;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> FirstPersonContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);

	UFUNCTION()
	void Look(const FInputActionValue& value);

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	FVector FirstPersonCameraOffset = FVector(2.8f, 6.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float FirstPersonFieldOfView = 70.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float FirstPersonScale = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* FirstPersonMeshComponent;
};
