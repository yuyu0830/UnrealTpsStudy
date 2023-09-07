// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletObjectPool.generated.h"

UCLASS()
class FPSSTUDY_API ABulletObjectPool : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABulletObjectPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		AActor* GetBullet();

	UFUNCTION()
		void ReturnBullet(AActor* bullet);

	UPROPERTY()
		int32 poolSize = 30;

private:
	UPROPERTY()
		TArray<AActor*> bulletPool; //미리 생성한 오브젝트를 담을 Array

	UPROPERTY()
		UClass* bulletBluePrint = nullptr;
};
