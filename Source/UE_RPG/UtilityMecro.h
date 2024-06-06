#pragma once
#include "CoreMinimal.h"

// value
#define SPEED1 0.1f
#define SPEED2 0.01f
#define SPEED3 0.001f
#define SPEED4 0.0001f

#define DELAY1 0.1f
#define DELAY2 0.2f
#define DELAY3 0.3f
#define DELAY4 0.4f
#define DELAY5 0.5f


// debug
#define DISPLAYLOG(text, ...) UE_LOG(LogTemp, Display, text,##__VA_ARGS__);
#define WARNINGLOG(text, ...) UE_LOG(LogTemp, Warning, text,##__VA_ARGS__);
#define	ERRORLOG(text, ...) UE_LOG(LogTemp, Error, text,##__VA_ARGS__);

#define SCREENLOG(key, time, color, text,/*bool NewOntop(true),FVector2D textScale */ ...) if(GEngine)\
	{\
		GEngine->AddOnScreenDebugMessage(key,time,color,text,__VA_ARGS__); \
	}
// ex) SCREENLOG(INDEX_NONE, 5.f, FColor::Red, TEXT("Red!!"));
