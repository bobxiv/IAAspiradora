#pragma once

//Inludes de SFML
#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"

#include "string"

#define ANGULAR_VEL 20.0f
#define LINEAL_VEL  80.0f

class AgentVacuum;

class Vacuum
{
private:

	std::string        m_CurrentState;

	sf::Shape          m_Sprite;
	sf::Shape		   m_Wheel1;
	sf::Shape		   m_Wheel2;
	sf::Shape		   m_Wheel3;
	sf::Shape		   m_Wheel4;
	sf::Shape		   m_Front;

	sf::Vector2<float> m_Posicion;

	float			   m_Angulo;

	AgentVacuum*	   m_pAgent;

	sf::Vector2<float> m_Aux;

public:
	friend class AgentVacuum;

	Vacuum();

	~Vacuum();

	void Update(float dt);

	void Dibujar(sf::RenderWindow& RW);


	void TurnLeft();

	void TurnRight();

	void MoveForward();

	void Clean();
};