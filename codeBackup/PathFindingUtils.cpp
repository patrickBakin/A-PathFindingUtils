// PathFinding.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include "Header.h"
#include <opencv2/opencv.hpp>


using namespace cv;


int main(int argc,char** argv)
{	
	std::string UserInput="";
	while ( true)
	{	
	
		std::cout << "PathFindingUtils v1.0" << std::endl;
		std::cout << "Visualize Pathfinding or Solve Maze? (1/2) " << std::endl;
		std::cin >> UserInput;
		if (strcmp(UserInput.c_str(), "1") == 0)
		{
			std::cout << "Pathfinding Visualization" << std::endl;
			AStar AS(50, 50, 0, 0, 29, 29);


		}
		else if(strcmp(UserInput.c_str(), "2") == 0)
		{
			std::cout << "Maze Solver" << "\nPlease Specify Image path: " << std::endl;
			std::string UserInputImage;

			std::cin >> UserInputImage;
			const char* ImageName = UserInputImage.c_str();//argv[1];
			Mat image;
			image = imread(ImageName, IMREAD_COLOR);
			if (!image.data)
			{
				printf("No image data \n");
				return 0;
			}

			Mat gray_image;
			resize(image, gray_image, cv::Size(image.cols * 1, image.rows * 1), 0, 0, INTER_LINEAR);
			cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);

			AStar AS;
			AS.InitAStar(gray_image.rows, gray_image.cols, 1597, 809, 9, 793);
			std::cout <<"ImageSize: " << gray_image.rows << "x" << gray_image.cols << std::endl;
			std::cout << "Projecting Nodes to image..." << std::endl;
			for (int i = 0; i < gray_image.rows; i++)
			{
				for (int j = 0; j < gray_image.cols; j++)
				{

					int Scale = (int)gray_image.at<uchar>(i, j);
					if (Scale < 255)
					{
						AS.SetObstacleNodes(i, j);
					}

				}

			}
			bool finished = AS.Solve_AStar();

			AStar::Node* CurrentNode = AS.nodeEnd;

			while (CurrentNode->parent != nullptr)
			{
				image.at<Vec3b>(CurrentNode->x, CurrentNode->y) = Vec3b(0, 0, 255);
				CurrentNode = CurrentNode->parent;
			}
			imwrite("result.jpg", image);
			std::cout << "Saved as result.jpg" << std::endl;
		}
		else
		{
			break;
		}


		
		
		system("cls");
	}
	return 0;
}

