// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletObjectPool.h"

// Sets default values
ABulletObjectPool::ABulletObjectPool()
{
	UE_LOG(LogTemp, Log, TEXT("Create Bullet Object Pool"));

	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder(TEXT("/Script/Engine.Blueprint'/Game/Objects/BulletModel.BulletModel'")); // �̸� �������� bullet �������Ʈ Ŭ������ FObjectFinder�Լ��� �̿��� Ž��
	if (blueprint_finder.Succeeded())
		bulletBluePrint = (UClass*)blueprint_finder.Object->GeneratedClass; //Ž�� �� bulletBulePrint ������ �Ҵ�
}

// Called when the game starts or when spawned
void ABulletObjectPool::BeginPlay()
{
	Super::BeginPlay();

	bulletPool.Empty();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; // ������ bullet�� �����ڸ� ���� ���ͷ� ����, 
	for (int32 i = 0; i < poolSize; i++) //bullet ��� ����Ʈ Ŭ������ ���� poolSize��ŭ bullet ������Ʈ �̸� ����
	{
		AActor* bullet = GetWorld()->SpawnActor<AActor>(bulletBluePrint, SpawnParams);
		if (bullet)
		{
			//bullet ���͸� ��Ȱ��ȭ�ϱ� ���� �Ʒ� �� ���� �Լ��� ���� (������ x, �浹 x, Tick�Լ� x)
			bullet->SetActorEnableCollision(false);
			bullet->SetActorTickEnabled(false);
			bullet->SetActorHiddenInGame(true);
			bulletPool.Add(bullet); //������ bullet�� bulletPool Array�� �߰�
			UE_LOG(LogTemp, Log, TEXT("Object Pool : Create Bullet %d"), i);
		}
	}
}

AActor* ABulletObjectPool::GetBullet() //Bullet ������Ʈ ��������
{	
	for (AActor* bullet : bulletPool)
	{
		bullet->SetActorHiddenInGame(false);
		bullet->SetActorTickEnabled(true);
		bullet->SetActorEnableCollision(true);
		if (bullet->GetActorLocation() == FVector(0, 0, 0)) // �Ⱦ��� ź
		{
			UE_LOG(LogTemp, Log, TEXT("Get Bullet"));
			return bullet;
		}
	}

	return nullptr;
}

void ABulletObjectPool::ReturnBullet(AActor* bullet) //����� Bullet ������Ʈ ��ȯ
{
	if (bullet)
	{
		bullet->SetActorTickEnabled(false);
		bullet->SetActorEnableCollision(false);
		bullet->SetActorHiddenInGame(true);
		bullet->SetActorLocation(FVector(0, 0, 0));
		UE_LOG(LogTemp, Log, TEXT("Return Bullet"))
	}
}


