#include "Vacuum.h"
#include "AgentVacuum.h"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

Vacuum::Vacuum(): m_Posicion(100.0f,100.0f)
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

	if( m_CurrentState == "Left" )
	{
		m_Angulo = m_Sprite.GetRotation() + dt*ANGULAR_VEL;
		if( m_Angulo > m_Aux.x )
		{
			m_Angulo = m_Aux.x;
			m_Angulo = ((int)m_Angulo)%360;

			m_pAgent->ExecuteSensors();
			m_pAgent->ExecuteActions();
		}
		m_Sprite.SetRotation( m_Angulo );
		return;
	}

	if( m_CurrentState == "Right" )
	{
		m_Angulo = m_Sprite.GetRotation() - dt*ANGULAR_VEL;
		if( m_Angulo < m_Aux.x )
		{
			m_Angulo = m_Aux.x;
			if( m_Angulo < 0.0f )
				m_Angulo = 360 + m_Angulo;

			m_pAgent->ExecuteSensors();
			m_pAgent->ExecuteActions();
		}
		m_Sprite.SetRotation( m_Angulo );
		return;
	}

	if( m_CurrentState == "Forward" )
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

void Vacuum::TurnLeft()
{
	m_CurrentState = "Left";
	m_Aux.x = m_Angulo + 90.0f;
}

void Vacuum::TurnRight()
{
	m_CurrentState = "Right";
	m_Aux.x = (m_Angulo - 90.0f);
}

void Vacuum::MoveForward()
{
	m_CurrentState = "Forward";

	sf::Vector2<float> dir(cos(m_Angulo*3.1415f/180), -sin(m_Angulo*3.1415f/180));
	m_Aux = m_Posicion + dir*400.0f;
}

void Vacuum::Clean()
{
	m_CurrentState = "Clean";
}