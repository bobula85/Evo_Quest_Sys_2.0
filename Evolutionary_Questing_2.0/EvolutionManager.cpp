// Fill out your copyright notice in the Description page of Project Settings.

#include <map>
#include <math.h>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <ranges>

#include "EvolutionManager.h"
#include "QuestManager.h"
#include "Quest.h"
#include "PlayStyleManager.h"
#include "MyUtils.h"

#include "MyAlgos.h"



//Compile options for debug and logging
//#define TIMING_ON
//#define ONSCREEN_DEBUG_ENABLED 
//#define LOGGING_ENABLED


UEvolutionManager::UEvolutionManager()
	:m_pQuestMgr(0), m_pPlayer(0), m_iMaxPairs(0), m_iMaxChildren(0), m_iMaxPopulation(0), m_iReintroRate(0), m_iEvolutionNumber(0)
{

}


UEvolutionManager::UEvolutionManager(UQuestManager* questMgr, UPlayStyleManager* player, int maxPairs, int maxChildren, int maxPopulation, int evolutionNumber, int reintroRate)
	: m_pQuestMgr(questMgr), m_pPlayer(player), m_iMaxPairs(maxPairs), m_iMaxChildren(maxChildren), m_iMaxPopulation(maxPopulation), m_iReintroRate(reintroRate), m_iEvolutionNumber(evolutionNumber)
{

}


UEvolutionManager::~UEvolutionManager()
{

}

union example
{
	short test[3];
	int i;
};

// Initiate the various stage of the evolutionary process
void UEvolutionManager::EvolveQuests()
{
	int s = sizeof(example);

	// Update previous population stats
	m_PreviousFreqInfoSel = {};
	m_PreviousFreqInfoFin = {};

	m_PreviousFreqInfoSel = m_CurrentFreqInfoSel;
	m_PreviousFreqInfoFin = m_CurrentFreqInfoFin;
	
	// Use the player statistics to calculate the optimum fitness values
	{

#ifdef TIMING_ON
		Timer timer("CalculateFitness");
#endif

		m_pPlayer->CalculateFitness();
	}

	{
#ifdef TIMING_ON
		Timer timer("Evaluate");
#endif

		// Evaluate all current population solutions based on player statistics
		for(UQuest *quest : m_pQuestMgr->vpPopulation)
			Evaluate(quest);
	}

	{
#ifdef TIMING_ON
		Timer timer("RouletteSelection");
#endif

		// Initiate the selection of the parent solutions
		// using roulette wheel selection
		SelectRoulette();
	}

	//{
	//	std::cout << "RouletteRanked" << std::endl;
	//	Timer timer;

		// Initiate the selection of the parent solutions
		// using roulette wheel selection
		//SelectRanked();
	//}

	{
#ifdef TIMING_ON
		Timer timer("Combination");
#endif

		// Initiate the creation of the child solutions
		// using parents selected
		Combine();
	}

	{
#ifdef TIMING_ON
		Timer timer("Mutation");
#endif

		// Initiate the mutation function based on population stats of the child solutions
		Mutate();
	}

	{
#ifdef LOGGING_ENABLED
		Timer timer("Survival");
#endif

		// Initiate the selection of the next generation of solutions
		Survive();
	}

	// Recalculate population Stat data
	DisplayChangeSel();
	DisplayChangeFin();

#ifdef LOGGING_ENABLED

	// Savle the created solutions information to
	// a .csv file
	//OutputResult();

#endif

	// Increment the number of evolutions that has taken place
	m_iEvolutionNumber++;
}

void UEvolutionManager::Evaluate(UQuest* qst)
{
	qst->m_iTypeElementFitness = m_pPlayer->GetFitnessValue(qst->m_eTypeElement);
	qst->m_iDistElementFitness = m_pPlayer->GetFitnessValue(qst->m_eDistElement);

	qst->m_iTotalFitness = qst->m_iTypeElementFitness + qst->m_iDistElementFitness;

	if (qst->m_iTotalFitness <= 0)
		qst->m_iTotalFitness = 1;
}

// Applies the roulette wheel selection method to select parent solutions
void UEvolutionManager::SelectRoulette()
{
	// Empty the parents vector
	m_aParents.clear();

	// Create a vecor to hold the sum of fitness values of selection
	std::vector<int> tempVec;

	// Hold the sum of fitness total value
	int sumOfFitness = 0;

	// Loop 1
	// Loop through the population and increment the sum of fitness values accordingly
	for (int i = 0; i < m_pQuestMgr->vpPopulation.size(); i++)
	{
		if (i == 0)
			tempVec.push_back(m_pQuestMgr->vpPopulation[i]->m_iTotalFitness);
		else
			tempVec.push_back(tempVec[i - 1] + m_pQuestMgr->vpPopulation[i]->m_iTotalFitness);

		sumOfFitness += (m_pQuestMgr->vpPopulation[i]->m_iTotalFitness);
	}

	// Loop 2
	// Loop through the population and select maxPairs of parent solutions 
	for (int i = 0; i < m_iMaxPairs; i++)
	{
		// Generate random number between 1 and total fitness for both parents
		int r1 = rand() % sumOfFitness + 1;
		int r2 = rand() % sumOfFitness + 1;

		// Create pair struct to hold both parents as one object
		FPair parentPair;

		//Loop 2.1
		// If the random number generated falls within the probaility value of
		// a population solution then add it to the pair object as one of the parents.
		for (int j = 0; j < tempVec.size(); j++)
		{
			if (tempVec[j] >= r1)
			{
				parentPair.parent1 = m_pQuestMgr->vpPopulation[j];
				break;
			}
		}

		// Loop 2.2
		for (int j = 0; j < tempVec.size(); j++)
		{
			if (tempVec[j] >= r2)
			{
				parentPair.parent2 = m_pQuestMgr->vpPopulation[j];
				break;
			}
		}

		if (parentPair.parent1->CompareID(parentPair.parent2))
			i--;
		else
			// add the parent pair to the parent vector
			m_aParents.push_back(parentPair);
	}
}


struct fitnessCompClass {
	bool operator() (UQuest* a, UQuest* b) { return (a->m_iTotalFitness < b->m_iTotalFitness); }
} fitnessCompObj;


// Not yet used. May be used in the next unit
void UEvolutionManager::SelectRanked()
{
	// Empty the parents vector
	m_aParents.clear();

	// Array to hold the population sorted by fitness
	std::vector<UQuest*> tempVecSortedPop;

	// Loop 1
	// Add all quests to the sort array
	for (int i = 0; i < m_pQuestMgr->vpPopulation.size(); i++)
		tempVecSortedPop.push_back(m_pQuestMgr->vpPopulation[i]);

	// Sort the array using a basic lambda function
	std::sort(tempVecSortedPop.begin() + 4, tempVecSortedPop.end(), fitnessCompObj);

	// calculates the total sum of all the ranked positions in the array (eg an array of 4 is 10 as 1+2+3+4 = 10)
	double sumOfRanks = ((double)tempVecSortedPop.size() + 1) * ((double)tempVecSortedPop.size() / 2);

	std::vector<double> probs;

	// Loop 2
	for (int i = 0; i < tempVecSortedPop.size(); i++)
	{
		//calculates raks probablility i = rank
		double prob = ((i+1) / sumOfRanks) * 100;
		probs.push_back(prob);
	}

	// Array to hold the stacked probaility of selection values
	std::vector<double> stackedProbabilities;

	// variable to hold the sum of all of the probabilities (should be 100%)
	int sumOfRankedFitness = 0;

	// Loop 3
	// Loop through the population and increment the sum of fitness values accordingly
	// so that the probabilities stack ready to be selected from eq |  prob1 |   prob2 = prob1 +prob2     |		prob3 = prob2 + prob3			| etc
	for (int i = 0; i < probs.size(); i++)
	{
		if (i == 0)
			stackedProbabilities.push_back(probs[i]);
		else
			// |  prob1 |   prob2 = prob1 +prob2     |		prob3 = prob2 + prob3			| etc
			stackedProbabilities.push_back(stackedProbabilities[i - 1] + probs[i]);

		// should be 100%
		sumOfRankedFitness += static_cast<int>(probs[i]);
	}

	// Loop 4
	// Loop though maxPairs to select all parent pairs
	for (int i = 0; i < m_iMaxPairs; i++)
	{
		// Generate a random number for each parent selection between 1 and sum of fitness (should be 100)
		int r1 = rand() % sumOfRankedFitness + 1;
		int r2 = rand() % sumOfRankedFitness + 1;

		FPair parentPair{};

		// Loop 4.1
		for (int j = 0; j < stackedProbabilities.size(); j++)
		{	
			// If the random number generated falls within the probaility value of
			// a population solution then add it to the pair object as one of the parents.
			if (stackedProbabilities[j] >= (double)r1)
			{
				parentPair.parent1 = m_pQuestMgr->vpPopulation[j];
				break;
			}
		}

		// Loop 4.2
		// If the random number generated falls within the probaility value of
		// a population solution then add it to the pair object as one of the parents.
		for (int j = 0; j < stackedProbabilities.size(); j++)
		{
			if (stackedProbabilities[j] >= (double)r2)
			{
				parentPair.parent2 = m_pQuestMgr->vpPopulation[j];
				break;
			}
		}

		// if parents are the same solution then repeat the selection process
		if (parentPair.parent1 == parentPair.parent2)
			i--;
		else
			// add the parent pair to the parent vector
			m_aParents.push_back(parentPair);
	}

}

void UEvolutionManager::Combine()
{
	m_aChildren.clear();

	// Loop 1
	for (int i = 0; i < m_aParents.size(); i++)
	{
		// Create the two new children
		UQuest* child1 = new UQuest();
		UQuest* child2 = new UQuest();
		
		m_aParents[i].child1 = child1;
		m_aParents[i].child2 = child2;

		// Initialise children quests using elements of the parents
		child1->Init(m_aParents[i].parent1->m_eDistBand, m_aParents[i].parent2->m_eType, "Quest? 1");
		child2->Init(m_aParents[i].parent2->m_eDistBand, m_aParents[i].parent1->m_eType, "Quest? 2");


		// Update child solutions distance, name and description information
		child1->SetDistanceFromPlayer(m_pPlayer->GetPlayerPos());
		child2->SetDistanceFromPlayer(m_pPlayer->GetPlayerPos());
		child1->setDistName();
		child2->setDistName();

		child1->setQuestName();
		child2->setQuestName();

		child1->setDescription();
		child2->setDescription();

		// Add children to the children vector
		m_aChildren.push_back(child1);
		m_aChildren.push_back(child2);
	}
}

void UEvolutionManager::CalculateFrequencySel(Ffrequency_Count* count)
{
	// Loop 1
	// Loop which caounts the the amount of each quest type element in the current population
	for (int i = 0; i < m_aChildren.size(); i++)
	{
		switch (m_aChildren[i]->m_eType)
		{
		case TP_KILL:
			count->KillCount++;
			break;
		case TP_GATHER:
			count->GatherCount++;
			break;
		case TP_FETCH:
			count->FetchCount++;
			break;
		case TP_EXPLORE:
			count->ExploreCount++;
			break;
		}

		switch (m_aChildren[i]->m_eDistBand)
		{
		case DIST_CLOSE:
			count->CloseCount++;
			break;
		case DIST_MID:
			count->MidCount++;
			break;
		case DIST_FAR:
			count->FarCount++;
			break;
		}
	}

	// Calculate the amount each type element appears in the population as a percentage
	count->KillFreq		= ((float)count->KillCount	/ (float)m_aChildren.size()) * 100;
	count->GatherFreq	= ((float)count->GatherCount/ (float)m_aChildren.size()) * 100;
	count->FetchFreq	= ((float)count->FetchCount	/ (float)m_aChildren.size()) * 100;
	count->ExploreFreq	= ((float)count->ExploreCount/ (float)m_aChildren.size()) * 100;

	// Add the type frequencies to an array for later use
	count->TypeFreqs.push_back(count->KillFreq);
	count->TypeFreqs.push_back(count->GatherFreq);
	count->TypeFreqs.push_back(count->FetchFreq);
	count->TypeFreqs.push_back(count->ExploreFreq);

	// Check for any quests type frequencies of zero. This indicates aloss of genetic material from the population
	if (count->KillFreq == 0.0 || count->GatherFreq == 0.0 || count->FetchFreq == 0.0 || count->ExploreFreq == 0.0)
	{
		// Add any lost type enum values to a list for the mutation function to use later
		if (count->KillFreq == 0.0)
			count->LowTypeFreqs.push_back((int)TP_KILL);
		if (count->GatherFreq == 0.0)
			count->LowTypeFreqs.push_back((int)TP_GATHER);
		if (count->FetchFreq == 0.0)
			count->LowTypeFreqs.push_back((int)TP_FETCH);
		if (count->ExploreFreq == 0.0)
			count->LowTypeFreqs.push_back((int)TP_EXPLORE);

		// Set low frequency to true so that mutation is triggered
		count->lowTypeFreq = true;
	}

	// Calculate the amount each distance element appears in the population as a percentage
	count->CloseFreq	= ((float)count->CloseCount	/ (float)m_aChildren.size())*100;
	count->MidFreq		= ((float)count->MidCount	/ (float)m_aChildren.size())*100;
	count->FarFreq		= ((float)count->FarCount	/ (float)m_aChildren.size())*100;
	
	// Add the type frequencies to an array for later use
	count->DistFreqs.push_back(count->CloseFreq);
	count->DistFreqs.push_back(count->MidFreq);
	count->DistFreqs.push_back(count->FarFreq);

	// Check for any quests distance frequencies of zero. This indicates aloss of genetic material from the population
	if (count->CloseFreq == 0.0 || count->MidFreq == 0.0 || count->FarFreq == 0.0)
	{
		// Add any lost distance enum values to a list for the mutation function to use later
		if (count->CloseFreq == 0.0)
			count->LowDistFreqs.push_back((int)DIST_CLOSE);
		if (count->MidFreq == 0.0)
			count->LowDistFreqs.push_back((int)DIST_MID);
		if (count->FarFreq == 0.0)
			count->LowDistFreqs.push_back((int)DIST_FAR);

		// Set low frequency to true so that mutation is triggered
		count->lowDistFreq = true;
	}
}

void UEvolutionManager::Mutate()
{
	// Create a container to hold the population frequency information
	m_CurrentFreqInfoSel = {};

	// Get frequency information 
	CalculateFrequencySel(&m_CurrentFreqInfoSel);

	// if an element type is missing add it back in as below
	if (m_CurrentFreqInfoSel.lowTypeFreq || m_CurrentFreqInfoSel.lowDistFreq)
	{
		// misssing type element
		if (m_CurrentFreqInfoSel.lowTypeFreq)
		{
			// Loop 1
			// for each type element that is missing 
			for (int i = 0; i < m_CurrentFreqInfoSel.LowTypeFreqs.size(); i++)
			{
				// Loop 1.1
				// add in 5 random ones
				for (int j = 0; j < m_iReintroRate; j++)
				{
					//generate a random index to change
					int randIndex = rand() % m_aChildren.size();

					// set it's type to the missing type
					m_aChildren[randIndex]->setType((QuestType)m_CurrentFreqInfoSel.LowTypeFreqs[i]);

					m_aChildren[randIndex]->setQuestName();
					m_aChildren[randIndex]->setDescription();
				}
			}
		}
		// missing distance 
		if (m_CurrentFreqInfoSel.lowDistFreq)
		{	
			// Loop 2
			// for each distance banding that is missing
			for (int i = 0; i < m_CurrentFreqInfoSel.LowDistFreqs.size(); i++)
			{
				int count = 0;
				
				int minRadius = 0;
				int maxRadius = 0; 

				//set the random number generater bounding based on missing dist type
				switch(m_CurrentFreqInfoSel.LowDistFreqs[i])
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

				// Loop 2.1
				while (count < m_iReintroRate)
				{
					// Generate random index 
					int randIndex = rand() % m_aChildren.size();

					// Generate random angle
					int randAngle = rand() % 360+1;

					// Generate random distance from point
					int randOffset = minRadius + (rand() % static_cast<int>(maxRadius - minRadius + 1));

					vector2D newVec((float)randOffset * cos(randAngle), (float)randOffset * sin(randAngle));

					// set x and y cords
					m_aChildren[randIndex]->setLocation(newVec);
					

					// Update quest location data
					m_aChildren[randIndex]->SetDistanceFromPlayer(m_pPlayer->GetPlayerPos());

					m_aChildren[randIndex]->setDistName();
					m_aChildren[randIndex]->setQuestName();
					m_aChildren[randIndex]->setDescription();

					count++;
				}
			}
		}
	}

}

void UEvolutionManager::Survive()
{
	//TO_ADD! make into a std map as it is presorted using balanced tree!!
	// Temperary vector used to hold what will be the next generation of solutions
	std::vector<UQuest*> tempNewPop;

	// Loop 1 
	// Evaluate the child solution so that all fittnesses are updated
	for (int i = 0; i < m_aChildren.size(); i++)
		Evaluate(m_aChildren[i]);

	// Loop 2
	// Loop through the current population and include quests accepted but 
	// not completed in the next generation by default
	for (int i = 0; i < m_pQuestMgr->vpPopulation.size(); i++)
	{
		if (m_pQuestMgr->vpPopulation[i]->m_eStatus == ST_ACCEPTED)
			tempNewPop.push_back(m_pQuestMgr->vpPopulation[i]);
	}

	// Array to hold the population sorted by fitness
	std::vector<UQuest*> tempSortPop;

	// Loop 3
	// Add all children to the sort array
	for (int i = 0; i < m_aChildren.size(); i++)
		tempSortPop.push_back(m_aChildren[i]);

	// Sort the array using a basic lambda function
	//tempSortPop.Sort([](const UQuest& a, const UQuest& b) { return a.m_iTotalFitness < b.m_iTotalFitness; });

	// calculates the total sum of all the ranked positions in the array (eg an array of 4 is 10 as 1+2+3+4 = 10)
	double sumOfRanks = ((double)tempSortPop.size() + 1) * ((double)tempSortPop.size() / 2);

	std::vector<double> probs;

	// Loop 4
	for (int i = 0; i < tempSortPop.size(); i++)
	{
		//calculates ranks probablility i = rank
		double prob = ((i + 1) / sumOfRanks) * 100;
		probs.push_back(prob);
	}

	std::vector<double> stackedProbabilities;
	double sumOfRankedFitness = 0;

	// Loop 5
	// Loop through the population and increment the sum of fitness values accordingly
	for (int i = 0; i < probs.size(); i++)
	{
		if (i == 0)
			stackedProbabilities.push_back(probs[i]);
		else
			stackedProbabilities.push_back(stackedProbabilities[i - 1] + probs[i]);

		sumOfRankedFitness += probs[i];
	}

	// Loop 6
	// Loop though the remaining space in the next generation and select at random
	// the children that will be added based on their fitness proportionate probailities
	while(tempNewPop.size() < m_iMaxPopulation)
	{
		int r1 = rand() % 100 + 1;

		for (int j = 0; j < stackedProbabilities.size(); j++)
		{
			if (stackedProbabilities[j] >= (double)r1)
			{
				int addedCheck = tempNewPop.size();

				//TO_ADD
				// Add a hashmap!! If it is already in the map than dont add it!
				tempNewPop.push_back(tempSortPop[j]);
				tempSortPop[j]->m_bChosen = true;

				break;
			}
		}
	}

	// Empty the quest managers population list
	m_pQuestMgr->vpPopulation.clear();

	// Loop 7
	// Add the new generation to the quest manager
	for (int i = 0; i < tempNewPop.size(); i++)
		m_pQuestMgr->vpPopulation.push_back(tempNewPop[i]);
}

void UEvolutionManager::CalculateFrequencyFin(Ffrequency_Count* count)
{
	// Loop 1
	// Loop which caounts the the amount of each quest type element in the current population
	for (int i = 0; i < m_pQuestMgr->vpPopulation.size(); i++)
	{
		switch (m_pQuestMgr->vpPopulation[i]->m_eType)
		{
		case TP_KILL:
			count->KillCount++;
			break;
		case TP_GATHER:
			count->GatherCount++;
			break;
		case TP_FETCH:
			count->FetchCount++;
			break;
		case TP_EXPLORE:
			count->ExploreCount++;
			break;
		}

		switch (m_pQuestMgr->vpPopulation[i]->m_eDistBand)
		{
		case DIST_CLOSE:
			count->CloseCount++;
			break;
		case DIST_MID:
			count->MidCount++;
			break;
		case DIST_FAR:
			count->FarCount++;
			break;
		}
	}

	// Calculate the amount each type element appears in the population as a percentage
	count->KillFreq = ((float)count->KillCount / (float)m_pQuestMgr->vpPopulation.size()) * 100;
	count->GatherFreq = ((float)count->GatherCount / (float)m_pQuestMgr->vpPopulation.size()) * 100;
	count->FetchFreq = ((float)count->FetchCount / (float)m_pQuestMgr->vpPopulation.size()) * 100;
	count->ExploreFreq = ((float)count->ExploreCount / (float)m_pQuestMgr->vpPopulation.size()) * 100;

	// Add the type frequencies to an array for later use
	count->TypeFreqs.push_back(count->KillFreq);
	count->TypeFreqs.push_back(count->GatherFreq);
	count->TypeFreqs.push_back(count->FetchFreq);
	count->TypeFreqs.push_back(count->ExploreFreq);

	// Calculate the amount each type element appears in the population as a percentage
	count->CloseFreq = ((float)count->CloseCount / (float)m_pQuestMgr->vpPopulation.size()) * 100;
	count->MidFreq = ((float)count->MidCount / (float)m_pQuestMgr->vpPopulation.size()) * 100;
	count->FarFreq = ((float)count->FarCount / (float)m_pQuestMgr->vpPopulation.size()) * 100;

	// Add the type frequencies to an array for later use
	count->DistFreqs.push_back(count->CloseFreq);
	count->DistFreqs.push_back(count->MidFreq);
	count->DistFreqs.push_back(count->FarFreq);
}

void UEvolutionManager::DisplayChangeSel()
{
	if (m_PreviousFreqInfoSel.TypeFreqs.size() == 0)
		return;

	std::vector<float> percentIncrease;

	// Loop 1
	for (int i = 0; i < m_CurrentFreqInfoSel.TypeFreqs.size(); i++)
	{
		// Compare current and previous frequency data to get the increase amount
		float increase = m_CurrentFreqInfoSel.TypeFreqs[i] - m_PreviousFreqInfoSel.TypeFreqs[i];

		// Calculate the increase as a percentage
		float percentInc = (increase / m_CurrentFreqInfoSel.TypeFreqs[i]) * 100;

		// Add the percentage increase to an array for later use
		percentIncrease.push_back(percentInc);
	}

	// Set the current type frequency values
	m_CurrentFreqInfoSel.KillChange		= percentIncrease[0];
	m_CurrentFreqInfoSel.GatherChange	= percentIncrease[1];
	m_CurrentFreqInfoSel.FetchChange	= percentIncrease[2];
	m_CurrentFreqInfoSel.ExploreChange	= percentIncrease[3];

	// Loop 2
	for (int i = 0; i < m_CurrentFreqInfoSel.DistFreqs.size(); i++)
	{
		// Compare current and previous frequency data to get the increase amount
		float increase = m_CurrentFreqInfoSel.DistFreqs[i] - m_PreviousFreqInfoSel.DistFreqs[i];

		// Calculate the increase as a percentage
		float percentInc = (increase / m_CurrentFreqInfoSel.DistFreqs[i]) * 100;

		// Add the percentage increase to an array for later use
		percentIncrease.push_back(percentInc);
	}

	// Set the current distance frequency values
	m_CurrentFreqInfoSel.CloseChange	= percentIncrease[4];
	m_CurrentFreqInfoSel.MidChange		= percentIncrease[5];
	m_CurrentFreqInfoSel.FarChange		= percentIncrease[6];

#ifdef DEBUG_ENABLED

	// Onscreen debug output
	if (GEngine)
	{
		FString Gen = FString(TEXT("GENERATION NUMBER: "));
		Gen += FString::FromInt(m_iEvolutionNumber);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 30.f, FColor::Green, Gen, false);

		for (int i = 0; i < percentIncrease.Num(); i++)
		{
			if (percentIncrease[i] > 0)
			{
				FString temp = m_pQuestMgr->getTypeName(i);
				temp += FString(TEXT(": "));
				temp += FString::SanitizeFloat(percentIncrease[i]);
				temp += FString(TEXT("% Increased"));

				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 30.f, FColor::Green, temp, false);
			}
			if (percentIncrease[i] < 0)
			{
				FString temp = m_pQuestMgr->getTypeName(i);
				temp += FString(TEXT(": "));
				temp += FString::SanitizeFloat(percentIncrease[i]);
				temp += FString(TEXT("% Decrease"));

				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 30.f, FColor::Red, temp, false);
			}
			if (percentIncrease[i] == 0)
			{
				FString temp = m_pQuestMgr->getTypeName(i);
				temp += FString(TEXT(": "));
				temp += FString(TEXT("No Change"));

				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 30.f, FColor::Yellow, temp, false);
			}
		}
	}
#endif

}

void UEvolutionManager::DisplayChangeFin()
{
	m_CurrentFreqInfoFin = {};

	// Get frequency information 
	CalculateFrequencyFin(&m_CurrentFreqInfoFin);

	if (m_PreviousFreqInfoFin.TypeFreqs.size() == 0)
		return;

	std::vector<float> percentIncreaseFin;

	// Loop 1 
	for (int i = 0; i < m_CurrentFreqInfoFin.TypeFreqs.size(); i++)
	{
		// Compare surrent and previous frequency data to get the increase amount
		float increase = m_CurrentFreqInfoFin.TypeFreqs[i] - m_PreviousFreqInfoFin.TypeFreqs[i];

		// Calculate the increase as a percentage
		float percentInc = (increase / m_CurrentFreqInfoFin.TypeFreqs[i]) * 100;

		// Add the percentage increase to an array for later use
		percentIncreaseFin.push_back(percentInc);
	}

	// Set the current type frequency values for 
	m_CurrentFreqInfoFin.KillChange			= percentIncreaseFin[0];
	m_CurrentFreqInfoFin.GatherChange		= percentIncreaseFin[1];
	m_CurrentFreqInfoFin.FetchChange		= percentIncreaseFin[2];
	m_CurrentFreqInfoFin.ExploreChange		= percentIncreaseFin[3];

	// Loop 2
	for (int i = 0; i < m_CurrentFreqInfoFin.DistFreqs.size(); i++)
	{
		// Compare current and previous frequency data to get the increase amount
		float increase = m_CurrentFreqInfoFin.DistFreqs[i] - m_PreviousFreqInfoFin.DistFreqs[i];

		// Calculate the increase as a percentage
		float percentInc = (increase / m_CurrentFreqInfoFin.DistFreqs[i]) * 100;

		// Add the percentage increase to an array for later use
		percentIncreaseFin.push_back(percentInc);
	}

	// Set the current distance frequency values
	m_CurrentFreqInfoFin.CloseChange		= percentIncreaseFin[4];
	m_CurrentFreqInfoFin.MidChange			= percentIncreaseFin[5];
	m_CurrentFreqInfoFin.FarChange			= percentIncreaseFin[6];

#ifdef DEBUG_ENABLED

	// Onscreen debug output
	if (GEngine)
	{
		FString Gen = FString(TEXT("GENERATION NUMBER: "));
		Gen += FString::FromInt(m_iEvolutionNumber);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 30.f, FColor::Green, Gen, false);

		for (int i = 0; i < percentIncreaseFin.Num(); i++)
		{
			if (percentIncreaseFin[i] > 0)
			{
				FString temp = m_pQuestMgr->getTypeName(i);
				temp += FString(TEXT(": "));
				temp += FString::SanitizeFloat(percentIncreaseFin[i]);
				temp += FString(TEXT("% Increased"));

				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 30.f, FColor::Green, temp, false);
			}
			if (percentIncreaseFin[i] < 0)
			{
				FString temp = m_pQuestMgr->getTypeName(i);
				temp += FString(TEXT(": "));
				temp += FString::SanitizeFloat(percentIncreaseFin[i]);
				temp += FString(TEXT("% Decrease"));

				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 30.f, FColor::Red, temp, false);
			}
			if (percentIncreaseFin[i] == 0)
			{
				FString temp = m_pQuestMgr->getTypeName(i);
				temp += FString(TEXT(": "));
				temp += FString(TEXT("No Change"));

				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 30.f, FColor::Yellow, temp, false);
			}
		}
	}

#endif
}

#ifdef LOGGING_ENABLED

/*
void UEvolutionManager::OutputResult()
{
	std::string filename("MessageLog.csv");
	std::ofstream myfile();

	myfile.open(filename);

	myfile << "This is the first cell in the first column.\n";
	myfile << "a,b,c,\n";
	myfile << "c,s,v,\n";
	myfile << "1,2,3.456\n";
	myfile << "semi;colon";


	std::string FilePath = FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir()) + TEXT("/DataOutput/MessageLog.csv");
	FString FileContent = TEXT("\n");

	FileContent += TEXT("GENERATION NUMBER,  ");
	FileContent += FString::FromInt(m_iEvolutionNumber);
	FileContent += TEXT(",\n");
	FileContent += TEXT("\n");

	FileContent += TEXT("SELECTION STAGE \n");
	FileContent += TEXT("\n");

	FileContent += TEXT("KILL,");
	FileContent += FString::FromInt(m_CurrentFreqInfoSel.KillFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iKill_Fitness);
	FileContent += TEXT("\n");

	FileContent += TEXT("FETCH,");
	FileContent += FString::FromInt(m_CurrentFreqInfoSel.FetchFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iFetch_Fitness);
	FileContent += TEXT("\n");

	FileContent += TEXT("GATHER,");
	FileContent += FString::FromInt(m_CurrentFreqInfoSel.GatherFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iGather_Fitness);
	FileContent += TEXT("\n");

	FileContent += TEXT("EXPLORE,");
	FileContent += FString::FromInt(m_CurrentFreqInfoSel.ExploreFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iExplore_Fitness);
	FileContent += TEXT("\n");

	FileContent += TEXT("CLOSE,");
	FileContent += FString::FromInt(m_CurrentFreqInfoSel.CloseFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iClose_Fitness);
	FileContent += TEXT("\n");

	FileContent += TEXT("MID,");
	FileContent += FString::FromInt(m_CurrentFreqInfoSel.MidFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iMid_Fitness);
	FileContent += TEXT("\n");

	FileContent += TEXT("FAR,");
	FileContent += FString::FromInt(m_CurrentFreqInfoSel.FarFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iFar_Fitness);
	FileContent += TEXT("\n");

	FileContent += TEXT("FINAL STAGE \n");
	FileContent += TEXT("\n");

	FileContent += TEXT("KILL,");
	FileContent += FString::FromInt(m_CurrentFreqInfoFin.KillFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iKill_Fitness);
	FileContent += TEXT("\n");

	FileContent += TEXT("FETCH,");
	FileContent += FString::FromInt(m_CurrentFreqInfoFin.FetchFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iFetch_Fitness);
	FileContent += TEXT("\n");

	FileContent += TEXT("GATHER,");
	FileContent += FString::FromInt(m_CurrentFreqInfoFin.GatherFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iGather_Fitness);
	FileContent += TEXT("\n");

	FileContent += TEXT("EXPLORE,");
	FileContent += FString::FromInt(m_CurrentFreqInfoFin.ExploreFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iExplore_Fitness);
	FileContent += TEXT("\n");

	FileContent += TEXT("CLOSE,");
	FileContent += FString::FromInt(m_CurrentFreqInfoFin.CloseFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iClose_Fitness);
	FileContent += TEXT("\n");

	FileContent += TEXT("MID,");
	FileContent += FString::FromInt(m_CurrentFreqInfoFin.MidFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iMid_Fitness);
	FileContent += TEXT("\n");

	FileContent += TEXT("FAR,");
	FileContent += FString::FromInt(m_CurrentFreqInfoFin.FarFreq);
	FileContent += TEXT(",%,");
	FileContent += TEXT("FITNESS,");
	FileContent += FString::FromInt(m_pPlayer->style->m_iFar_Fitness);
	FileContent += TEXT("\n");

	FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);

	myfile.close();

	return 0;
}
*/

#endif

