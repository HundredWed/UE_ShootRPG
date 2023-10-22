// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/EnemyBase.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
AEnemyBase::AEnemyBase()
{

}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(HitActionMontage_NoDamaged) && !IsValid(HitActionMontage))
	{
		WARNINGLOG(TEXT("Enemy HitMontage is not!!"));
	}

	CharaterType = ECharacterTypes::NPC_Monster;
}

void AEnemyBase::SetActionStateNormal()
{
	CharaterActionState = ECharacterActionState::Normal;
}

void AEnemyBase::GetHit(const FVector& targetLocation)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!IsValid(AnimInstance))
	{
		WARNINGLOG(TEXT("is not IsValid Enemy AnimInstance!!"));
		return;
	}

	double dis = (GetActorLocation() - targetLocation).Length();

	if (dis >= NoDamagedDistance)
	{
		NoDamaged(AnimInstance, targetLocation);
		SCREENLOG(INDEX_NONE, 5.f, FColor::Black, TEXT("NoDamaged!!"));
		return;
	}

	GetDamage(AnimInstance);
}

void AEnemyBase::NoDamaged(UAnimInstance* animInstance, const FVector& targetLocation)
{
	if (IsValid(HitActionMontage_NoDamaged) && CharaterActionState == ECharacterActionState::Normal)
	{
		//SCREENLOG(INDEX_NONE, 5.f, FColor::Red, TEXT("Ouch!!"));
		animInstance->Montage_Play(HitActionMontage_NoDamaged);
		LooAtTarget(targetLocation);
		CharaterActionState = ECharacterActionState::Action;
	}
}

void AEnemyBase::GetDamage(UAnimInstance* animInstance)
{
	animInstance->Montage_Play(HitActionMontage);

	/**update hp*/
}

void AEnemyBase::LooAtTarget(const FVector& targetLocation)
{
	const FVector forward = GetActorForwardVector();

	const FVector impactLowered(targetLocation.X, targetLocation.Y, GetActorLocation().Z);
	const FVector toHit = (impactLowered - GetActorLocation()).GetSafeNormal();

	const double cosTheta = FVector::DotProduct(forward, toHit);
	double theta = FMath::Acos(cosTheta);
	theta = FMath::RadiansToDegrees(theta);

	const FVector CrossProduct = FVector::CrossProduct(forward, toHit);
	if (CrossProduct.Z < 0)
	{
		theta *= -1.f;
	}

	CurrentTurningValue = 0;

	if (theta <= -70.f && theta >= -110.f)/**left*/
	{
		//enemyrot.Yaw += Theta;
		//SetActorRotation(enemyrot);
		TurnAtHitDir(theta);
	}
	else if (theta >= 70.f && theta <= 110.f)/**right*/
	{
		//enemyrot.Yaw += Theta;
		//SetActorRotation(enemyrot);
		TurnAtHitDir(theta);
	}
	else if (theta <= -160.f || theta >= 160.f)/**back*/
	{
		//enemyrot.Yaw += Theta;
		//SetActorRotation(enemyrot);
		TurnAtHitDir(theta);
	}
}

void AEnemyBase::TurnAtHitDir(const double theta)
{
	if (CurrentTurningValue > fabs(theta))
	{
		WARNINGLOG(TEXT("TurnAtHitDir End!!!"));
		return;
	}
		
	if (theta > 0)
	{
		CurrentTurningValue += TurnSpeed;

		FRotator rot = GetActorRotation();
		rot.Yaw += TurnSpeed;
		SetActorRotation(rot);
	}
	else if (theta < 0)
	{
		CurrentTurningValue += TurnSpeed;

		FRotator rot = GetActorRotation();
		rot.Yaw -= TurnSpeed;
		SetActorRotation(rot);
	}

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([this, theta]() {TurnAtHitDir(theta); });

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.01f, false);

}

void AEnemyBase::TurnAtHitDir_Ver1(const FRotator& enemyrot) 
{
	//SCREENLOG(INDEX_NONE, 5.f, FColor::Black, FString::Printf(TEXT("TurnAtHitDir")));
	//DISPLAYLOG(TEXT("TurnAtHitDir"));

	FRotator currentRot = GetActorRotation();
	float newYaw = FMath::FInterpTo(currentRot.Yaw, enemyrot.Yaw, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 15.f);

	if (((int32)(fabs(newYaw))) == ((int32)(fabs(enemyrot.Yaw))))
	{
		WARNINGLOG(TEXT("TurnAtHitDir End!!"));
		return;
	}

	FRotator NewRot{ 0, newYaw ,0 };

	SetActorRotation(NewRot);

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([this, enemyrot]() {TurnAtHitDir_Ver1(enemyrot); });

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.01f, false);
}
