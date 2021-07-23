// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>

#include "QuestManager.h"
#include "myMaths.h"

struct test
{
	QuestElements element;
	int* fitnessValue;
};

class UPlayStyleManager
{

public:	

	UPlayStyleManager();

	~UPlayStyleManager();

	vector2D& GetPlayerPos() { return m_v2dPlayerPos; }

	//! A function which initialises the play style manager.
	/*!

	*/
	void Init();

	//! A function which updates the players play style.
	/*!

	*/
	void CalculateFitness();

	int GetFitnessValue(int element);

	////////////////////////////	KILL STATS	////////////////////////////////////////////////

	int m_iKill_Com;		/*!< An integer value representing amount of kill quests completed */
	int m_iKill_InCom;		/*!< An integer value representing amount of kill quests incomplete */
	int m_iKill_Abn;		/*!< An integer value representing amount of kill quests abandoned */
	int m_iKill_Rej;		/*!< An integer value representing amount of kill quests rejected */

	////////////////////////////	GATHER STATS	////////////////////////////////////////////////

	int m_iGather_Com;		/*!< An integer value representing amount of gather quests completed */
	int m_iGather_InCom;	/*!< An integer value representing amount of gather quests incomplete */
	int m_iGather_Abn;		/*!< An integer value representing amount of gather quests abandoned */
	int m_iGather_Rej;		/*!< An integer value representing amount of gather quests rejected */

	////////////////////////////	FETCH STATS	////////////////////////////////////////////////

	int m_iFetch_Com;		/*!< An integer value representing amount of fetch quests completed */
	int m_iFetch_Abn;		/*!< An integer value representing amount of fetch quests abandoned */
	int m_iFetch_InCom;		/*!< An integer value representing amount of fetch quests incomplete */
	int m_iFetch_Rej;		/*!< An integer value representing amount of fetch quests rejected */

	////////////////////////////	EXPLORE STATS	////////////////////////////////////////////////

	int m_iExplore_Com;		/*!< An integer value representing amount of fetch quests completed */
	int m_iExplore_InCom;	/*!< An integer value representing amount of fetch quests incomplete */
	int m_iExplore_Abn;		/*!< An integer value representing amount of fetch quests abandoned */
	int m_iExplore_Rej;		/*!< An integer value representing amount of explore quests rejected */

	////////////////////////////	CLOSE STATS		////////////////////////////////////////////////

	int m_iClose_Com;		/*!< An integer value representing amount of close quests complete */
	int m_iClose_InCom;		/*!< An integer value representing amount of close quests incomplete */
	int m_iClose_Abn;		/*!< An integer value representing amount of close quests abandoned */
	int m_iClose_Rej;		/*!< An integer value representing amount of close quests rejected */

	////////////////////////////	MID STATS		////////////////////////////////////////////////

	int m_iMid_Com;			/*!< An integer value representing amount of mid quests complete */
	int m_iMid_InCom;		/*!< An integer value representing amount of mid quests incomplete */
	int m_iMid_Abn;			/*!< An integer value representing amount of mid quests abandoned */
	int m_iMid_Rej;			/*!< An integer value representing amount of mid quests rejected */

	////////////////////////////	FAR STATS		////////////////////////////////////////////////

	int m_iFar_Com;			/*!< An integer value representing amount of far quests complete */
	int m_iFar_InCom;		/*!< An integer value representing amount of far quests incomplete */
	int m_iFar_Abn;			/*!< An integer value representing amount of far quests abandoned */
	int m_iFar_Rej;			/*!< An integer value representing amount of far quests rejected */

	////////////////////////////	FITNESS VALUES		////////////////////////////////////////////////

	int m_iKill_Fitness;		/*!< An integer value representing the final kill fitness */
	int m_iGather_Fitness;		/*!< An integer value representing the final gather fitness */
	int m_iFetch_Fitness;		/*!< An integer value representing the final fetch fitness */
	int m_iExplore_Fitness;		/*!< An integer value representing the final fetch fitness */
	int m_iClose_Fitness;		/*!< An integer value representing the final close fitness */
	int m_iMid_Fitness;			/*!< An integer value representing the final mid fitness */
	int m_iFar_Fitness;			/*!< An integer value representing the final far fitness */

	// could be exposed to the unreal editor for easy modification

	double m_dCompMultiplier;		/*!< A multiplier of type double used in the completed quests part of the fitness calculation */
	double m_dInCompMultiplier;		/*!< A multiplier of type double used in the incomplete quests part of the fitness calculation */
	double m_dAbnMultiplier;		/*!< A multiplier of type double used in the abandoned quests part of the fitness calculation */
	double m_dRejMultiplier;		/*!< A multiplier of type double used in the rejected quests part of the fitness calculation */

	vector2D m_v2dPlayerPos;

	std::vector<test> m_aFitnesses = {  test{ELEMENT_KILL,		&m_iKill_Fitness},
										test{ELEMENT_GATHER,	&m_iGather_Fitness},
										test{ELEMENT_FETCH,		&m_iFetch_Fitness},
										test{ELEMENT_EXPLORE,	&m_iExplore_Fitness},
										test{ELEMENT_CLOSE,		&m_iClose_Fitness},
										test{ELEMENT_MID,		&m_iMid_Fitness},
										test{ELEMENT_FAR,		&m_iFar_Fitness}	};
};
