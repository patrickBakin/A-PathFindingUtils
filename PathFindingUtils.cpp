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

struct Coor
{	
	bool CoorValid=false;
	struct Startpoint
	{
		int x=0;
		int y=0;
	} Start;
	struct Endpoint
	{
		int x=0;
		int y=0;
	} End;
};

Coor getStartEndPoint(Mat& image)
{	
	Coor PointCoor;
	
	Mat getRedImage;
	inRange(image, Scalar(10, 10, 100), Scalar(100, 100, 255), getRedImage);
	int nonZero = countNonZero(getRedImage);
	if (nonZero <= 0)
	{
		printf("The Image doesn't contain a start point.\n");
		
		return PointCoor;
	}
	Mat RednonZeroCoordinates;
	findNonZero(getRedImage, RednonZeroCoordinates);
	int i;
	for (i = 0; i < RednonZeroCoordinates.total(); i++) {
		//std::cout << "Zero#" << i << ": " << nonZeroCoordinates.at<Point>(i).x << ", " << nonZeroCoordinates.at<Point>(i).y << endl;
		break;
	}


	Mat getBlueImage;

	inRange(image, Scalar(100, 0, 0), Scalar(255, 80, 80), getBlueImage);
	nonZero = countNonZero(getBlueImage);
	if (nonZero <= 0)
	{
		printf("The Image doesn't contain an End point.\n");
		return PointCoor;
		
	}
	Mat BluenonZeroCoordinates;
	findNonZero(getBlueImage, BluenonZeroCoordinates);
	int j;
	for (j = 0; j < BluenonZeroCoordinates.total(); j++) {
		//std::cout << "Zero#" << i << ": " << nonZeroCoordinates.at<Point>(i).x << ", " << nonZeroCoordinates.at<Point>(i).y << endl;
		break;
	}

	PointCoor.CoorValid = true;
	PointCoor.Start.x = RednonZeroCoordinates.at<Point>(i).x;
	PointCoor.Start.y = RednonZeroCoordinates.at<Point>(i).y;
	PointCoor.End.x = BluenonZeroCoordinates.at<Point>(j).x;
	PointCoor.End.y = BluenonZeroCoordinates.at<Point>(j).y;
	
	return PointCoor;

}
int main(int argc,char** argv)
{	
	
	std::string UserInput="";
	while ( true)
	{	
		std::cout << R"(
  ___      _   _    ___ _         _ _             _   _ _   _ _    
 | _ \__ _| |_| |_ | __(_)_ _  __| (_)_ _  __ _  | | | | |_(_) |___
 |  _/ _` |  _| ' \| _|| | ' \/ _` | | ' \/ _` | | |_| |  _| | (_-<
 |_| \__,_|\__|_||_|_| |_|_||_\__,_|_|_||_\__, |  \___/ \__|_|_/__/
                                          |___/                    
)";
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
				continue;
			}
			Coor StartEndCoor = getStartEndPoint(image);
			if (!StartEndCoor.CoorValid)
			{
				printf("Start or End Spot Position is not valid\n");
				continue;
			}
			Mat gray_image;
			resize(image, gray_image, cv::Size(image.cols * 1, image.rows * 1), 0, 0, INTER_LINEAR);
			cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);

			AStar AS;
			printf("%d %d %d %d\n", StartEndCoor.Start.y, StartEndCoor.Start.x, StartEndCoor.End.y, StartEndCoor.End.x);
			AS.InitAStar(gray_image.rows, gray_image.cols, StartEndCoor.End.y, StartEndCoor.End.x, StartEndCoor.Start.y, StartEndCoor.Start.x);
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
			imshow("result", image);
			waitKey(0);
			
		}
		else
		{
			break;
		}


		
		
		system("cls");
	}
	return 0;
}

