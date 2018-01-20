/*
# OpenCV based Image Processing Algorithm.
# GitHub: techping
# File: grayscale-transformer.cc
 */
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;

template <typename T>
class grayscale_transformer {
	public:
		template <typename functor>
		grayscale_transformer(functor func)
		{
			transform_function = func;
		}
		~grayscale_transformer()
		{
		}
		void transform(const Mat &ori_image, Mat &new_image)
		{
			if (!transform_function)
				return;
			if (ori_image.empty())
				return;
			if (new_image.empty())
				new_image.create(ori_image.size(), ori_image.type());
			for (int i = 0; i < new_image.rows; i++)
				for (int j = 0; j < new_image.cols * sizeof(T); j++)
						new_image.at<uchar>(i, j) = (*transform_function)(ori_image.at<uchar>(i, j));
		}
	private:
		uchar (*transform_function)(uchar);
};

uchar convert_(uchar gray)
{
	return 255 - gray;
}

#define LOGARITHM_VAL 2
uchar logarithm_(uchar gray)
{
	return (log(gray) / log(LOGARITHM_VAL)) / (log(255) / log(LOGARITHM_VAL)) * 255;
}

#define GAMMA_VAL 1.6
uchar gamma_(uchar gray)
{
	return pow(gray, GAMMA_VAL) / pow(255, GAMMA_VAL) * 255;
}

int main()
{
	Mat img = imread("../../img/lenna.jpg"), new_img;
	if (img.empty())
		return -1;
	grayscale_transformer<Vec3b> gt([](uchar gray)->uchar{return 255 - gray;});
	gt.transform(img, new_img);
	imshow("original", img);
	imshow("new", new_img);
	waitKey(0);
	return 0;
}
