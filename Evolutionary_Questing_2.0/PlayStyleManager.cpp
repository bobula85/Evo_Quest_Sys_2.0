// Fill out your copyright notice in the Description page of Project Settings.

#include <algorithm>

#include "PlayStyleManager.h"

UPlayStyleManager::UPlayStyleManager()
{
	Init();
}

UPlayStyleManager::~UPlayStyleManager()
{
}

// Sets default values
void UPlayStyleManager::Init()
{
	// Multipliers used to change weighting of fitness function elements
	// Could be exposed to the editor for easier modification
	m_dCompMultiplier = 1.1;
	m_dInCompMultiplier = 1.0;
	m_dAbnMultiplier = 0.8;
	m_dRejMultiplier = 1.3;

	int baseValue = 1; 

	// Initiale all player preference statistics to 1
	m_iKill_Com = baseValue;
	m_iGather_Com = baseValue;
	m_iFetch_Com = baseValue;
	m_iExplore_Com = baseValue;

	m_iKill_InCom = baseValue;
	m_iGather_InCom = baseValue;
	m_iFetch_InCom = baseValue;
	m_iExplore_InCom = baseValue;

	m_iKill_Abn = baseValue;
	m_iGather_Abn = baseValue;
	m_iFetch_Abn = baseValue;
	m_iExplore_Abn = baseValue;

	m_iKill_Rej = baseValue;
	m_iGather_Rej = baseValue;
	m_iFetch_Rej = baseValue;
	m_iExplore_Rej = baseValue;

	m_iClose_Com = baseValue;
	m_iMid_Com = baseValue;
	m_iFar_Com = baseValue;

	m_iClose_InCom = baseValue;
	m_iMid_InCom = baseValue;
	m_iFar_InCom = baseValue;

	m_iClose_Abn = baseValue;
	m_iMid_Abn = baseValue;
	m_iFar_Abn = baseValue;

	m_iClose_Rej = baseValue;
	m_iMid_Rej = baseValue;
	m_iFar_Rej = baseValue;


	m_iKill_Fitness = 1;
	m_iGather_Fitness = 1;
	m_iFetch_Fitness = 1;
	m_iExplore_Fitness = 1;
	
	m_iClose_Fitness = 1;
	m_iMid_Fitness = 1;
	m_iFar_Fitness = 1;

	m_v2dPlayerPos = {100.0, 250.0};
}

int UPlayStyleManager::GetFitnessValue(int element)
{
	auto temp = std::find_if(m_aFitnesses.begin(), m_aFitnesses.end(), [element](test entry) { return entry.element == element; } );

	if (temp != m_aFitnesses.end())
		return *temp->fitnessValue;
	else
		return -1;
}

void UPlayStyleManager::CalculateFitness()
{
	// Calculate fitness using player preference statisics eg fitness = (((Completed*?) + Incomplete*?) - Abandoned *? - (Rejected*?))
	// Set to 1 if zero for roulette wheel selection purposes
	m_iKill_Fitness = (((m_iKill_Com * m_dCompMultiplier) + (m_iKill_InCom * m_dInCompMultiplier)) - ((m_iKill_Abn * m_dAbnMultiplier) + (m_iKill_Rej*m_dRejMultiplier)));
	if (m_iKill_Fitness <= 0)
		m_iKill_Fitness = 1;

	m_iGather_Fitness = (((m_iGather_Com * m_dCompMultiplier) + (m_iGather_InCom * m_dInCompMultiplier)) - ((m_iGather_Abn * m_dAbnMultiplier) + (m_iGather_Rej * m_dRejMultiplier)));
	if (m_iGather_Fitness <= 0)
		m_iGather_Fitness = 1;

	m_iFetch_Fitness = (((m_iFetch_Com * m_dCompMultiplier) + (m_iFetch_InCom * m_dInCompMultiplier)) - ((m_iFetch_Abn * m_dAbnMultiplier) + (m_iFetch_Rej * m_dRejMultiplier)));
	if (m_iFetch_Fitness <= 0)
		m_iFetch_Fitness = 1;

	m_iExplore_Fitness = (((m_iExplore_Com * m_dCompMultiplier) + (m_iExplore_InCom * m_dInCompMultiplier)) - ((m_iExplore_Abn * m_dAbnMultiplier) + (m_iExplore_Rej * m_dRejMultiplier)));
	if (m_iExplore_Fitness <= 0)
		m_iExplore_Fitness = 1;
	
	m_iClose_Fitness = (((m_iClose_Com * m_dCompMultiplier) + (m_iClose_InCom * m_dInCompMultiplier)) - ((m_iClose_Abn * m_dAbnMultiplier) + (m_iClose_Rej * m_dRejMultiplier)));
	if (m_iClose_Fitness <= 0)
		m_iClose_Fitness = 1;

	m_iMid_Fitness = (((m_iMid_Com * m_dCompMultiplier) + (m_iMid_InCom * m_dInCompMultiplier)) - ((m_iMid_Abn * m_dAbnMultiplier) + (m_iMid_Rej * m_dRejMultiplier)));
	if (m_iMid_Fitness <= 0)
		m_iMid_Fitness = 1;

	m_iFar_Fitness = (((m_iFar_Com * m_dCompMultiplier) + (m_iFar_InCom * m_dInCompMultiplier)) - ((m_iFar_Abn * m_dAbnMultiplier) + (m_iFar_Rej * m_dRejMultiplier)));
	if (m_iFar_Fitness <= 0)
		m_iFar_Fitness = 1;


}