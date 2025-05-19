// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/GM_InGame.h"
#include "PC_InGame.h"
AGM_InGame::AGM_InGame()
{
	PlayerControllerClass = APC_InGame::StaticClass();
}
