#include "AgentVacuum.h"
#include "Vacuum.h"
#include "AmbientVacuum.h"

#include "iostream"
#include "fstream"

#include <queue>

#include <cstdlib>
#include <cmath>

//-------------------------------------------------------------//
//-------------	Constructor/Destructor	-----------------------//
//-------------------------------------------------------------//

AgentVacuum::AgentVacuum(Vacuum* pAvatar): m_pAvatar(pAvatar)
	{}

//-------------------------------------------------------------//
//-----------------		Methods		---------------------------//
//-------------------------------------------------------------//

void AgentVacuum::_MoveLeft()
{
	#if _DEBUG
		std::cout<<"Accion: Mover Izquierda"<< std::endl;
		std::cout<<"-------------------------------"<< std::endl;
	#endif

	m_pAvatar->MoveLeft();
	AmbientVacuum* vacuumWorld = AmbientVacuum::GetInstance();
	vacuumWorld->m_RoomIndex = _max(1, vacuumWorld->m_RoomIndex-1);
}

void AgentVacuum::_MoveRight()
{
	#if _DEBUG
		std::cout<<"Accion: Mover Derecha"<< std::endl;
		std::cout<<"-------------------------------"<< std::endl;
	#endif

	m_pAvatar->MoveRight();
	AmbientVacuum* vacuumWorld = AmbientVacuum::GetInstance();
	vacuumWorld->m_RoomIndex = _min(2, vacuumWorld->m_RoomIndex+1);
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
		//Control de limpieza
		m_StateSensors.H1 = vacuumWorld->m_aRoomDity[0];
		m_StateSensors.H2 = vacuumWorld->m_aRoomDity[1];
		
		//Control de posicion
		m_StateSensors.PosIndex = vacuumWorld->m_RoomIndex;
		/*
		#if _DEBUG
			std::cout<<"Habitacion 1:     "<< m_StateSensors.H1       << std::endl;
			std::cout<<"Habitacion 2:     "<< m_StateSensors.H2       << std::endl;
			std::cout<<"Posicion Indice:  "<< m_StateSensors.PosIndex << std::endl;
			std::cout<<"-------------------------------"<< std::endl;
		#endif
		*/
	}
}

std::vector<AgentVacuum::Node> AgentVacuum::_Search()
{
	m_SearchT.clear();
	std::vector<Node> path;
	m_SearchT.insert( m_SearchT.begin(), Node(m_StateSensors) );
	path.push_back(Node(m_StateSensors));

	//Search strategy
	_BroadSearch(path);

	return path;
}

void AgentVacuum::_BroadSearch(std::vector<Node>& path)
{
	/*2	create a queue Q
	3      enqueue source onto Q
	4      mark source
	5      while Q is not empty:
	6          dequeue an item from Q into v
	7          for each edge e incident on v in Graph:
	8              let w be the other end of e
	9              if w is not marked:
	10                 mark w
	11                 enqueue w onto Q*/
	
	std::queue<Dash_Structures::DTree<Node>::iterator> Q;

	std::queue< std::vector<Node> > Qpath;
	Qpath.push( path );

	_AddLevel(m_SearchT.begin());//Expand the tree

	Q.push(m_SearchT.begin());//starts form the root
	if( _TestSolution(*m_SearchT.begin()) )
		return;
	
	while( !Q.empty() )
	{
		Dash_Structures::DTree<Node>::iterator aux = Q.front();
		Q.pop();

		std::vector<Node> auxPath = Qpath.front();
		Qpath.pop();

		Dash_Structures::DTree<Node>::iterator it = aux.lchild();
		while(it != m_SearchT.end())
		{
			Qpath.push( auxPath );
			Qpath.back().push_back(*it);
			Q.push(it);

			if( _TestSolution(*it) )
			{
				path = Qpath.back();
				return;
			}

			_AddLevel(it);//Expand the tree

			it = it.right();
		}
	}
}

void AgentVacuum::_AddLevel(Dash_Structures::DTree<Node>::iterator it)
{
	int PosIndex= (*it).m_State.PosIndex;//The current position of the vacuum in this state

	Dash_Structures::DTree<Node>::iterator Dadit = it;

	it = m_SearchT.insert( it.lchild(), Node("Clean", State( (PosIndex==1)? 0:(*it).m_State.H1,
															 (PosIndex==2)? 0:(*it).m_State.H2,
															  PosIndex)
											));

	it = m_SearchT.insert( it,Node("Left", State( (*Dadit).m_State.H1,
												  (*Dadit).m_State.H2,
												  _max(1, PosIndex-1))
										  ));

	it = m_SearchT.insert( it,Node("Right", State( (*Dadit).m_State.H1,
												   (*Dadit).m_State.H2,
												   _min(2, PosIndex+1))
										  ));
}

std::vector<AgentVacuum::Node> AgentVacuum::_SubSearch(Dash_Structures::DTree<Node>::iterator it, std::vector<Node> path)
{
	//End condition
	if( _TestSolution(*it) )
		return path;

	if( it == m_SearchT.end() )
		return std::vector<Node>();

	//Transverse
	int PosIndex= path.back().m_State.PosIndex;
	std::vector<Node> pathA = path;
	pathA.push_back(Node("Clean", State( (PosIndex==1)? 0:path.back().m_State.H1,
										 (PosIndex==2)? 0:path.back().m_State.H2,
										  PosIndex)
						)
					);
	pathA = _SubSearch( it.right() , pathA);
	if( pathA.size() != 0 )
		return pathA;
	std::vector<Node> pathB = path;
	pathB.push_back(Node("Left", State( path.back().m_State.H1,
										path.back().m_State.H2,
										_min(1, PosIndex-1))
						)
					);
	pathB = _SubSearch( it.lchild() , pathB);
	if( pathB.size() != 0 )
		return pathB;

	//Nothing found end condition
	return std::vector<Node>();
}

bool AgentVacuum::_TestSolution(const AgentVacuum::Node& state)
{
	//If the 2 rooms are clean then we have the solution
	if(state.m_State.H1 == false && state.m_State.H2 == false)
		return true;
	else
		return false;
}

int AgentVacuum::_min(int a, int b)
{
	if( a < b )
		return a;
	else
		return b;
}

int AgentVacuum::_max(int a, int b)
{
	if( a > b )
		return a;
	else
		return b;
}

void AgentVacuum::_SaveDotFile(std::ifstream input, Dash_Structures::DTree<Node>::iterator it)
{
	input<< (*it).m_Action << id <<" [label="+m_Action+"]";

	//Draw it node relations
	Dash_Structures::DTree<Node>::iterator Sonit = it.lchild();
	while(Sonit != m_SearchT.end())
	{
		input<< (*it).m_Action << id << " -- " << (*Sonit).m_Action << Sonid;

		Sonit = Sonit.right();
	}

	//Expand to the childs
	Sonit = it.lchild();
	while(Sonit != m_SearchT.end())
	{
		_SaveDotFile(input, Sonit)

		Sonit = Sonit.right();
	}
}

void AgentVacuum::ExecuteActions()
{
	std::vector<Node> sec = _Search();

	if(sec.size() == 1)//the initial state always is in the path
		return;

	//the first element of sec if the action to apply first
	if(sec[1].m_Action == "Clean")
		_Clean();
	else
		if(sec[1].m_Action == "Left")
			_MoveLeft();
		else
			if(sec[1].m_Action == "Right")
				_MoveRight();
}