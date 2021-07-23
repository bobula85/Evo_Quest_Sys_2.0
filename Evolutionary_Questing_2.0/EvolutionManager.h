// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EvolutionManager.h"

class UQuestManager;
class UQuest;
class UPlayStyleManager;


#include <vector>

// Struct used to contain all population statistical data
struct Ffrequency_Count
{
///////////////////////////		Selection Population Stats		/////////////////////////////////////////////

	int KillCount;			/**< Integer value of total number of kill quests */
	float KillFreq;			/**< Float value of kill quests frequency in the population */
	float KillChange;		/**< Float value of change to kill quests frequency in the population */

	int FetchCount;			/**< Integer value of total number of fetch quests */
	float FetchFreq;		/**< Float value of fetch quests frequency in the population */
	float FetchChange;		/**< Float value of change to fetch quests frequency in the population */

	int GatherCount;		/**< Integer value of total number of gather quests */
	float GatherFreq;		/**< Float value of gather quests frequency in the population */
	float GatherChange;		/**< Float value of change to gather quests frequency in the population */

	int ExploreCount;		/**< Integer value of total number of explore quests */
	float ExploreFreq;		/**< Float value of expolre quests frequency in the population */
	float ExploreChange;	/**< Float value of change to explore quests frequency in the population */

	int CloseCount;			/**< Integer value of total number of close quests */
	float CloseFreq;		/**< Float value of close quests frequency in the population */
	float CloseChange;		/**< Float value of change to close quests frequency in the population */

	int MidCount;			/**< Integer value of total number of mid quests */
	float MidFreq;			/**< Float value of mid quests frequency in the population */
	float MidChange;		/**< Float value of change to mid quests frequency in the population */

	int FarCount;			/**< Integer value of total number of far quests */
	float FarFreq;			/**< Float value of far quests frequency in the population */
	float FarChange;		/**< Float value of change to far quests frequency in the population */

	bool lowTypeFreq = false;	/**< Bool value used to show low frequency of a quest type */
	bool lowDistFreq = false;	/**< Bool value used to show low frequency of a quest distance */

	std::vector<int> LowTypeFreqs;	/**< Array of low type frequencies */
	std::vector<int> LowDistFreqs;	/**< Array of low distance frequencies */
	std::vector<float> TypeFreqs;	/**< Array of all type frequencies */
	std::vector<float> DistFreqs;	/**< Array of all distance frequencies */

	int highestType;		/**< Array of all distance frequencies */
	int highestDist;		/**< Array of all distance frequencies */
};

/**< Data strucute that hold a pair of parent quests and the two resulting child quests */
struct FPair
{
	FPair()
		: parent1(nullptr), parent2(nullptr), child1(nullptr), child2(nullptr)
	{}

	FPair(UQuest* p1, UQuest* p2)
		: parent1(p1), parent2(p2)
	{}

	UQuest* parent1;		/**< Quest poniter to parent quest 1 */
	UQuest* parent2;		/**< Quest poniter to parent quest 2 */
	UQuest* child1;			/**< Quest poniter to child quest 1 */
	UQuest* child2;			/**< Quest poniter to child quest 2 */
};


struct FGenerationData
{
	int Generation;
	int Stage;

	int PercentageKill;
	int PercentageFetch;
	int PercentageGather;
	int PercentageExplore;
	int PercentageClose;
	int PercentageMid;
	int PercentageFar;

};

class UEvolutionManager 
{
	
public:	

	//! Default constructor for evolution manager
	/*!
		\param p a character pointer which allows access to the play style manager.
		\param mgr a pointer to the quest manager.
	*/
	UEvolutionManager();

	//! Default constructor for evolution manager
/*!
	\param p a character pointer which allows access to the play style manager.
	\param mgr a pointer to the quest manager.
*/
	UEvolutionManager(UQuestManager* questMgr, UPlayStyleManager* player, int maxPairs, int maxChildren, int maxPopulation, int evolutionNumber, int reintroRate);

	//! Default destructor for Evolution manager.
	/*!
		\param p a character pointer which allows access to the play style manager.
		\param mgr a pointer to the quest manager.
	*/
	~UEvolutionManager();

	//void Init(PlayStyleManager* p, UQuestManager* mgr);

	//! A function which begins the evolutionary process.
	/*!
	* 
	*/
	void EvolveQuests();

	const int GetMaxPairs()								{ return m_iMaxPairs;						};
	const int GetMaxChildren()							{ return m_iMaxChildren;					};
	const int GetMaxPopulation()						{ return m_iMaxPopulation;					};
	const int GetReintroRate()							{ return m_iReintroRate;					};
	const int GetEvolutionNumber()						{ return m_iEvolutionNumber;				};

	void SetMaxPairs(int newMaxPairs)					{ m_iMaxPairs			= newMaxPairs;		};
	void SetMaxChildren(int newMaxChildren)				{ m_iMaxChildren		= newMaxChildren;	};
	void SetMaxPopulation(int newMaxPopulation)			{ m_iMaxPopulation		= newMaxPopulation; };
	void SetReintroRate(int newReintroRate)				{ m_iReintroRate		= newReintroRate;	};
	void SetEvolutionNumber(int newEvoNumber)			{ m_iEvolutionNumber	= newEvoNumber;		};

	//! A function which evaluates a quests fitness values.
	/*!
		\param qst a Quest pointer for a quest object.
	*/
	void Evaluate(UQuest* qst);

private:



	//! A function which selects parents using the roulette wheele selection method.
	/*!
	*/
	void SelectRoulette();

	//! A function which selects parents using the ranked selection method.
	/*!
	*/
	void SelectRanked();

	//! A function which creates new solutions from the parents selected in SelectionRoulette() and SelectRanked().
	/*!
	*/
	void Combine();

	//! A function which calculates the frequecy of each quest element type before survivor selection.
	/*!
	*/
	void CalculateFrequencySel(Ffrequency_Count* count);

	//! A function which calculates the frequecy of each quest element type after the evolutionary process has finished.
	/*!
	*/
	void CalculateFrequencyFin(Ffrequency_Count* count);

	//! A function mutates a set amount of quests when some genetic material is lost (not selected for the next generation).
	/*!
	*/
	void Mutate();

	//! A function which selects soultions created in Combine() to be added to the population.
	/*!
	*/
	void Survive();

	//! A function which updates the frequency change values and displayes them depening on debug option
	/*!
	*/
	void DisplayChangeSel();

	//! A function which updates the frequency change values and displayes them depening on debug option
	/*!
	*/
	void DisplayChangeFin();

	//! A function outputs evolutionary data to a .csv file.
	/*!
	*/
	//void OutputResult();



///////////////////////////////////////////////////		Variables		//////////////////////////////////////////////////////////////////////////////

	UQuestManager* m_pQuestMgr;							/*!< A pointer of QuestManager* which holds all quest information. */
	UPlayStyleManager* m_pPlayer;						/*!< A pointer of type Player* which holds the players accepted quests. */

	int m_iMaxPairs;									/*!< An integer which declares the maximum amount of mating pairs that can be selected for breeding. */
	int m_iMaxChildren;									/*!< An integer which declares the maximum amount of children that can be created per evolution. */
	int m_iMaxPopulation;								/*!< An integer which declares the maximum size of the population. */
	int m_iReintroRate;									/*!< An integer used to trigger mutation of quest elements. */
	int m_iEvolutionNumber; 							/*!< An integer which declares the total number of evolutions that have taken place so far. */

	std::vector<UQuest*> m_aChildren;					/*!< A vector of type Quest* which holds the players accepted quests. */
	std::vector<FPair> m_aParents;						/*!< A vector of type quest which holds the players accepted quests. */
	
	std::vector<FGenerationData> m_aEvoGenerationData;	/*!< A vector of type quest which holds the players accepted quests. */

	Ffrequency_Count m_CurrentFreqInfoSel;				/*!< A structure that holds the current populations frequency information prior to selection. */
	Ffrequency_Count m_PreviousFreqInfoSel;				/*!< A structure that holds the previous populations frequency information prior to selection. */
	Ffrequency_Count m_CurrentFreqInfoFin;				/*!< A structure that holds the current populations frequency information at the end of an evolution. */
	Ffrequency_Count m_PreviousFreqInfoFin;				/*!< A structure that holds the previous populations frequency information at the end of an evolution. */
};
