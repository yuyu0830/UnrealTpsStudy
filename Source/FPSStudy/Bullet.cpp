// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	colliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	RootComponent = colliderComponent;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	colliderComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	myPool = Cast<ABulletObjectPool>(this->GetOwner());
}

void ABullet::BulletInit(AActor* player, FRotator rotation)
{
	GetWorldTimerManager().SetTimer(lifespan, this, &ABullet::DisableBullet, 3.0, false);
	shooter = player;

	// Set Fire Angle
	FRotator fireAngle = player->GetActorRotation();

	fireAngle.Yaw += rotation.Yaw + 1.f;
	fireAngle.Pitch += rotation.Pitch - 2.5f;

	FVector angle = fireAngle.Vector();

	forward = angle;


	// Set Fire Location
	FVector fireLocation = player->GetActorLocation();

	fireLocation.Z += 40.f;
	
	SetActorLocation(fireLocation + angle * 120);
	//forward.Normalize();
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (shooter)
		AddActorLocalOffset(forward * BulletSpeed * DeltaTime);
}

void ABullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == shooter) return;
	UE_LOG(LogTemp, Log, TEXT("Bullet Collision Detected"));
	GetWorldTimerManager().ClearTimer(lifespan);
	DisableBullet();
}

void ABullet::DisableBullet()
{
	shooter = nullptr;
	forward = FVector(0, 0, 0);
	myPool->ReturnBullet(this);
	UE_LOG(LogTemp, Log, TEXT("Bullet Disabled"));
}