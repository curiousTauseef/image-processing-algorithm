/*
# OpenCV based Image Processing Algorithm.
# GitHub: techping
# File: perspective-transformer.cc
 */
#include <opencv2/opencv.hpp>
#include <cmath>
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>

template <typename T>
class perspective_transformer {
	public:
		perspective_transformer(void)
		{ }
		perspective_transformer(cv::Mat& matrix): perspective_matrix(matrix)
		{ }
		void calc_matrix(cv::Point2d point1[4], cv::Point2d point2[4])
		{
			if (perspective_matrix.empty())
				perspective_matrix.create(cv::Size(3, 3), CV_64FC1);
			Eigen::Matrix3d mat1 = quadrilateral_to_square(point1);
			Eigen::Matrix3d mat2 = square_to_quadrilateral(point2);
			Eigen::Matrix3d mat = mat1 * mat2;
			cv::eigen2cv(mat, perspective_matrix);
		}
		void transform(const cv::Mat& original_image, cv::Mat& output_image)
		{
			if (perspective_matrix.empty())
				return;
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
		Eigen::Matrix3d square_to_quadrilateral(cv::Point2d point[4])
		{
			Eigen::Matrix3d mat3d;
			double delta_x3 = point[0].x - point[1].x + point[2].x - point[3].x;
			double delta_y3 = point[0].y - point[1].y + point[2].y - point[3].y;
			if (delta_x3 == 0.0 && delta_y3 == 0.0) {
				mat3d << point[1].x - point[0].x,  point[1].y - point[0].y, 0.0, 
								  point[2].x - point[1].x,  point[2].y - point[1].y, 0.0,
								  point[0].x,				  point[0].y,			   1.0;
				return mat3d;
			}
			double delta_x1 = point[1].x - point[2].x;
			double delta_x2 = point[3].x - point[2].x;
			double delta_y1 = point[1].y - point[2].y;
			double delta_y2 = point[3].y - point[2].y;
			double denominator = delta_x1 * delta_y2 - delta_x2 * delta_y1;
			double a13 = (delta_x3 * delta_y2 - delta_x2 * delta_y3) / denominator;
			double a23 = (delta_x1 * delta_y3 - delta_x3 * delta_y1) / denominator;
			mat3d <<  point[1].x - point[0].x + a13 * point[1].x, point[1].y - point[0].y + a13 * point[1].y, a13, 
							  point[3].x - point[0].x + a23 * point[3].x, point[3].y - point[0].y + a23 * point[3].y, a23,
							  point[0].x,									point[0].y,									1.0;
			return mat3d;
		}
		Eigen::Matrix3d quadrilateral_to_square(cv::Point2d point[4])
		{
			return square_to_quadrilateral(point).inverse();
		}
};

#define PI 3.14159265

int main()
{
	cv::Mat img = cv::imread("../../img/lenna.jpg"), new_img;
	if (img.empty())
		return -1;
	/*cv::Mat matrix = (cv::Mat_<double>(3, 3) << 0.5,   0, 0, 
												0,   0.5, 0,
												200, 200, 1);
	perspective_transformer<cv::Vec3b> pt(matrix);*/
	perspective_transformer<cv::Vec3b> pt;
	cv::Point2d point[2][4];
	point[0][0] = cv::Point2d(0.0, 0.0);
	point[0][1] = cv::Point2d(img.cols - 1, 0.0);
	point[0][2] = cv::Point2d(0.0, img.rows - 1);
	point[0][3] = cv::Point2d(img.cols - 1, img.rows - 1);
	point[1][0] = cv::Point2d(img.cols / 3, 0);
	point[1][1] = cv::Point2d(img.cols / 3 * 2, 0);
	point[1][2] = cv::Point2d(0.0, img.rows - 1);
	point[1][3] = cv::Point2d(img.cols - 1, img.rows - 1);
	pt.calc_matrix(point[0], point[1]);
	pt.transform(img, new_img);
	cv::imshow("original", img);
	cv::imshow("new", new_img);
	cv::waitKey(0);
	return 0;
}
