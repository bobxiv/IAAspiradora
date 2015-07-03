#include <cstdlib>
#include <iostream>

#include "Game.h"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

Game::Game(): m_Window(sf::VideoMode(600,300,32), "Agente Aspiradora Busqueda"), m_Pausa(true), m_PausaLabel("Pausa")
{
	m_Window.Show(true);
	m_Window.SetFramerateLimit(60);

	m_PausaLabel.SetPosition( 300 - m_PausaLabel.GetRect().GetWidth()/2.0f,150 + m_PausaLabel.GetRect().GetHeight()/2.0f);
	m_PausaLabel.SetFont( sf::Font::GetDefaultFont() );

	m_pAmbient = AmbientVacuum::GetInstance();
}

Game::~Game()
{
	delete m_pAmbient;
}

//-------------------------------------------------------------//
//-----------------		Metodos		---------------------------//
//-------------------------------------------------------------//

void Game::Iniciar()
{
	const sf::Input& input = m_Window.GetInput();
	
	float dt;
	while( m_Window.IsOpened() )
	{
		//Atrapamos los eventos para cerra la ventana
		while( m_Window.GetEvent(m_Evento) )
		{
			switch( m_Evento.Type )
			{
			case sf::Event::Closed:
					m_Window.Close();
				break;
				
			case sf::Event::KeyPressed:
				if( m_Evento.Key.Code == sf::Key::Escape)
					m_Window.Close();
				break;

			case sf::Event::KeyReleased:
				m_Pausa = !m_Pausa;
				#ifdef _DEBUG
					if( m_Pausa == true )
						std::cout<<"PAUSA..."<<std::endl;
					else
						std::cout<<"-------------------------------"<< std::endl;
				#endif
				break;
			}
		}
		dt = m_Window.GetFrameTime();

		Actualizar(dt);    //Actualizamos la escena

		Dibujar();       //Dibujamos todo el juego
	}
}

void Game::Actualizar(float dt)
{
	if( !m_Pausa )
	{
		m_Vacuum.Update(dt);

		m_pAmbient->Update();
	}
}

void Game::Dibujar()
{
	m_Window.Clear();

	AmbientVacuum* ambient = AmbientVacuum::GetInstance();
	ambient->Dibujar(m_Window);

	m_Vacuum.Dibujar(m_Window);

	if( m_Pausa )
		m_Window.Draw( m_PausaLabel );

	m_Window.Display();
}