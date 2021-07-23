// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestManager.h"

#include"myMaths.h"


UQuestManager::UQuestManager()
{
	InitDefaults();
}


UQuestManager::~UQuestManager()
{

}

void UQuestManager::InitDefaults()
{
	// Initialise far

	UQuest* far_kill		= new UQuest(DIST_FAR, TP_KILL, "Kill Far");
	UQuest* far_gather		= new UQuest(DIST_FAR, TP_GATHER, "Gather Far");
	UQuest* far_fetch		= new UQuest(DIST_FAR, TP_FETCH, "Fetch Far");
	UQuest* far_explore		= new UQuest(DIST_FAR, TP_EXPLORE, "Explore Far");

	// Initialise mid

	UQuest* mid_kill		= new UQuest(DIST_MID, TP_KILL, "Kill Mid");	
	UQuest* mid_gather		= new UQuest(DIST_MID, TP_GATHER, "Gather Mid");	
	UQuest* mid_fetch		= new UQuest(DIST_MID, TP_FETCH, "Fetch Mid");	
	UQuest* mid_explore		= new UQuest(DIST_MID, TP_EXPLORE, "Explore Mid");

	// Initialise close

	UQuest* close_kill		= new UQuest(DIST_CLOSE, TP_KILL, "Kill Close");
	UQuest* close_gather	= new UQuest(DIST_CLOSE, TP_GATHER, "Gather Close");
	UQuest* close_fetch		= new UQuest(DIST_CLOSE, TP_FETCH, "Fetch Close");
	UQuest* close_explore	= new UQuest(DIST_CLOSE, TP_EXPLORE, "Explore Close");

	vpPopulation.push_back(far_kill);
	vpPopulation.push_back(far_gather);
	vpPopulation.push_back(far_fetch);
	vpPopulation.push_back(far_explore);

	vpPopulation.push_back(mid_kill);
	vpPopulation.push_back(mid_gather);
	vpPopulation.push_back(mid_fetch);
	vpPopulation.push_back(mid_explore);

	vpPopulation.push_back(close_kill);
	vpPopulation.push_back(close_gather);
	vpPopulation.push_back(close_fetch);
	vpPopulation.push_back(close_explore);

	for (UQuest* quest : vpPopulation)
		quest->Init(quest->m_eDistBand, quest->m_eType, quest->m_sQuestName);
}

std::string UQuestManager::getTypeName(int typ)
{
	// Set quest name vaue based on type
	switch (typ)
	{
	case TP_KILL:
		return "Kill ";
		break;
	case TP_GATHER:
		return "Gater ";
		break;
	case TP_FETCH:
		return "Fetch ";
		break;
	case TP_EXPLORE:
		return "Explore ";
		break;
	}

	return "none";
}

vector2D UQuestManager::RandQuestPos(int distBounds)
{
	int minRadius = 0;
	int maxRadius = 0;

	switch (distBounds)
	{
	case DIST_CLOSE:
	{
		minRadius = 0;
		maxRadius = 1000;

		break;
	}
	case DIST_MID:
	{
		minRadius = 1000;
		maxRadius = 2000;

		break;
	}
	case DIST_FAR:
	{
		minRadius = 2000;
		maxRadius = 4000;

		break;
	}
	}

	// Generate random angle
	int randAngle = rand() % 360+1;

	// Generate random distance from point
	int randOffset = minRadius + (rand() % static_cast<int>(maxRadius - minRadius + 1));

	// Get x and y coordiates based on angle and offest dist
	int newX = randOffset * cos(randAngle); // FMath::RandRange(-2000, 2000);
	int newY = randOffset * sin(randAngle); // FMath::RandRange(-2000, 2000);

	vector2D newVector(newX, newY);

	return newVector;
}





