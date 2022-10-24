// PathFinding.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

#include "Header.h"
#include <opencv2/opencv.hpp>


using namespace cv;


int main(int argc,char** argv)
{	
	const char* ImageName = "maze3.png";//argv[1];
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
	AS.InitAStar(gray_image.rows, gray_image.cols,1597,809,9,793);
	std::cout << gray_image.rows << " " << gray_image.cols << std::endl;
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
		image.at<Vec3b>(CurrentNode->x, CurrentNode->y) = Vec3b(0, 255, 255);
		CurrentNode = CurrentNode->parent;
	}
	imwrite("result.jpg", image);
	
	/*
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
	//AS.MainWindow();
	*/
	
	return 0;
}

