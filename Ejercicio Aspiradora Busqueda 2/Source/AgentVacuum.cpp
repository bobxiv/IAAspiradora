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

void AgentVacuum::_MoveH1()
{
	#if _DEBUG
		std::cout<<"Accion: Mover a H1"<< std::endl;
		std::cout<<"-------------------------------"<< std::endl;
	#endif

	m_pAvatar->MoveH1();
	AmbientVacuum* vacuumWorld = AmbientVacuum::GetInstance();
	vacuumWorld->m_RoomIndex = 1;
}

void AgentVacuum::_MoveH2()
{
	#if _DEBUG
		std::cout<<"Accion: Mover a H2"<< std::endl;
		std::cout<<"-------------------------------"<< std::endl;
	#endif

	m_pAvatar->MoveH2();
	AmbientVacuum* vacuumWorld = AmbientVacuum::GetInstance();
	vacuumWorld->m_RoomIndex = 2;
}

void AgentVacuum::_MoveH3()
{
	#if _DEBUG
		std::cout<<"Accion: Mover a H3"<< std::endl;
		std::cout<<"-------------------------------"<< std::endl;
	#endif

	m_pAvatar->MoveH3();
	AmbientVacuum* vacuumWorld = AmbientVacuum::GetInstance();
	vacuumWorld->m_RoomIndex = 3;
}

void AgentVacuum::_MoveH4()
{
	#if _DEBUG
		std::cout<<"Accion: Mover a H4"<< std::endl;
		std::cout<<"-------------------------------"<< std::endl;
	#endif

	m_pAvatar->MoveH4();
	AmbientVacuum* vacuumWorld = AmbientVacuum::GetInstance();
	vacuumWorld->m_RoomIndex = 4;
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
		m_StateSensors.H3 = vacuumWorld->m_aRoomDity[2];
		m_StateSensors.H4 = vacuumWorld->m_aRoomDity[3];
		
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
	/*
	2	create a queue Q
	3      enqueue source onto Q
	4      mark source
	5      while Q is not empty:
	6          dequeue an item from Q into v
	7          for each edge e incident on v in Graph:
	8              let w be the other end of e
	9              if w is not marked:
	10                 mark w
	11                 enqueue w onto Q
	*/
	
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
	State curState = (*it).m_State;
	int& PosIndex = curState.PosIndex;

	Dash_Structures::DTree<Node>::iterator Dadit = it;

	//Move Operators			isInside(i) ^ ~isInside(j) -> Movej -> ~isInside(i) ^ isInside(j)
	if( curState.PosIndex == 1 || curState.PosIndex == 4 ) 
	{
		it = m_SearchT.insert( it.lchild(),Node("H2", State( curState.H1,
												    curState.H2,
													curState.H3,
													curState.H4,
												    2)
									   ));

		it = m_SearchT.insert( it,Node("H3", State( curState.H1,
												    curState.H2,
													curState.H3,
													curState.H4,
												    3)
									   ));
	}else
		if( curState.PosIndex == 2 || curState.PosIndex == 3 ) 
		{
			it = m_SearchT.insert( it.lchild(),Node("H1", State( curState.H1,
														curState.H2,
														curState.H3,
														curState.H4,
														1)
										   ));

			it = m_SearchT.insert( it,Node("H4", State( curState.H1,
														curState.H2,
														curState.H3,
														curState.H4,
														4)
										   ));
		}

	//Clean Operator			isDirty(curIndex) -> Clean -> ~isDirty(curIndex)
	if( curState.PosIndex == 1 && curState.H1 == true || curState.PosIndex == 2 && curState.H2 == true ||
		curState.PosIndex == 3 && curState.H3 == true || curState.PosIndex == 4 && curState.H4 == true)
	{
		it = m_SearchT.insert( it, Node("Clean", State( (PosIndex==1)? 0:(*it).m_State.H1,
															     (PosIndex==2)? 0:(*it).m_State.H2,
																 (PosIndex==3)? 0:(*it).m_State.H3,
																 (PosIndex==4)? 0:(*it).m_State.H4,
																  PosIndex)
												));
	}

}

bool AgentVacuum::_TestSolution(const AgentVacuum::Node& state)
{
	//If the 4 rooms are clean then we have the solution
	if(state.m_State.H1 == false && state.m_State.H2 == false && state.m_State.H3 == false && state.m_State.H4 == false)
		return true;
	else
		return false;
}

void AgentVacuum::_MainSaveDotFile(std::string FileName, const std::vector<Node>& path)
{
	std::ofstream file(FileName.c_str());

	if( !file.is_open() )
		throw std::exception("No se pudo guardar el archivo dot");

	file<<"graph graphname"<<std::endl<<'{'<<std::endl;

	_SaveDotFile(file, m_SearchT.begin(), 0, path);

	file<<'}';

	file.close();
}

void AgentVacuum::_SaveDotFile(std::ostream& output, Dash_Structures::DTree<Node>::iterator it, int deep, const std::vector<Node>& path)
{
	int id = reinterpret_cast<int>(&(*it));
	
	//The name id of a node will be the concatenation of the action with a id

	if( it->m_Action == "" )
		it->m_Action = "root";

	output<< it->m_Action << id <<" [label="+it->m_Action;


	if( deep<path.size() && path[deep].m_State == it->m_State )
		output<<", color=blue]"<< std::endl;
	else
		output<<']'<< std::endl;

	//Draw it node relations
	Dash_Structures::DTree<Node>::iterator Sonit = it.lchild();
	while(Sonit != m_SearchT.end())
	{
		int Sonid = reinterpret_cast<int>(&(*Sonit));

		output<< it->m_Action << id << " -- " << (*Sonit).m_Action << Sonid<< std::endl;

		Sonit = Sonit.right();
	}

	//Expand to the childs
	Sonit = it.lchild();
	while(Sonit != m_SearchT.end())
	{
		_SaveDotFile(output, Sonit, deep+1, path);

		Sonit = Sonit.right();
	}
}

void AgentVacuum::ExecuteActions()
{
	std::vector<Node> sec = _Search();

	_MainSaveDotFile("arbol.dot", sec);

	if(sec.size() == 1)//the initial state always is in the path
		return;

	//the first element of sec if the action to apply first
	if(sec[1].m_Action == "Clean")
		_Clean();
	else
		if(sec[1].m_Action == "H1")
			_MoveH1();
		else
			if(sec[1].m_Action == "H2")
				_MoveH2();
			else
				if(sec[1].m_Action == "H3")
					_MoveH3();
				else
					if(sec[1].m_Action == "H4")
						_MoveH4();
}