#pragma once

#include "Agent.h"

class Vacuum;

class AgentVacuum: public Agent
{
	private:

		#pragma region Sensors States

			//Tells if we are in the origin room
			bool m_bInfraredSensor;

			//Tells if the room is dirty
			bool m_bFotoSensor;

			//Tells if we are colliding
			bool m_bTouchSensor;

		#pragma endregion

		#pragma region Actions

			void _TurnLeft();

			void _TurnRight();

			void _MoveForward();

			void _Clean();

		#pragma endregion

		Vacuum* m_pAvatar;

	public:
		AgentVacuum(Vacuum* pAvatar);

		virtual void ExecuteSensors();

		virtual void ExecuteActions();
};