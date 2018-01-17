/*
# OpenCV based Image Processing Algorithm.
# GitHub: techping
# File: nearest-neighbor-interpolation.cc
 */
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("../../img/lenna.jpg");
	if (img.empty())
		return -1;
	cout << "[Original] height:" << img.rows << "; width :" << img.cols << ";" << endl;

	cout << "Please input the Sx and Sy:" << endl;
	float Sx, Sy;
	cin >> Sx >> Sy;
	cout << "Sx = " << Sx << "; Sy = " << Sy << ";" << endl;

	Mat new_img;
	int new_height = img.rows * Sx, new_width = img.cols * Sy;
	new_img.create(new_height, new_width, CV_8UC3);
	cout << "[New] height:" << new_height << "; width :" << new_width << ";" << endl;

	for(int j = 0; j < new_height; j++)
		for(int i = 0; i < new_width; i++)
			new_img.at<Vec3b>(j, i) = img.at<Vec3b>((int)(j / Sx), (int)(i / Sy));

	imshow("original", img);
	imshow("new", new_img);
	waitKey(0);
	return 0;
}
