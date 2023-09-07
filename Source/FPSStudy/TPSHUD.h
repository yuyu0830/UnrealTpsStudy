// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TPSHUD.generated.h"

/**

*/
UCLASS()
class FPSSTUDY_API ATPSHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly)
        UTexture2D* CrosshairTexture;

public:
    virtual void DrawHUD() override;
};