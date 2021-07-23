#pragma once

#include <string>

#include "myMaths.h"



// Enum for quest availablity status setting
enum QuestStatus
{
	ST_AVAILABLE = 0,
	ST_ACCEPTED,
	ST_COMPLETED,
	ST_ABANDONED,
	ST_REJECTED
};

// Enum for quest distance band setting
enum QuestDistance
{
	DIST_CLOSE = 0,
	DIST_MID,
	DIST_FAR
};

//Enum for quest type setting
enum QuestType
{
	TP_KILL = 0,
	TP_GATHER,
	TP_FETCH,
	TP_EXPLORE
};

enum QuestElements
{
	ELEMENT_NULL = 0,
	ELEMENT_KILL,
	ELEMENT_GATHER,
	ELEMENT_FETCH,
	ELEMENT_EXPLORE,
	ELEMENT_CLOSE,
	ELEMENT_MID,
	ELEMENT_FAR
};


class UQuest
{

public:

	UQuest(int distEnum, int typEnum, std::string qName)
		: m_eDistBand(distEnum), m_eType(typEnum), m_sQuestName(qName)
	{
		m_iQuestId = m_iAccumulativeQuestId;
		m_iAccumulativeQuestId++;

		m_fDistance				= 0;
		m_iTotalFitness			= 0;
		m_iTypeElementFitness	= 0;
		m_iDistElementFitness	= 0;

		m_eTypeElement			= ELEMENT_NULL;
		m_eDistElement			= ELEMENT_NULL;
	}

	UQuest()
		: m_eDistBand(-1), m_eType(-1), m_sQuestName("")
	{
		m_iQuestId = m_iAccumulativeQuestId;
		m_iAccumulativeQuestId++;
	}

	UQuest(const UQuest& other)
		: m_eDistBand(other.m_eDistBand), m_eType(other.m_eType), m_sQuestName(other.m_sQuestName), 
		m_iQuestId(other.m_iQuestId), m_iXPos(other.m_iXPos), m_iYPos(other.m_iYPos), m_iZPos(other.m_iZPos), 
		m_vQuestLocation(other.m_vQuestLocation), m_eStatus(other.m_eStatus), m_fDistance(other.m_fDistance),
		m_iTotalFitness(other.m_iTotalFitness), m_iTypeElementFitness(other.m_iTypeElementFitness), 
		m_iDistElementFitness(other.m_iDistElementFitness), m_sTypeName(other.m_sTypeName), m_sDistName(other.m_sDistName), 
		m_sDescription(other.m_sDescription), m_bEventCreated(other.m_bEventCreated), m_bInCompleteAdded(other.m_bInCompleteAdded), m_bChosen(other.m_bChosen)
	{		 
	}

	~UQuest() {}

	//! Initialises the quests storage.
	/*!
		\param d		- enum used to set quest distance
		\param typ		- enum used to set quest type
		\param qName	- string used to set quests name
		\param p		- pointer the the quest manager object
	*/
	void Init(int distEnum, int typEnum, std::string qName);

	//! Destroys quest and removes all data.
	/*!
		\param comQuest - pointer to quest to be compared to.
		\return True if quest data is equal.
	*/
	bool Compare(UQuest* comQuest)
	{
		// Compare two quests, return true if the same, false if different
		if (comQuest->m_iXPos == m_iXPos && comQuest->m_iYPos == m_iYPos && comQuest->m_iZPos == m_iZPos && comQuest->m_eType == m_eType)
			return true;
		else
			return false;
	}

	bool CompareID(UQuest* comQuest)
	{
		// Compare two quests, return true if the same, false if different
		if (comQuest->m_iQuestId == m_iQuestId)
			return true;
		else
			return false;
	}

	//! Overloads equal to operator using compare function.
	/*!
		\param comQuest - pointer to quest to be compared to
		\return True if quest data is equal.
	*/
	bool operator==(UQuest* comQuest)
	{
		return Compare(comQuest);
	}

	//! A function which initialises a text value of the quest type.
	/*!

	*/
	void setTypeName();

	//! A function which initialises a text value of the quest type.
	/*!
		\param Typ - enum used for setting the quests type
	*/
	void setType(QuestType Typ);

	//! A function sets the location of the quest.
	/*!
		\param loc used for setting each element of the plcation
	*/
	void setLocation(vector2D loc) { m_iXPos = loc.getX();  m_iYPos = loc.getY(); }

	//! A function which initialises m_iZPosa text value of the quest distance banding (close, mid, far).
/*!

*/
	void setDistName();

	//!  A function which initialises a text value of the quest complete name (type + distance).
	/*!

	*/
	void setQuestName();

	//! A function which sets the description text for the quest ( eg "got to the castle and slay the dragon")
	/*!

	*/
	void setDescription();

	//! A function which sets the quest status - AVAILABLE, ACCEPTED, COMPLETE, ABANDONED.
	/*!
		\param state of type enum used to set the state of the quest.
	*/
	void setState(QuestStatus state);

	//! A function which sets the distance of the quest from the player and update banding, name and description.
	/*!
		\param playerPos - position used for distance calculation.
	*/
	void SetDistanceFromPlayer(vector2D playerPos);

	///////////////////////////////////////////////////		Variables		//////////////////////////////////////////////////////////////////////////////

	int m_eDistBand;		/**< Enum value used for representation of distance banding (close, mid, far).*/
	int m_eType;				/**< Enum value used for representation of quest type. */
	std::string m_sQuestName;		/**< String value used to store the text of a quest name for diplay in the text box. */

	int m_iQuestId;

	float m_iXPos;					/**< Integer value x representing the quests x position. */
	float m_iYPos;					/**< Integer value y representing the quests y position. */
	float m_iZPos;					/**< Integer value z representing the quests y position. */

	vector2D m_vQuestLocation;		/**< Vector of quests location. */

	QuestStatus m_eStatus;			/**< Enum value used for representation of quest status (completed, available, accepted)*/

	QuestElements m_eTypeElement;	/**< Enum value used for representation of quest status (completed, available, accepted)*/

	QuestElements m_eDistElement;	/**< Enum value used for representation of quest status (completed, available, accepted)*/

	float m_fDistance;				/**< Float value indicates the actual distance of the quest from the player*/

	int m_iTotalFitness;			/**< Integer value representing the quests total fitness*/

	int m_iTypeElementFitness;		/**< Integer value representing the fitness of the type element*/

	int m_iDistElementFitness;		/**< Integer value representing the fitness of the distance element*/

	std::string m_sTypeName;		/**< String value used to store the text of a quest type for diplay in the text box. */

	std::string m_sDistName;		/**< String value used to store the text of a quest distance name for diplay in the text box. */

	std::string m_sDescription;		/**< String value used to store the text of a quest description for diplay in the text box. */

	bool m_bEventCreated;			/**< Bool value representing creation event*/

	bool m_bInCompleteAdded;		/**< Bool value representing the addition of an incomplete quest*/

	bool m_bChosen;					/**< integer value representing the fitness of the distance element*/

	static int m_iAccumulativeQuestId;			/**< Integer value used for unique identification. */
};
