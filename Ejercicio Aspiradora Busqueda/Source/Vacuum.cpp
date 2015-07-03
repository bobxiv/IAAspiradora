#include "Vacuum.h"
#include "AgentVacuum.h"
#include "AmbientVacuum.h"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

Vacuum::Vacuum()
{
	//m_Sprite = sf::Shape::Circle(sf::Vector2<float>(0.0f,0.0f), 30.0f, sf::Color::Green); 
	m_Sprite = sf::Shape::Rectangle(sf::Vector2<float>(-25.0f,15.0f), sf::Vector2<float>(25.0f,-15.0f), sf::Color::Green); 
	m_Angulo = 90.0f;
	m_Sprite.SetRotation(90);

	m_Wheel1 = sf::Shape::Rectangle(sf::Vector2<float>(-30.0f,20.0f), sf::Vector2<float>(-15.0f,13.5f), sf::Color::Blue); 
	m_Wheel1.SetRotation(90);

	m_Wheel2 = sf::Shape::Rectangle(sf::Vector2<float>(-30.0f,-13.5f), sf::Vector2<float>(-15.5f,-20.0f), sf::Color::Blue); 
	m_Wheel2.SetRotation(90);

	m_Wheel3 = sf::Shape::Rectangle(sf::Vector2<float>(15.0f,20.0f), sf::Vector2<float>(30.0f,13.5f), sf::Color::Blue); 
	m_Wheel3.SetRotation(90);

	m_Wheel4 = sf::Shape::Rectangle(sf::Vector2<float>(15.0f,-13.5f), sf::Vector2<float>(30.0f,-20.0f), sf::Color::Blue); 
	m_Wheel4.SetRotation(90);

	m_Front = sf::Shape::Line(sf::Vector2<float>(20.0f,0.0f), sf::Vector2<float>(29.0f,0.0f), 3.0f, sf::Color::White); 
	m_Front.SetRotation(90);

	//Initialize the position
	AmbientVacuum* vacuumWorld = AmbientVacuum::GetInstance();
	switch( vacuumWorld->m_OriginalRoomIndex )
	{
		case 1:
			m_Posicion.x = 100.0f;
			m_Posicion.y = 150.0f;
			break;
		case 2:
			m_Posicion.x = 500.0f;
			m_Posicion.y = 150.0f;
			break;
	}
	m_Sprite.SetPosition(m_Posicion.x,m_Posicion.y);

	m_pAgent = new AgentVacuum(this);
}

Vacuum::~Vacuum()
{
	delete m_pAgent;
}

//-------------------------------------------------------------//
//-----------------		Methods		---------------------------//
//-------------------------------------------------------------//

void Vacuum::Update(float dt)
{
	if( m_CurrentState == "" )
	{
		m_pAgent->ExecuteSensors();
		m_pAgent->ExecuteActions();
		return;
	}

	if( m_CurrentState == "Left" || m_CurrentState == "Right" )
	{
		sf::Vector2<float> dir = m_Aux - m_Posicion;
		float dist = sqrt( dir.x*dir.x + dir.y*dir.y );
		if( dist < 5 )
		{
			m_Sprite.SetPosition(m_Aux.x, m_Aux.y);
			m_pAgent->ExecuteSensors();
			m_pAgent->ExecuteActions();
			return;
		}
		dir /= sqrt( dir.x*dir.x + dir.y*dir.y );
		dir *= dt*LINEAL_VEL;
		m_Posicion += dir;

		m_Sprite.Move( dir.x, dir.y);
		return;
	}

	if( m_CurrentState == "Clean" )
	{
		m_pAgent->ExecuteSensors();
		m_pAgent->ExecuteActions();
		return;
	}
}

void Vacuum::Dibujar(sf::RenderWindow& RW)
{
	m_Wheel1.SetRotation(m_Sprite.GetRotation());
	m_Wheel1.SetPosition(m_Sprite.GetPosition());
	m_Wheel2.SetRotation(m_Sprite.GetRotation());
	m_Wheel2.SetPosition(m_Sprite.GetPosition());
	m_Wheel3.SetRotation(m_Sprite.GetRotation());
	m_Wheel3.SetPosition(m_Sprite.GetPosition());
	m_Wheel4.SetRotation(m_Sprite.GetRotation());
	m_Wheel4.SetPosition(m_Sprite.GetPosition());
	RW.Draw(m_Wheel1);
	RW.Draw(m_Wheel2);
	RW.Draw(m_Wheel3);
	RW.Draw(m_Wheel4);
	RW.Draw(m_Sprite);

	m_Front.SetRotation(m_Sprite.GetRotation());
	m_Front.SetPosition(m_Sprite.GetPosition());
	RW.Draw(m_Front);
}

void Vacuum::MoveLeft()
{
	m_CurrentState = "Left";

	m_Aux = m_Posicion - sf::Vector2f( 400.0f, 0.0f);
}

void Vacuum::MoveRight()
{
	m_CurrentState = "Right";
	
	m_Aux = m_Posicion + sf::Vector2f( 400.0f, 0.0f);
}

void Vacuum::Clean()
{
	m_CurrentState = "Clean";
}