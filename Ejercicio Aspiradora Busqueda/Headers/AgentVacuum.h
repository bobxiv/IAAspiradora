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
				State(): H1(true), H2(true), PosIndex(-1)
					{}
				State(const State& B): H1(B.H1), H2(B.H2), PosIndex(B.PosIndex)
					{}
				State(bool h1, bool h2, int posIndex): H1(h1), H2(h2), PosIndex(posIndex)
					{}

				bool H1;
				bool H2;
				int  PosIndex;
			};
			
			//This agent is omnisient so he knows everything that happens in the ambient... basically
			//they have the save information
			State m_StateSensors;

		#pragma endregion

		#pragma region Actions

			//Moves to the room in the left if it cans or do nothing
			void _MoveLeft();

			//Moves to the room in the right if it cans or do nothing
			void _MoveRight();

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

		std::vector<Node> _SubSearch(Dash_Structures::DTree<Node>::iterator it, std::vector<Node> path);

		//Test the node to see if that state satisfice the problem objetive
		bool _TestSolution(const AgentVacuum::Node& state);

		//Returns the minimum of the two numbers
		int _min(int a, int b);

		//Returns the maximum of the two numbers
		int _max(int a, int b);

		void _SaveDotFile(std::ifstream input, Dash_Structures::DTree<Node>::iterator it);

		Vacuum* m_pAvatar;

	public:
		AgentVacuum(Vacuum* pAvatar);

		virtual void ExecuteSensors();

		virtual void ExecuteActions();
};