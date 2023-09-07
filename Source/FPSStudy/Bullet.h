// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "BulletObjectPool.h"
#include "Bullet.generated.h"

UCLASS()
class FPSSTUDY_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void BulletInit(AActor* player, FRotator offset);

	UFUNCTION()
		void DisableBullet();

	UPROPERTY()
		AActor* shooter = nullptr;

	UPROPERTY()
		FVector forward;

private:

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* colliderComponent;

	UPROPERTY(VisibleAnywhere)
		float BulletSpeed = 1000.0f;

	UPROPERTY()
		ABulletObjectPool* myPool = nullptr;

	UPROPERTY()
		FTimerHandle lifespan;
};