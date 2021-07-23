
#include "Quest.h"


void UQuest::Init(int distEnum, int typEnum, std::string qName)
{
	// Set quest x position
	m_iXPos = 0;
	// Set quest y position
	m_iYPos = 0;

	// Set quest y position
	m_iZPos = 130;

	m_vQuestLocation =  vector2D(m_iXPos, m_iYPos);

	m_bEventCreated = false;

	// Set quest type position
	m_eType = typEnum;
	// Initialise state to available
	m_eStatus = ST_AVAILABLE;

	m_sQuestName = qName;

	// Initialise the quest type name
	setTypeName();
	setDescription();

	m_bInCompleteAdded = false;
}

void UQuest::setTypeName()
{
	// Set quest name vaue based on type
	switch (m_eType)
	{
	case TP_KILL:
		m_sTypeName = "Kill Quest";
		m_eTypeElement = ELEMENT_KILL;
		break;
	case TP_GATHER:
		m_sTypeName = "Gather Quest";
		m_eTypeElement = ELEMENT_GATHER;
		break;
	case TP_FETCH:
		m_sTypeName = "Fetch Quest";
		m_eTypeElement = ELEMENT_FETCH;
		break;
	case TP_EXPLORE:
		m_sTypeName = "Explore Quest";
		m_eTypeElement = ELEMENT_EXPLORE;
		break;
	}
}

void UQuest::setType(QuestType Typ)
{
	m_eType = Typ;

	setTypeName();
}

void UQuest::setDistName()
{
	// Set distance banding name text by band
	switch (m_eDistBand)
	{
	case DIST_CLOSE:
		m_sDistName = " (Close)";
		m_eDistElement = ELEMENT_CLOSE;
		break;
	case DIST_MID:
		m_sDistName = " (Mid)";
		m_eTypeElement = ELEMENT_MID;
		break;
	case DIST_FAR:
		m_sDistName = " (Far)";
		m_eTypeElement = ELEMENT_FAR;
		break;
	}
}

void UQuest::setDescription()
{
	int randOpener = rand() % 3+1;

	switch (randOpener)
	{
	case 0:
		m_sDescription = "I have a mighty quest for a mighty worrier! Your travels will take you ";
		break;
	case 1:
		m_sDescription = "Hello Adventurer! Might you aid me in a task? It's ";
		break;
	case 2:
		m_sDescription = "Hey you? Do me a favor will you? It's ";
		break;
	case 3:
		m_sDescription = "If only there were someone who could help me.... Ah you there? I have a job for you! It's ";
		break;
	}

	// Set distance banding name text by band
	switch (m_eDistBand)
	{
	case DIST_CLOSE:
		m_sDescription += "only a short distance so pack light. ";
		break;
	case DIST_MID:
		m_sDescription += "not too far. ";
		break;
	case DIST_FAR:
		m_sDescription += "a great distance so be prepeared. ";
		break;
	}

	// Set quest name vaue based on type
	switch (m_eType)
	{
	case TP_KILL:
		m_sDescription += "When you arrive at your destination dispatch the swine that you find there! He served me cold tea this one time... there's no excuse for it!";
		break;
	case TP_GATHER:
		m_sDescription += "Christmas is fast approaching and i need some good twigs for my reef! could you gather me some?";
		break;
	case TP_FETCH:
		m_sDescription += "Whilst out sinking some bevies with my bestie last night i misplaced my hoard or treasure... could you find it?";
		break;
	case TP_EXPLORE:
		m_sDescription += "I have been studying the contents of the rubbish looking stone houses that keep randomly appearing but recently one collapsed on my leg! Could you go explore that one for me?";
		break;
	}
}

void UQuest::setQuestName()
{
	setDistName();

	std::string newName;

	newName += m_sTypeName;
	newName += m_sDistName;
	m_sQuestName = newName;
}

void UQuest::setState(QuestStatus state)
{
	// Set quest state
	m_eStatus = state;
}

void UQuest::SetDistanceFromPlayer(vector2D playerPos)
{
	m_fDistance = playerPos.getDistance(m_vQuestLocation);

	// Set the distance banding based on distance from player
	if (m_fDistance > 0 && m_fDistance <= 1000)
	{
		m_eDistBand = DIST_CLOSE;
		m_eDistElement = ELEMENT_CLOSE;
	}
	else if (m_fDistance > 1000 && m_fDistance <= 2000)
	{
		m_eDistBand = DIST_MID;
		m_eDistElement = ELEMENT_MID;
	}
	else if (m_fDistance > 2000)
	{
		m_eDistBand = DIST_FAR;
		m_eDistElement = ELEMENT_FAR;
	}

	setQuestName();
	setDescription();
}