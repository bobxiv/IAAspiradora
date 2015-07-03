#pragma once

//#include "AgentVacuum.h"
#include "Agent.h"

class AgentVacuum;

//Inludes de SFML
#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"


#define ROOM1_X 100
#define ROOM1_Y 100

#define ROOM2_X 500
#define ROOM2_Y 100

#define ROOM3_X 100
#define ROOM3_Y 500

#define ROOM4_X 500
#define ROOM4_Y 500

//Room indices:
//
//	-----------------
//  |	1	|	2	|
//	|----------------
//	|	3	|	4	|
//	-----------------
class AmbientVacuum: public Ambient
{
	private:

		//The index of the Room that the agent is
		int m_RoomIndex;

		int m_OriginalRoomIndex;

		bool m_aRoomDity[4];

		static AmbientVacuum* m_pInstance;

	public:
		friend class AgentVacuum;

		static AmbientVacuum* GetInstance();

		AmbientVacuum();

		virtual ~AmbientVacuum();

		void Dibujar(sf::RenderWindow& RW) const;
};