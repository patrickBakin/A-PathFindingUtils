#include "Header.h"




void AStar::MainWindow()
{
	bool Draw = false;
	while (this->window->isOpen())
	{
		while (this->window->pollEvent(ev))
		{
			switch (ev.type)
			{
			case sf::Event::Closed:
				this->window->close();
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::Escape)
				{
					this->window->close();
				}
				if (ev.key.code == sf::Keyboard::S)
				{

					Draw = Solve_AStar();
				}
				if (ev.key.code == sf::Keyboard::Up || ev.key.code == sf::Keyboard::Down || ev.key.code == sf::Keyboard::Right || ev.key.code == sf::Keyboard::Left)
				{
					//Draw = false;
					ChangeDestination(ev.key.code);
				}
				if (ev.key.code == sf::Keyboard::R)
				{
					ReGenObstacles();
				}
			default:
				break;
			}
		}
		this->window->clear();


		DrawNodes(Draw);



		//draw Here
		this->window->display();
	}
}

void AStar::InitAStar(int NumNodeX, int NumNodeY, int NodeStartX, int NodeStartY, int NodeEndX, int NodeEndY) //asd
{
	this->NumNodeX = NumNodeX;
	this->NumNodeY = NumNodeY;
	nodes = new Node[NumNodeX * NumNodeY];

	//Initialize Nodes
	for (int x = 0; x < NumNodeX; x++)
	{
		for (int y = 0; y < NumNodeY; y++)
		{
			nodes[y * NumNodeX + x].x = x; // ...because we give each node its own coordinates
			nodes[y * NumNodeX + x].y = y;
			nodes[y * NumNodeX + x].bObstacle = false;
			nodes[y * NumNodeX + x].parent = nullptr;
			nodes[y * NumNodeX + x].bVisited = false;



			if (x == NodeStartX && y == NodeStartY)
			{
				nodes[y * NumNodeX + x].nodetype = START_NODE;

				this->nodeStart = &nodes[y * NumNodeX + x];
			}
			else if (x == NodeEndX && y == NodeEndY)
			{
				nodes[y * NumNodeX + x].nodetype = END_NODE;

				this->nodeEnd = &nodes[y * NumNodeX + x];
			}
		}
	}


	//create connections NodeNeighBours
	for (int x = 0; x < NumNodeX; x++)
		for (int y = 0; y < NumNodeY; y++)
		{
			if (y > 0)
				nodes[y * NumNodeX + x].NodeNeighbours.push_back(&nodes[(y - 1) * NumNodeX + (x + 0)]);
			if (y < NumNodeY - 1)
				nodes[y * NumNodeX + x].NodeNeighbours.push_back(&nodes[(y + 1) * NumNodeX + (x + 0)]);
			if (x > 0)

				nodes[y * NumNodeX + x].NodeNeighbours.push_back(&nodes[(y + 0) * NumNodeX + (x - 1)]);

			if (x < NumNodeX - 1)

				nodes[y * NumNodeX + x].NodeNeighbours.push_back(&nodes[(y + 0) * NumNodeX + (x + 1)]);


		}


}


bool AStar::SetObstacleNodes(int x, int y)
{
	nodes[y * NumNodeX + x].bObstacle = true;
	nodes[y * NumNodeX + x].nodetype = OBST_NODE;
	return true;
}

bool AStar::GenerateAnObstacle()
{

	int rangeX = NumNodeX - 2 + 1, rangeY = NumNodeY - 2 + 1;
	int numX = rand() % rangeX, numY = rand() % rangeY;

	if (nodes[numY * NumNodeX + numX].nodetype == START_NODE || nodes[numY * NumNodeX + numX].nodetype == END_NODE) return false;
	nodes[numY * NumNodeX + numX].bObstacle = true;
	nodes[numY * NumNodeX + numX].nodetype = OBST_NODE;
	return true;
}

bool AStar::GenerateSquareObstacle(int SizeN)
{
	//srand(time(0));
	int rangeX = NumNodeX - 2 + 1, rangeY = NumNodeY - 2 + 1;
	int numX = rand() % (rangeX), numY = rand() % (rangeY);


	for (int y = numY; y <= numY + SizeN; y++)
	{
		for (int x = numX; x <= numX + SizeN; x++)
		{
			if (x >= NumNodeX || y >= NumNodeY) continue;
			std::cout << x << " " << y << std::endl;
			if (nodes[y * NumNodeX + x].nodetype == START_NODE || nodes[y * NumNodeX + x].nodetype == END_NODE) continue;
			nodes[y * NumNodeX + x].bObstacle = true;
			nodes[y * NumNodeX + x].nodetype = OBST_NODE;
		}

	}

	return true;
}

float AStar::GetGlobalDistance(Node* nodeA, Node* nodeB)
{
	return sqrtf((nodeA->x - nodeB->x) * (nodeA->x - nodeB->x) + (nodeA->y - nodeB->y) * (nodeA->y - nodeB->y));
}

bool AStar::Solve_AStar()
{
	ResetAllNodes();
	Node* CurrentNode = nodeStart;
	nodeStart->fLocalGoal = 0.0f;
	nodeStart->fGlobalGoal = GetGlobalDistance(nodeStart, nodeEnd);
	std::vector<Node*> NodesToTestList;



	NodesToTestList.push_back(nodeStart);

	while (!NodesToTestList.empty() && CurrentNode != nodeEnd)
	{
		std::sort(NodesToTestList.begin(),
			NodesToTestList.end(),
			[](const Node* lhs, const Node* rhs)
			{return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		while (!NodesToTestList.empty() && NodesToTestList.front()->bVisited)

		{
			NodesToTestList.erase(NodesToTestList.begin());
		}
		if (NodesToTestList.empty())
		{
			break;
		}
		CurrentNode = NodesToTestList.front();
		CurrentNode->bVisited = true;

		for (auto& nodeNeighbour : CurrentNode->NodeNeighbours)
		{

			if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
				NodesToTestList.push_back(nodeNeighbour);


			float fPossiblyLowerGoal = CurrentNode->fLocalGoal + GetGlobalDistance(CurrentNode, nodeNeighbour);

			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
			{
				nodeNeighbour->parent = CurrentNode;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + GetGlobalDistance(nodeNeighbour, nodeEnd);
			}
		}
	}


	return nodeEnd->parent != nullptr;

}

void AStar::DrawNodes(bool DrawPath)
{
	float Radius = 2;
	float X = 0, Y = 0;
	float Dx = (1024 - NumNodeX * Radius) / (NumNodeX + 1);
	float Dy = (1024 - NumNodeY * Radius) / (NumNodeY + 1);
	for (int x = 0; x < NumNodeX; x++)
	{
		for (int y = 0; y < NumNodeY; y++)
		{
			X += Dx;
			(this->nodes)[y * NumNodeX + x].aNode.setPosition(X, Y);
			(this->nodes)[y * NumNodeX + x].PosX = (this->nodes)[y * NumNodeX + x].aNode.getPosition().x;
			(this->nodes)[y * NumNodeX + x].PosY = (this->nodes)[y * NumNodeX + x].aNode.getPosition().y;
			(this->nodes)[y * NumNodeX + x].aNode.setRadius(Radius);

			switch ((this->nodes)[y * NumNodeX + x].nodetype)
			{
			case START_NODE:
				(this->nodes)[y * NumNodeX + x].aNode.setFillColor(sf::Color::Yellow);
				break;
			case END_NODE:
				(this->nodes)[y * NumNodeX + x].aNode.setFillColor(sf::Color::Green);
				break;
			case OBST_NODE:
				(this->nodes)[y * NumNodeX + x].aNode.setFillColor(sf::Color::Red);
				break;
			case JUST_NODE:
				(this->nodes)[y * NumNodeX + x].aNode.setFillColor(sf::Color::Blue);
				break;
			default:
				break;
			}

			if (DrawPath)
			{
				if (IsInPath(&(this->nodes)[y * NumNodeX + x], this->nodeEnd))
				{

					(this->nodes)[y * NumNodeX + x].aNode.setFillColor(sf::Color::White);
				}
			}
			this->window->draw((this->nodes)[y * NumNodeX + x].aNode);
		}
		Y += Dy;
		X = 0;
	}


}


bool AStar::ChangeDestination(sf::Keyboard::Key key)
{
	switch (key)
	{
	case sf::Keyboard::Up:
		if (!(nodeEnd->x <= 0 || nodes[(nodeEnd->y) * NumNodeX + nodeEnd->x - 1].bObstacle))
		{
			nodeEnd->nodetype = JUST_NODE;
			nodeEnd = &nodes[(nodeEnd->y) * NumNodeX + nodeEnd->x - 1];
			nodeEnd->nodetype = END_NODE;
		}
		break;
	case sf::Keyboard::Down:
		if (!(nodeEnd->x >= NumNodeY - 1 || nodes[(nodeEnd->y) * NumNodeX + nodeEnd->x + 1].bObstacle))
		{
			nodeEnd->nodetype = JUST_NODE;
			nodeEnd = &nodes[(nodeEnd->y) * NumNodeX + nodeEnd->x + 1];
			nodeEnd->nodetype = END_NODE;
		}
		break;
	case sf::Keyboard::Left:
		if (!(nodeEnd->y <= 0 || nodes[(nodeEnd->y - 1) * NumNodeX + nodeEnd->x].bObstacle))
		{
			nodeEnd->nodetype = JUST_NODE;
			nodeEnd = &nodes[(nodeEnd->y - 1) * NumNodeX + nodeEnd->x];
			nodeEnd->nodetype = END_NODE;
		}
		break;
	case sf::Keyboard::Right:
		if (!(nodeEnd->y >= NumNodeY - 1 || nodes[(nodeEnd->y + 1) * NumNodeX + nodeEnd->x].bObstacle))
		{
			nodeEnd->nodetype = JUST_NODE;
			nodeEnd = &nodes[(nodeEnd->y + 1) * NumNodeX + nodeEnd->x];
			nodeEnd->nodetype = END_NODE;
		}
		break;
	default:
		break;
	}
	return Solve_AStar();
	//return true;
}

bool AStar::IsInPath(Node* Node1, Node* Node2)
{
	Node* parent = Node2;
	while (parent->parent != nullptr)
	{
		if (Node1 == parent && Node1 != Node2)
		{
			return true;
		}
		parent = parent->parent;
	}

	return false;
}

void AStar::ResetAllNodes()
{
	for (int x = 0; x < NumNodeX; x++)
	{
		for (int y = 0; y < NumNodeY; y++)
		{
			
			nodes[y * NumNodeX + x].bVisited = false;
			nodes[y * NumNodeX + x].fGlobalGoal = INFINITY;
			nodes[y * NumNodeX + x].fLocalGoal = INFINITY;
			nodes[y * NumNodeX + x].parent = nullptr;	// No parents
			

		}
	}
}

void AStar::GenerateMapObstacles()
{
	for (int i = 0; i <= 50; i++)
	{
		GenerateSquareObstacle(3);
		GenerateAnObstacle();
		GenerateAnObstacle();
		GenerateAnObstacle();
		//GenerateSquareObstacle(2);
	}
}
void AStar::ReGenObstacles()
{
	for (int x = 0; x < NumNodeX; x++)
	{
		for (int y = 0; y < NumNodeY; y++)
		{
			nodes[y * NumNodeX + x].nodetype = JUST_NODE;
			nodes[y * NumNodeX + x].bObstacle = false;


		}
	}
	GenerateMapObstacles();
	
}