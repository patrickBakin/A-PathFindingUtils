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
bool FindCircle(Mat& FullImage,int* StartX,int* StartY,int* EndX,int* EndY,int minDst)
{	
	std::vector<Vec3f> circles;
	HoughCircles(FullImage, circles, HOUGH_GRADIENT, 1.25, FullImage.rows/4, 105,33, 5,40);
	int i=0;
	sort(circles.begin(), circles.end(), [](const Vec3f& lhs, const Vec3f& rhs) {return cvRound(lhs[2]) > cvRound(rhs[2]); });
	
	for (i=0; i < 2; i++)
	{	
		
		std::cout << "Circle Detected!" << std::endl;
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		if (i == 0)
		{
			*StartX = center.x+radius;
			*StartY = center.y;
		}
		if (i == 1)
		{
			*EndX = center.x+radius;
			*EndY = center.y;
		}

		circle(FullImage, center, 3, Scalar(0, 255, 0), -1, 8, 0);

		circle(FullImage, center, radius, Scalar(255, 0, 255), 3, 8, 0);

		
	}
#ifdef DEBUG
	imshow("Hough Circle Transform Demo", FullImage);
	line(FullImage, Point(0, 288), Point(1024, 288), Scalar(225, 220, 225), 2, 8);
	// middle line
	line(FullImage, Point(360, 0), Point(360, 576), Scalar(225, 220, 225), 2, 8);
	//break cans into 4 by 4
	line(FullImage, Point(600, 0), Point(600, 576), Scalar(225, 220, 225), 2, 8);
	//      x, y  

	imshow("Lines", FullImage);
	imwrite("lineoutput.jpg", FullImage);


	waitKey(0);
#endif // DEBUG
	//  namedWindow("Hough Circle Transform Demo", CV_WINDOW_NORMAL);
	
	return i > 0;
}
bool CheckCircleInTile(int CircleX, int CircleY,int RectX,int RectY,int TileSizeX,int TileSizeY)
{
	return sqrtf((CircleY - RectY) * (CircleY - RectY) + (CircleX - RectX) * (CircleX - RectX)) <= sqrtf(TileSizeX * TileSizeX + TileSizeY * TileSizeY) / 2;
}
ImageMap PartitionImage(cv::Mat& image, int M, int N,int StartX,int StartY,int EndX,int EndY)
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
			//TileType StartEndTile = StartOrEndTile(RectImage);
			
			if (CheckCircleInTile(StartX, StartY, get_rect_center(roi).x, get_rect_center(roi).y, width, height) && !IM.StartEndTile[0].CoorValid)
			{	
				
				std::cout << "here" << std::endl;
				IM.StartEndTile[0].CoorValid = true;
				IM.StartEndTile[0].Type = START;
				IM.StartEndTile[0].Positionx = get_rect_center(roi).x;
				IM.StartEndTile[0].Positiony = get_rect_center(roi).y;
				IM.StartEndTile[0].TileX = j;
				IM.StartEndTile[0].TileY = i;
				IM.StartEndTile[0].CoorValid = true;
				

			}
			else if (CheckCircleInTile(EndX, EndY, get_rect_center(roi).x, get_rect_center(roi).y, width,height) && !IM.StartEndTile[1].CoorValid)
			{

				std::cout << "here11" << std::endl;
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

				inRange(RectImage, Scalar(0, 0, 0), Scalar(50, 50, 50), RectImageM);
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

void RemoveShadow(Mat* image)
{	
	
	Mat* rgb_planes = new Mat[image->channels()];
	split(*image, rgb_planes);
	std::vector<Mat> result_planes,result_norm_planes;
	for (int i=0;i<image->channels(); i++)
	{	
		Mat* plane = &rgb_planes[i];
		Mat dilated_img;
		
		dilate(*plane, dilated_img, Mat(), Point(-1, -1), 2, 1, 1);
		
		
		Mat bg_img;
		medianBlur(dilated_img,bg_img ,21);
		Mat diff_img;
		absdiff(*plane, bg_img, diff_img);
		diff_img = 255 - diff_img;
		Mat norm_img;
		normalize(diff_img, norm_img, 0, 255, NORM_MINMAX, CV_8UC1);
		result_planes.push_back(diff_img);
		result_norm_planes.push_back(norm_img);
	}
	Mat result;
	Mat result_norm;
	merge(result_planes, result);
	merge(result_norm_planes, result_norm);
	delete[] rgb_planes;
#ifdef DEBUG
	cv::imshow("shawdows_out_norm.png", result_norm);
	cv::waitKey(0);

	
#endif // DEBUG
	* image = result_norm;
	//return result_norm;
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
			Mat imageClone;
			int Paththreshold = 4;
			image = imread(ImageName, IMREAD_COLOR);
			{
				if (!image.data)
				{
					printf("No image data \n");
					continue;
				}
				imageClone = image.clone();
				RemoveShadow(&image);
				
				Mat gray_image;
				
				resize(image, gray_image, cv::Size(image.cols * 1, image.rows * 1), 0, 0, INTER_LINEAR);
				cvtColor(gray_image, gray_image, COLOR_BGR2GRAY);
				int StartX, StartY, EndX, EndY;
				
				if (!FindCircle(gray_image, &StartX, &StartY, &EndX, &EndY, image.rows / Paththreshold))
				{
					std::cout << "Circle Not Found" << std::endl;
					continue;
				}
				gray_image = gray_image > 200;
#ifdef DEBUG
				imshow("gray", gray_image);
				waitKey(0);
#endif // DEBUG
				
				
				
				//continue;
				ImageMap IM = PartitionImage(gray_image, image.cols / Paththreshold, image.rows / Paththreshold,StartX,StartY,EndX,EndY);
				if (!IM.StartEndTile[0].CoorValid || !IM.StartEndTile[1].CoorValid)
				{
					std::cout << " Can't find Start or End point" << std::endl;
					continue;
				}
				//threshold(gray_image, gray_image, 128, 255, THRESH_BINARY);
#ifdef DEBUG
				imshow("Binary", gray_image);
				waitKey(0);
#endif // DEBUG
				
				//continue;
				AStar AS;
				
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
						line(imageClone, tempPoint, p, Scalar(0, 0, 255), 2, LINE_AA);
					}
					//image.at<Vec3b>(CurrentNode->x * Paththreshold, CurrentNode->y * Paththreshold) = Vec3b(0, 0, 255);
					CurrentNode = CurrentNode->parent;
					tempPoint.x = CurrentNode->y * Paththreshold;
					tempPoint.y = CurrentNode->x * Paththreshold;
				}

			}
			imwrite("result.jpg", imageClone);

			std::cout << "Saved as result.jpg" << std::endl;
			namedWindow("Display", WINDOW_AUTOSIZE);
			imshow("Display", imageClone);
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

