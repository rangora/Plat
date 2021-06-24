// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"


/**
 * 
 */

class AWorldMap;


 class FMapWorker : public FRunnable {
 public:
     FMapWorker(AWorldMap* aMap);
     ~FMapWorker();
    

 	virtual bool Init() override;
 	virtual uint32 Run() override;
 	virtual void Stop() override;
    
    FMapWorker* InitWrapper(AWorldMap* aMap);
    

 private:
     void ThreadCreateTerrain();
     void ThreadCreateTerrainPartial(FVector Where, int pivotIndex);

     /* perlin noise */
	 float fade(float t);
	 float noise(float x, float y);
	 float octave(int x, int y, int numOfOctaves, float noiseScale);

 public:
    bool bJobDone = false;

 private:
    FRunnableThread* pThread;
    
    FThreadSafeCounter StopTaskCounter;
    
    AWorldMap* CurrentMap = nullptr;
 	
    bool bRun;
 };



