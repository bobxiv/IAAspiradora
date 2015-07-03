#include "AmbientVacuum.h"
#include "AgentVacuum.h"

//Singleton Variables Declaration
AmbientVacuum* AmbientVacuum::m_pInstance = NULL;

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

AmbientVacuum::AmbientVacuum()
{
	//m_apAgents.push_back( new AgentVacuum(this) );

	m_aRoomDity[0] = true;
	m_aRoomDity[1] = true;
	m_aRoomDity[2] = true;
	m_aRoomDity[3] = true;

	m_OriginalRoomIndex = m_RoomIndex = 1;
}

AmbientVacuum::~AmbientVacuum()
{
	m_apAgents.clear();
}


//-------------------------------------------------------------//
//-----------------		Methods		---------------------------//
//-------------------------------------------------------------//


AmbientVacuum* AmbientVacuum::GetInstance()
{
	if( !AmbientVacuum::m_pInstance )
		AmbientVacuum::m_pInstance = new AmbientVacuum();
	return m_pInstance;
}

void AmbientVacuum::Dibujar(sf::RenderWindow& RW) const
{
	sf::Shape dirt = sf::Shape::Rectangle(sf::Vector2<float>(0.0f,0.0f), sf::Vector2<float>(300.0f,300.0f), sf::Color::Red);
	if( m_aRoomDity[0] )
	{
		dirt.SetPosition( sf::Vector2<float>(0,0) );
		RW.Draw(dirt);
	}

	if( m_aRoomDity[1] )
	{
		dirt.SetPosition( sf::Vector2<float>(300,0) );
		RW.Draw(dirt);
	}

	if( m_aRoomDity[2] )
	{
		dirt.SetPosition( sf::Vector2<float>(0,300) );
		RW.Draw(dirt);
	}

	if( m_aRoomDity[3] )
	{
		dirt.SetPosition( sf::Vector2<float>(300,300) );
		RW.Draw(dirt);
	}
}