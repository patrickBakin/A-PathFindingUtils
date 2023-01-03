#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include<cstdlib>

class GG
{
public:
	GG()
	{
		this->window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "PathFindingUtils", sf::Style::Titlebar | sf::Style::Close);
		this->window->setFramerateLimit(60);
	}
	GG(bool noGui) {  };
	
	

	virtual ~GG()
	{	
		std::cout << "Destroyed Window" << std::endl;
		delete this->window;
	}

	

protected:

	virtual void MainWindow();
	sf::RenderWindow* window;
	sf::Event ev;
};


class AStar : public GG
{

public:

	enum NodeType
	{
		START_NODE,
		END_NODE,
		OBST_NODE,
		JUST_NODE
		
	};

	struct Node
	{
		NodeType nodetype = JUST_NODE;
		bool bObstacle = false;			// Is the node an obstruction?
		bool bVisited = false;			// Have we searched this node before?
		float fGlobalGoal;				// Distance to goal so far
		float fLocalGoal;				// Distance to goal if we took the alternative route
		int x;							// Nodes position in 2D space
		int y;
		float PosX, PosY;
		std::vector<Node*> NodeNeighbours;	// Connections to neighbours
		Node* parent;
		sf::CircleShape aNode;
		
		
		bool operator<= (const Node* other) const {
			return fGlobalGoal <= other->fGlobalGoal;
		}
	};

	Node* nodes = nullptr;
	Node* nodeStart = nullptr;
	Node* nodeEnd = nullptr;
	
	int NumNodeX;
	int NumNodeY;

	void MainWindow() override;
	AStar() : GG(false) {};
	

	void GenerateMapObstacles();
	AStar(int NumNodeX, int NumNodeY, int NodeStartX, int NodeStartY, int NodeEndX, int NodeEndY) : GG()
	{
	
		InitAStar(NumNodeX, NumNodeY, NodeStartX, NodeStartY, NodeEndX, NodeEndY);
		GenerateMapObstacles();
		MainWindow();
		
	};

	void InitAStar(int NumNodeX, int NumNodeY, int NodeStartX, int NodeStartY, int NodeEndX, int NodeEndY); //asd

	bool SetObstacleNodes(int x, int y);

	bool GenerateAnObstacle();

	bool GenerateSquareObstacle(int SizeN);

	float GetGlobalDistance(Node* nodeA, Node* nodeB);
	
	bool Solve_AStar();
	

	
	void DrawNodes(bool DrawPath);
	
	bool ChangeDestination(sf::Keyboard::Key key);
	
	bool IsInPath(Node* Node1, Node* Node2);

	void ReGenObstacles();

	

	~AStar() override
	{
		EndGame();

	}


private:
	void ResetAllNodes();

	void EndGame()
	{
		delete[] this->nodes;
	}
};



	


