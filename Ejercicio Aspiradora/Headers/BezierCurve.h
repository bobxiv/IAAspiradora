#pragma once

//Inludes de SFML
#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"


//Una curva de Bezier Cuadratica de forma parametrica con parametro t=[0,1]
class BezierCurve
{
	private:

		sf::Vector2f m_P0;

		sf::Vector2f m_M0;

		sf::Vector2f m_M1;

		sf::Vector2f m_P1;

	public:

		BezierCurve(const sf::Vector2f& P0,const sf::Vector2f& M0, const sf::Vector2f& M1, const sf::Vector2f& P1);

		sf::Vector2f Evaluar(float t) const;
};