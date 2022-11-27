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
enum TileType
{
	START,
	END,
	OBTACLE,
	NONE

};
struct Tile
{	
	bool CoorValid=false;
	int Positionx;
	int Positiony;
	int TileX;
	int TileY;
	TileType Type= NONE;
};
struct ImageMap
{
	int width;
	int Height;
	Tile StartEndTile[2];
	std::vector<Tile> ObstacleTile;

};
/*Coor getStartEndPoint(Mat& image)
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

}*/


Point get_rect_center(Rect& tile)
{	
	return Point(tile.br() + tile.tl()) * 0.5;
}

TileType StartOrEndTile(Mat& tile)
{	

	Mat getRedImage;

	//inRange(tile, Scalar(0, 0, 255), Scalar(0, 0, 128), getRedImage);
	//Mat1b mask1, mask2;
	//inRange(tile, Scalar(0, 70, 50), Scalar(10, 255, 255), mask1);
	//inRange(tile, Scalar(170, 70, 50), Scalar(180, 255, 255), mask2);

	//Mat1b mask = mask1 | mask2;
	inRange(tile, Scalar(10, 10, 100), Scalar(100, 100, 255), getRedImage);
	
	
		int nonZero = countNonZero(getRedImage);
		if (nonZero > 0)
		{

			return START;
		}
		else
		{

			Mat getBlueImage;
			inRange(tile, Scalar(100, 0, 0), Scalar(255, 80, 80), getBlueImage);
			nonZero = countNonZero(getBlueImage);
			if (nonZero > 0)
			{

				return END;

			}
			else
			{
				return NONE;
			}
		}
	
	
	
}


ImageMap PartitionImage(cv::Mat& image, int M, int N)
{
	// All images should be the same size ...
	int width = image.cols / M;
	int height = image.rows / N;
	// ... except for the Mth column and the Nth row
	int width_last_column = width + (image.cols % width);
	int height_last_row = height + (image.rows % height);

	
	ImageMap IM;
	IM.width = M;
	IM.Height = N;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			// Compute the region to crop from
			cv::Rect roi(width * j,
				height * i,
				(j == (M - 1)) ? width_last_column : width,
				(i == (N - 1)) ? height_last_row : height);
			
			
			Mat RectImage = image(roi);
			//resize(RectImage, RectImage, Size(256, 256));
			/*namedWindow("Display", WINDOW_NORMAL);
			imshow("Display", RectImage);
			waitKey(0);*/
			TileType StartEndTile = StartOrEndTile(RectImage);
			if (StartEndTile==START)
			{	
				

				IM.StartEndTile[0].CoorValid = true;
				IM.StartEndTile[0].Type = START;
				IM.StartEndTile[0].Positionx = get_rect_center(roi).x;
				IM.StartEndTile[0].Positiony = get_rect_center(roi).y;
				IM.StartEndTile[0].TileX = j;
				IM.StartEndTile[0].TileY = i;
				IM.StartEndTile[0].CoorValid = true;
				

			}
			else if (StartEndTile == END)
			{


				IM.StartEndTile[1].CoorValid = true;
				IM.StartEndTile[1].Type = END;
				IM.StartEndTile[1].Positionx = get_rect_center(roi).x;
				IM.StartEndTile[1].Positiony = get_rect_center(roi).y;
				IM.StartEndTile[1].TileX = j;
				IM.StartEndTile[1].TileY = i;
				IM.StartEndTile[1].CoorValid = true;
		

			}
			else
			{
				Mat RectImageM;

				inRange(RectImage, Scalar(0, 0, 0), Scalar(105, 105, 105), RectImageM);
				if (countNonZero(RectImageM) > 0)
				{
					Tile tile;
					tile.CoorValid = true;
					tile.Type = OBTACLE;
					tile.Positionx = get_rect_center(roi).x;
					tile.Positiony = get_rect_center(roi).y;
					tile.TileX = j;
					tile.TileY = i;
					IM.ObstacleTile.push_back(tile);
				
				}
			}
				
			
		}
	}

	return IM;

	
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
		else if (strcmp(UserInput.c_str(), "2") == 0)
		{
			std::cout << "Maze Solver" << "\nPlease Specify Image path: " << std::endl;
			std::string UserInputImage;

			std::cin >> UserInputImage;
			const char* ImageName = UserInputImage.c_str();//argv[1];
			Mat image;
			int Paththreshold = 4;
			image = imread(ImageName, IMREAD_COLOR);
			{
				if (!image.data)
				{
					printf("No image data \n");
					continue;
				}

				ImageMap IM = PartitionImage(image, image.cols / Paththreshold, image.rows / Paththreshold);
				if (!IM.StartEndTile[0].CoorValid || !IM.StartEndTile[1].CoorValid)
				{
					std::cout << " Can't find Start or End point" << std::endl;
					continue;
				}
				Mat gray_image;
				resize(image, gray_image, cv::Size(image.cols * 1, image.rows * 1), 0, 0, INTER_LINEAR);
				cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);

				AStar AS;
				//printf("%d %d %d %d\n", StartEndCoor.Start.y, StartEndCoor.Start.x, StartEndCoor.End.y, StartEndCoor.End.x);
				AS.InitAStar(IM.Height, IM.width, IM.StartEndTile[1].TileY, IM.StartEndTile[1].TileX, IM.StartEndTile[0].TileY, IM.StartEndTile[0].TileX);
				
				std::cout << "Projecting Nodes to image..." << std::endl;
				
				for (const Tile& tile : IM.ObstacleTile)
				{	
					
					AS.SetObstacleNodes(tile.TileY, tile.TileX);
				}
				auto start = std::chrono::high_resolution_clock::now();
				bool finished = AS.Solve_AStar();
				auto stop = std::chrono::high_resolution_clock::now();
				auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
				std::cout << "Solving Time: " << duration.count()<< " microseconds" << std::endl;

				std::string PathFound = finished ? "Path Found!" : "Path not found";
				std::cout <<  PathFound << std::endl;
				AStar::Node* CurrentNode = AS.nodeEnd;
				Point tempPoint(-1,-1);
				while (CurrentNode->parent != nullptr)
				{	
					Point p(CurrentNode->y * Paththreshold, CurrentNode->x * Paththreshold);
					if (tempPoint.x != -1 && tempPoint.y != -1)
					{
						line(image, tempPoint, p, Scalar(0, 0, 255), 2, LINE_AA);
					}
					//image.at<Vec3b>(CurrentNode->x * Paththreshold, CurrentNode->y * Paththreshold) = Vec3b(0, 0, 255);
					CurrentNode = CurrentNode->parent;
					tempPoint.x = CurrentNode->y * Paththreshold;
					tempPoint.y = CurrentNode->x * Paththreshold;
				}

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

