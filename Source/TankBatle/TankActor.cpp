// Fill out your copyright notice in the Description page of Project Settings.


#include "TankActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TankMovementComponent.h"

// Sets default values
ATankActor::ATankActor()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(50.f, 50.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	//GetCharacterMovement()->JumpZVelocity = 600.f;
	//GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1000.0f; // The camera follows at this distance behind the character	
	CameraBoom->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	static ConstructorHelpers::FObjectFinder<UMaterial> material(TEXT("Material'/Game/Materials/MAT_Tank_Blue.MAT_Tank_Blue'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MDL_TankChassis(TEXT("StaticMesh'/Game/StaticMeshs/MDL_TankChassis.MDL_TankChassis'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MDL_TankBarrel(TEXT("StaticMesh'/Game/StaticMeshs/MDL_TankBarrel.MDL_TankBarrel'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MDL_TankTurret(TEXT("StaticMesh'/Game/StaticMeshs/MDL_TankTurret.MDL_TankTurret'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MDL_TankWheel_BL(TEXT("StaticMesh'/Game/StaticMeshs/MDL_TankWheel_BL.MDL_TankWheel_BL'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MDL_TankWheel_BR(TEXT("StaticMesh'/Game/StaticMeshs/MDL_TankWheel_BR.MDL_TankWheel_BR'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MDL_TankWheel_FL(TEXT("StaticMesh'/Game/StaticMeshs/MDL_TankWheel_FL.MDL_TankWheel_FL'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MDL_TankWheel_FR(TEXT("StaticMesh'/Game/StaticMeshs/MDL_TankWheel_FR.MDL_TankWheel_FR'"));

	UStaticMeshComponent* TankChassis_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankChassis"));
	TankChassis_Mesh->SetupAttachment(GetMesh());
	TankChassis_Mesh->SetStaticMesh(MDL_TankChassis.Object);
	TankChassis_Mesh->SetMaterial(0, material.Object);
	TankChassis_Mesh->SetRelativeLocation(FVector(0.0f, -29.0f, -59.0f));
	TankChassis_Mesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	TankChassis_Mesh->SetRelativeScale3D(FVector(5, 5, 5));

	UStaticMeshComponent* TankBarrel_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankBarrel"));
	TankBarrel_Mesh->AttachToComponent(TankChassis_Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	TankBarrel_Mesh->SetStaticMesh(MDL_TankBarrel.Object);
	TankBarrel_Mesh->SetMaterial(0, material.Object);

	UStaticMeshComponent* TankTurret_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankTurret"));
	TankTurret_Mesh->AttachToComponent(TankChassis_Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	TankTurret_Mesh->SetStaticMesh(MDL_TankTurret.Object);
	TankTurret_Mesh->SetMaterial(0, material.Object);

	UStaticMeshComponent* TankWheel_BL_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankWheel_BL"));
	TankWheel_BL_Mesh->AttachToComponent(TankChassis_Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	TankWheel_BL_Mesh->SetStaticMesh(MDL_TankWheel_BL.Object);
	TankWheel_BL_Mesh->SetMaterial(0, material.Object);

	UStaticMeshComponent* TankWheel_BR_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankWheel_BR"));
	TankWheel_BR_Mesh->AttachToComponent(TankChassis_Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	TankWheel_BR_Mesh->SetStaticMesh(MDL_TankWheel_BR.Object);
	TankWheel_BR_Mesh->SetStaticMesh(MDL_TankWheel_BR.Object);
	TankWheel_BR_Mesh->SetMaterial(0, material.Object);

	UStaticMeshComponent* TankWheel_FL_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankWheel_FL"));
	TankWheel_FL_Mesh->AttachToComponent(TankChassis_Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	TankWheel_FL_Mesh->SetStaticMesh(MDL_TankWheel_FL.Object);
	TankWheel_FL_Mesh->SetMaterial(0, material.Object);

	UStaticMeshComponent* TankWheel_FR_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankWheel_FR"));
	TankWheel_FR_Mesh->AttachToComponent(TankChassis_Mesh, FAttachmentTransformRules::KeepRelativeTransform);
	TankWheel_FR_Mesh->SetStaticMesh(MDL_TankWheel_FR.Object);
	TankWheel_FR_Mesh->SetMaterial(0, material.Object);

}

// Called when the game starts or when spawned
void ATankActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATankActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATankActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ATankActor::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATankActor::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}


void ATankActor::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATankActor::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
