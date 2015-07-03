#pragma once

//Agente y ambiente del problema
#include "AgentVacuum.h"
#include "AmbientVacuum.h"

#include "Vacuum.h"

//Inludes de SFML
#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"

//Clase principal del juego, contiene toda la estructura de juego
//Este objeto sera el responsable de:
//	-Crear la escena
//	-Dibujar la pantalla
//	-Actualizar el juego
class Game
{
private:
	
	//Ventana principal
	sf::RenderWindow m_Window;

	sf::Event        m_Evento;

	//Elementos de la escena:
	//-----------------------

	AmbientVacuum*   m_pAmbient;

	Vacuum           m_Vacuum;

	sf::String       m_PausaLabel;

	bool             m_Pausa;

public:

	Game();

	~Game();

	//Inicializa la escena, se llama luego de creada la instacia de Game
	void Iniciar();

	//Actualiza la escena
	void Actualizar(float dt);

	//Dibuja la escena
	void Dibujar();

};