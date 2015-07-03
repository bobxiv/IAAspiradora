#include <cstdlib>
#include <ctime>

#include "Game.h"

using namespace std;

int main(int argc, char* argv[])
{
	Game MyGame;//El objeto de juego principal

	srand(time(NULL));//ponemos una semilla pseudo-aleatoria

	MyGame.Iniciar();

	return 0;
}

