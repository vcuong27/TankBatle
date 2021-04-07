// Copyright Epic Games, Inc. All Rights Reserved.


#include "TankBatleGameModeBase.h"
#include "TankBatleController.h"
#include "TankActor.h"

ATankBatleGameModeBase::ATankBatleGameModeBase()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATankBatleController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Class'/Script/TankBatle.TankActor'"));
	if (PlayerPawnBPClass.Class != nullptr)
	{

		UE_LOG(LogTemp, Warning, TEXT("ATankBatleGameModeBase set defaul pawn"));

		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
