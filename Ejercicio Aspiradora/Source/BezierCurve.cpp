#include "BezierCurve.h"

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

BezierCurve::BezierCurve(const sf::Vector2f& P0,const sf::Vector2f& M0, const sf::Vector2f& M1, const sf::Vector2f& P1): 
	m_P0(P0), m_M0(M0), m_M1(M1), m_P1(P1)
	{}

//-------------------------------------------------------------//
//-----------------		Methods		---------------------------//
//-------------------------------------------------------------//

sf::Vector2f BezierCurve::Evaluar(float t) const
{
	return sf::Vector2f(0,0);
}