/*
# OpenCV based Image Processing Algorithm.
# GitHub: techping
# File: perspective-transformer.cc
 */
#include <opencv2/opencv.hpp>
#include <cmath>

template <typename T>
class perspective_transformer {
	public:
		perspective_transformer(cv::Mat& matrix): perspective_matrix(matrix)
		{ }
		void transform(const cv::Mat& original_image, cv::Mat& output_image)
		{
			if (original_image.empty())
				return;
			if (output_image.empty())
				output_image.create(original_image.size(), original_image.type());
			for (int i = 0; i < original_image.rows; i++)
				for (int j = 0; j < original_image.cols * sizeof(T); j++) {
					int ii = (perspective_matrix.at<double>(0, 0) * i + perspective_matrix.at<double>(1, 0) * (j  / sizeof(T)) + perspective_matrix.at<double>(2, 0)) / (perspective_matrix.at<double>(0, 2) * i + perspective_matrix.at<double>(1, 2) * (j  / sizeof(T)) + perspective_matrix.at<double>(2, 2));
					int jj = (perspective_matrix.at<double>(0, 1) * i + perspective_matrix.at<double>(1, 1) * (j  / sizeof(T)) + perspective_matrix.at<double>(2, 1)) / (perspective_matrix.at<double>(0, 2) * i + perspective_matrix.at<double>(1, 2) * (j  / sizeof(T)) + perspective_matrix.at<double>(2, 2));
					if (ii < 0 || jj < 0 || ii >= original_image.rows || jj >= original_image.cols) {
						j += (sizeof(T) - 1);
						continue;
					}
					for (int k = 0; k < sizeof(T); k++) {
						if (k > 0)
							j++;
						output_image.at<uchar>(ii, jj * sizeof(T) + k) = original_image.at<uchar>(i, j);
					}
				}
		}
	private:
		/*
		 * cv::Mat perspective_matrix
		 * This is a 3*3 matrix:
		 * | t11 t12 t13 |
		 * | t21 t22 t23 |
		 * | t31 t32 t33 | (t33 === 1)
		 * The coordinate transform as:
		 * x = (t11 * v + t21 * w + t31) / (t13 * v + t23 * w + t33)
		 * y = (t12 * v + t22 * w + t32) / (t13 * v + t23 * w + t33)
		 * (v, w) is the old coordinate.
		 * (x, y) is the new coordinate.
		 */
		cv::Mat perspective_matrix;
};

#define PI 3.14159265

int main()
{
	cv::Mat img = cv::imread("../../img/lenna.jpg"), new_img;
	if (img.empty())
		return -1;
	cv::Mat matrix = (cv::Mat_<double>(3, 3) << 0.5,   0, 0, 
												0,   0.5, 0,
												200, 200, 1);
	perspective_transformer<cv::Vec3b> pt(matrix);
	pt.transform(img, new_img);
	cv::imshow("original", img);
	cv::imshow("new", new_img);
	cv::waitKey(0);
	return 0;
}
