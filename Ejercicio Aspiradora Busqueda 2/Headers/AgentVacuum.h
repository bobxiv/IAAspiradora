#pragma once

#include "Agent.h"

#include "Dash_Tree.h"

#include <vector>

class Vacuum;

class AgentVacuum: public Agent
{
	private:

		#pragma region Sensors States

			struct State
			{
				State(): H1(true), H2(true), H3(true), H4(true), PosIndex(-1)
					{}
				State(const State& B): H1(B.H1), H2(B.H2), H3(B.H3), H4(B.H4), PosIndex(B.PosIndex)
					{}
				State(bool h1, bool h2, bool h3, bool h4, int posIndex): H1(h1), H2(h2), H3(h3), H4(h4), PosIndex(posIndex)
					{}

				bool operator==(const State& B) const
				{
					return (H1==B.H1 && H2==B.H2 && H3==B.H3 && H4==B.H4 && PosIndex==B.PosIndex);
				}

				bool H1;
				bool H2;
				bool H3;
				bool H4;
				int  PosIndex;
			};
			
			//This agent is omnisient so he knows everything that happens in the ambient... basically
			//they have the save information
			State m_StateSensors;

		#pragma endregion

		#pragma region Actions

			//Moves to the room H1
			void _MoveH1();

			//Moves to the room H2
			void _MoveH2();

			//Moves to the room H3
			void _MoveH3();

			//Moves to the room H4
			void _MoveH4();

			//Cleans the room if it is dirty or do nothing
			void _Clean();

		#pragma endregion

		//Node in the Search tree
		//It have information of the state of the ambient un that passe
		//And the last action apply to get to that state
		class Node
		{
			public:
				Node(std::string action, AgentVacuum::State state): m_Action(action), m_State(state)
					{}
				Node(AgentVacuum::State state): m_State(state)
					{}
				Node(const Node& B): m_Action(B.m_Action), m_State(B.m_State)
					{}
				Node()
					{}

				bool operator==(const Node& B) const
				{
					if( m_Action.size() != B.m_Action.size() )
						return false;
					for(int i=0; i<m_Action.size() ;++i)
						if( m_Action[i] != B.m_Action[i] )
							return false;

					return (m_State==B.m_State);
				}

				std::string m_Action;
				AgentVacuum::State m_State;
		};
		
		//Search Tree
		Dash_Structures::DTree<Node> m_SearchT;

		//Main search call excecute the search for the action path to follow
		std::vector<Node> _Search();

		//Uses Broad search to find the node that satisfice the function _TestSolution
		void _BroadSearch(std::vector<Node>& path);

		//Help class used to expand the tree, because in this case the Succesor function always gives all the actions
		//Adds 3 sons to the iterator passed, with the following actions and corresponding states:
		// 1) Clean
		// 2) Left
		// 3) Right
		void _AddLevel(Dash_Structures::DTree<Node>::iterator it);

		//Test the node to see if that state satisfice the problem objetive
		bool _TestSolution(const AgentVacuum::Node& state);

		void _MainSaveDotFile(std::string FileName, const std::vector<Node>& path);

		void _SaveDotFile(std::ostream& output, Dash_Structures::DTree<Node>::iterator it, int deep, const std::vector<Node>& path);

		Vacuum* m_pAvatar;

	public:
		AgentVacuum(Vacuum* pAvatar);

		virtual void ExecuteSensors();

		virtual void ExecuteActions();
};