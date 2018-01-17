/*
# OpenCV based Image Processing Algorithm.
# GitHub: techping
# File: bilinear-interpolation.cc
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

	int x1, x2, y1, y2;
	Vec3b temp1, temp2;
	for (int j = 0; j < new_height; j++)
		for (int i = 0; i < new_width; i++) {
			x1 = j / Sx;
			x2 = x1 + 1;
			y1 = i / Sy;
			y2 = y1 + 1;
			if (x2 >= new_height || y2 >= new_width) {
				new_img.at<Vec3b>(j, i) = img.at<Vec3b>(x1, y1);
				continue;
			}
			temp1[0] = (x2 - j/Sx) * img.at<Vec3b>(x1, y1)[0] + (j/Sx - x1) * img.at<Vec3b>(x1, y2)[0];
			temp1[1] = (x2 - j/Sx) * img.at<Vec3b>(x1, y1)[1] + (j/Sx - x1) * img.at<Vec3b>(x1, y2)[1];
			temp1[2] = (x2 - j/Sx) * img.at<Vec3b>(x1, y1)[2] + (j/Sx - x1) * img.at<Vec3b>(x1, y2)[2];
			temp2[0] = (x2 - j/Sx) * img.at<Vec3b>(x2, y1)[0] + (j/Sx - x1) * img.at<Vec3b>(x2, y2)[0];
			temp2[1] = (x2 - j/Sx) * img.at<Vec3b>(x2, y1)[1] + (j/Sx - x1) * img.at<Vec3b>(x2, y2)[1];
			temp2[2] = (x2 - j/Sx) * img.at<Vec3b>(x2, y1)[2] + (j/Sx - x1) * img.at<Vec3b>(x2, y2)[2];
			new_img.at<Vec3b>(j, i)[0] = ((y2 - i/Sy) * temp1[0] + (i/Sy - y1) * temp2[0]);
			new_img.at<Vec3b>(j, i)[1] = ((y2 - i/Sy) * temp1[1] + (i/Sy - y1) * temp2[1]);
			new_img.at<Vec3b>(j, i)[2] = ((y2 - i/Sy) * temp1[2] + (i/Sy - y1) * temp2[2]);
		}

	imshow("original", img);
	imshow("new", new_img);
	waitKey(0);
	return 0;
}
