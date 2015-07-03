#include "AgentVacuum.h"
#include "Vacuum.h"
#include "AmbientVacuum.h"

#include "iostream"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

AgentVacuum::AgentVacuum(Vacuum* pAvatar): m_pAvatar(pAvatar)
	{}

//-------------------------------------------------------------//
//-----------------		Methods		---------------------------//
//-------------------------------------------------------------//

void AgentVacuum::_TurnLeft()
{
	#if _DEBUG
		std::cout<<"Accion: Girar Izquierda"<< std::endl;
		std::cout<<"-------------------------------"<< std::endl;
	#endif
		
	m_pAvatar->TurnLeft();
}

void AgentVacuum::_TurnRight()
{
	#if _DEBUG
		std::cout<<"Accion: Girar Derecha"<< std::endl;
		std::cout<<"-------------------------------"<< std::endl;
	#endif

	m_pAvatar->TurnRight();
}

void AgentVacuum::_MoveForward()
{
	#if _DEBUG
		std::cout<<"Accion: Avanzar"<< std::endl;
		std::cout<<"-------------------------------"<< std::endl;
	#endif

	m_pAvatar->MoveForward();
	AmbientVacuum* vacuumWorld = AmbientVacuum::GetInstance();
	switch( vacuumWorld->m_RoomIndex )
	{
		case 1:
			if( m_pAvatar->m_Angulo == 0 )
				vacuumWorld->m_RoomIndex = 2;
			else if( m_pAvatar->m_Angulo == 270 )
				vacuumWorld->m_RoomIndex = 3;
			break;
		case 2:
			if( m_pAvatar->m_Angulo == 180 )
				vacuumWorld->m_RoomIndex = 1;
			else if( m_pAvatar->m_Angulo == 270 )
				vacuumWorld->m_RoomIndex = 4;
			break;
		case 3:
			if( m_pAvatar->m_Angulo == 0 )
				vacuumWorld->m_RoomIndex = 4;
			else if( m_pAvatar->m_Angulo == 90 )
				vacuumWorld->m_RoomIndex = 1;
			break;
		case 4:
			if( m_pAvatar->m_Angulo == 180 )
				vacuumWorld->m_RoomIndex = 3;
			else if( m_pAvatar->m_Angulo == 90 )
				vacuumWorld->m_RoomIndex = 2;
			break;
	}
}

void AgentVacuum::_Clean()
{
	#if _DEBUG
		std::cout<<"Accion: Limpiar"<< std::endl;
		std::cout<<"-------------------------------"<< std::endl;
	#endif

	m_pAvatar->Clean();
	AmbientVacuum* vacuumWorld = AmbientVacuum::GetInstance();
	vacuumWorld->m_aRoomDity[vacuumWorld->m_RoomIndex-1] = false;
}

void AgentVacuum::ExecuteSensors()
{
	AmbientVacuum* vacuumWorld = AmbientVacuum::GetInstance();

	if( vacuumWorld != NULL )
	{
		//Reinicializo
		m_bInfraredSensor = m_bTouchSensor = m_bFotoSensor = false;

		//Control de limpieza
		if( vacuumWorld->m_aRoomDity[vacuumWorld->m_RoomIndex-1] )
			m_bFotoSensor = true;
		
		//Control de colision
		switch(vacuumWorld->m_RoomIndex)
		{
			case 1:
				if( m_pAvatar->m_Angulo == 180 || m_pAvatar->m_Angulo == 90 )
					m_bTouchSensor = true;
				break;
			case 2:
				if( m_pAvatar->m_Angulo == 0 || m_pAvatar->m_Angulo == 90 )
					m_bTouchSensor = true;
				break;
			case 3:
				if( m_pAvatar->m_Angulo == 180 || m_pAvatar->m_Angulo == 270 )
					m_bTouchSensor = true;
				break;
			case 4:
				if( m_pAvatar->m_Angulo == 0 || m_pAvatar->m_Angulo == 270 )
					m_bTouchSensor = true;
				break;
		}

		//Control de posicion original
		if( vacuumWorld->m_OriginalRoomIndex == vacuumWorld->m_RoomIndex )
			m_bInfraredSensor = true;

		#if _DEBUG
			std::cout<<"Sensor Tactil:     "<< m_bTouchSensor   << std::endl;
			std::cout<<"Fotosensor:        "<< m_bFotoSensor    << std::endl;
			std::cout<<"Sensor Infrarrojo: "<< m_bInfraredSensor<< std::endl;
			std::cout<<"-------------------------------"<< std::endl;
		#endif
	}
}

void AgentVacuum::ExecuteActions()
{
	//Conditional Format
	if( m_bFotoSensor )
		_Clean();
	else
	{
		if( m_bTouchSensor )
			_TurnLeft();
		else
			_MoveForward();
	}
	/*
	//Binary Table Format
	switch( 255 && m_bFotoSensor && m_bTouchSensor )
	{
		case 0x11111111:
			{

			}
		case 0x11111111:
			{

			}
	}
	*/
}