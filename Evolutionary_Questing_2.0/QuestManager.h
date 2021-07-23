// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include<vector>
#include <math.h>
#include <string>
#include <map>

#include "Quest.h"

class UPlayStyleManager;
class vector2D;

namespace std
{
	template<> struct less<UQuest>
	{
		bool operator() (const UQuest& lhs, const UQuest& rhs) const
		{
			return lhs.m_iQuestId < rhs.m_iQuestId;
		}
	};
}

class UQuestManager
{
public:

	//! Default constructor for quest manager.
	/*!

	*/
	UQuestManager();

	//! default destructor for quest manager
	/*!

	*/
	~UQuestManager();

	void InitDefaults();

	//! A function generates a random position within distance bounds.
	/*!
		\param distBounds - integer value used for setting max and min bounds (distance banding).
		\return 2d vector used for x and y position
	*/
	vector2D RandQuestPos(int distBounds);

	//! A function which initialises a text value of the quest type.
	/*!
		\param Typ - integer value used for switching type.
		\return string of type name
	*/
	std::string getTypeName(int Typ);

	///////////////////////////////////////////////////		Variables		//////////////////////////////////////////////////////////////////////////////


	std::vector<UQuest*> vpPopulation;				/**< A vector of type quest pointer which holds all currently existing quests. */

	std::map<UQuest, int> m_mFitnesses;			/**< Ordered list of fitnesses */
};





