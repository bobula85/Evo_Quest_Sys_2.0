// Evolutionary_Questing_2.0.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "BasicUtilities.h"

#include "QuestManager.h"
#include "PlayStyleManager.h"
#include "EvolutionManager.h"

#include "MyUtils.h"

#include <iostream>

int UQuest::m_iAccumulativeQuestId;

/*
TO_ADD - SEARCH FOR THIS TO SEE WHERE ALGORITHMIC CHANGES SHOULD BE!
*/

int main()
{
    TimeProfiler::Get().TimeBeginSession("Profile");

    UQuestManager* qManager = new UQuestManager();
    UPlayStyleManager* player = new UPlayStyleManager();

    UEvolutionManager* evoManager = new UEvolutionManager(qManager, player, 10, 20, 10, 0, 10);

    for(int i = 0; i < 4; i++)
        evoManager->EvolveQuests();

    TimeProfiler::Get().TimeEndSession();

    std::cin.get();



}

