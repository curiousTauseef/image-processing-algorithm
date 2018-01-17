/*
# OpenCV based Image Processing Algorithm.
# GitHub: techping
# File: bicubic-interpolation.cc
 */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>

using namespace cv;
using namespace std;

/*
The BiCubic Polynomial:
		(a + 2) * abs(x) ^ 3 - (a + 3) * abs(x) ^ 2 + 1, for abs(x) <= 1
W(x) =  a * abs(x) ^ 3 - 5 * a * abs(x) ^ 2 + 8 * a * abs(x) - 4 * a, for 1 < abs(x) < 2
		0, otherwise
 */
float bicubic_polynomial(float x)
{
    float abs_x = abs(x);
    float a = -0.5;/* We assumed that a is -0.5. */
    if (abs_x <= 1.0)
        return (a + 2) * pow(abs_x, 3) - (a + 3) * pow(abs_x, 2) + 1;
    else if (abs_x < 2.0)
        return a * pow(abs_x, 3) - 5 * a * pow(abs_x, 2) + 8 * a * abs_x - 4 * a;
    else
        return 0.0;
}

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

	for (int j = 0; j < new_height; j++)
		for (int i = 0; i < new_width; i++) {
			Vec3b sum(0, 0, 0);
			int x = j / Sx;
			int y = i / Sy;
			if (x - 2 < 0 || y - 2 < 0 || x + 3 >= new_height || y + 3 >= new_width) {
				new_img.at<Vec3b>(j, i) = img.at<Vec3b>(x, y);
				continue;
			}
			for (int l = -1; l < 3; l++)
				for (int m = -1; m < 3; m++)
					for (int n = 0; n < 3; n++)
						sum[n] += img.at<Vec3b>(x + l, y + m)[n] * bicubic_polynomial(-l) * bicubic_polynomial(-m);
			new_img.at<Vec3b>(j, i) = sum;
		}

	imshow("original", img);
	imshow("new", new_img);
	waitKey(0);
	return 0;
}
