// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "UObject/NoExportTypes.h"
#include "MapWorker.generated.h"

/**
 * 
 */
UCLASS()
class PLAT_API UMapWorker : public UObject
{
	GENERATED_BODY()

};

//class TMapWorker : public FRunnable {
//public:
//	TMapWorker();
//
//	virtual bool Init() override;
//	virtual uint32 Run() override;
//	virtual void Stop() override;
//
//
//private:
//	static UMapWorker* pInstance;
//	
//	FRunnableThread* RunnableThread;
//	
//	bool bRun;
//
//
//
//};


