#pragma once
#include "CoreMinimal.h"

// value
const float SPEED1 = 0.1f;
const float SPEED2 = 0.01f;
const float SPEED3 = 0.001f;
const float SPEED4 = 0.0001f;

const float DELAY1 = 0.1f;
const float DELAY2 = 0.2f;
const float DELAY3 = 0.3f;
const float DELAY4 = 0.4f;
const float DELAY5 = 0.5f;


// debug
#define DISPLAYLOG(text, ...) UE_LOG(LogTemp, Display, text,##__VA_ARGS__);
#define WARNINGLOG(text, ...) UE_LOG(LogTemp, Warning, text,##__VA_ARGS__);
#define	ERRORLOG(text, ...) UE_LOG(LogTemp, Error, text,##__VA_ARGS__);

#define SCREENLOG(key, time, color, text,/*bool NewOntop(true),FVector2D textScale */ ...) if(GEngine)\
	{\
		GEngine->AddOnScreenDebugMessage(key,time,color,text,__VA_ARGS__); \
	}
// ex) SCREENLOG(INDEX_NONE, 5.f, FColor::Red, TEXT("Red!!"));
