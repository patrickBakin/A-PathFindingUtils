// PathFinding.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

#include "Header.h"



int main()
{	
	//GG gg;
	
	int NumNodeX, NumNodeY, StartX, StartY, EndX, EndY;
	std::string options;
	std::cout << "NumNodeX"<<std::endl;
	std::cin >> NumNodeX;
	std::cout << "NumNodeY" << std::endl;
	std::cin >> NumNodeY;
	std::cout << "StartX" << std::endl;
	std::cin >> StartX;
	std::cout << "StartY" << std::endl;
	std::cin >> StartY;
	std::cout << "EndX" << std::endl;
	std::cin >> EndX;
	std::cout << "EndY" << std::endl;
	std::cin >> EndY;
	
	
	AStar AS;
	AS.InitAStar(NumNodeX, NumNodeY,StartX,StartY,EndX,EndY);
	AS.MainWindow();
	
	
	return 0;
}

