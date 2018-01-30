#include <opencv2/opencv.hpp>
#include <cmath>

template <typename T>
class blur {
	public:
		blur(cv::Point point = cv::Point(-1, -1), cv::Mat kernel = cv::Mat(cv::Size(3, 3), CV_8UC1, cv::Scalar::all(1)), bool normalize = true): anchor_point(point), kernel_matrix(kernel), is_normalized(normalize)
		{
			kernel_size = kernel_matrix.size();
			if (anchor_point == cv::Point(-1, -1))
				anchor_point = cv::Point(kernel_size.width / 2, kernel_size.height / 2);
		}
		virtual void do_blur(const cv::Mat& original_image, cv::Mat& blur_image)
		{
			if (blur_image.empty())
				blur_image.create(original_image.size(), original_image.type());
			for (int i = 0; i < original_image.rows; i++)
				for (int j = 0; j < int(original_image.cols * sizeof(T)); j++) {
					int temp = 0, count = 0;
					for (int l = i - anchor_point.y; l <= i + kernel_size.height - anchor_point.y - 1; l++) {
						for (int m = j - int(sizeof(T) * anchor_point.x); m <= j + int(sizeof(T) * (kernel_size.width - anchor_point.x - 1)); m += int(sizeof(T)))
							if (l >= 0 && l <= original_image.rows && m >= 0 && m <= int(sizeof(T) * original_image.cols)) {
								temp += original_image.at<uchar>(l, m) * kernel_matrix.at<uchar>(l - (i - anchor_point.y), (m - (j - int(sizeof(T) * anchor_point.x))) / int(sizeof(T)));
								count += kernel_matrix.at<uchar>(l - (i - anchor_point.y), (m - (j - int(sizeof(T) * anchor_point.x))) / int(sizeof(T)));
							}
					}
					blur_image.at<uchar>(i, j) = (is_normalized ? temp / count : temp > 255 ? 255 : temp);
				}
		}
	protected:
		cv::Size kernel_size;
		cv::Point anchor_point;
		cv::Mat kernel_matrix;
		bool is_normalized;
};

template <typename T>
class box_blur: public blur<T> {
	public:
		box_blur(cv::Size size = cv::Size(3, 3), cv::Point point = cv::Point(-1, -1), bool normalize = true): blur<T>(point, cv::Mat(size, CV_8UC1, cv::Scalar::all(1)), normalize)
		{
			if (blur<T>::anchor_point == cv::Point(-1, -1))
				blur<T>::anchor_point = cv::Point(blur<T>::kernel_size.width / 2, blur<T>::kernel_size.height / 2);
		}
};

template <typename T>
class mean_blur: public box_blur<T> {
	public:
		mean_blur(cv::Size size = cv::Size(3, 3), cv::Point point = cv::Point(-1, -1)): box_blur<T>(size, point, true)
		{ }
};

/*
 * Gaussian Function
 * in tex:
 * $G(x,y)=Ae^{\frac{-x^2}{2\sigma^2_x}+\frac{-y^2}{2\sigma^2_y}}$
 */
template <typename T>
class gaussian_blur: public blur<T> {
	public:
		gaussian_blur(cv::Size size = cv::Size(3, 3), cv::Point point = cv::Point(-1, -1), double sigma_x = 1.5, double sigma_y = 1.5): blur<T>(point, cv::Mat(size, CV_64FC1, cv::Scalar::all(1.5)), true)
		{
			for (int i = -blur<T>::anchor_point.y; i <= blur<T>::kernel_size.height - blur<T>::anchor_point.y - 1; i++) {
				for (int j = -blur<T>::anchor_point.x; j <= blur<T>::kernel_size.width - blur<T>::anchor_point.x - 1; j++) {
					cv::Mat(blur<T>::kernel_matrix).at<double>(i + blur<T>::anchor_point.y, j + blur<T>::anchor_point.x) = exp(-pow(i, 2) / double(2 * pow(sigma_x, 2)) - pow(j, 2) / double(2 * pow(sigma_y, 2)));
				}
			}
		}
		void do_blur(const cv::Mat& original_image, cv::Mat& blur_image)
		{
			if (blur_image.empty())
				blur_image.create(original_image.size(), original_image.type());
			for (int i = 0; i < original_image.rows; i++)
				for (int j = 0; j < int(original_image.cols * sizeof(T)); j++) {
					double temp = 0.0, count = 0.0;
					for (int l = i - blur<T>::anchor_point.y; l <= i + blur<T>::kernel_size.height - blur<T>::anchor_point.y - 1; l++) {
						for (int m = j - int(sizeof(T) * blur<T>::anchor_point.x); m <= j + int(sizeof(T) * (blur<T>::kernel_size.width - blur<T>::anchor_point.x - 1)); m += int(sizeof(T)))
							if (l >= 0 && l <= original_image.rows && m >= 0 && m <= int(sizeof(T) * original_image.cols)) {
								temp += original_image.at<uchar>(l, m) * cv::Mat(blur<T>::kernel_matrix).at<double>(l - (i - blur<T>::anchor_point.y), (m - (j - int(sizeof(T) * blur<T>::anchor_point.x))) / int(sizeof(T)));
								count += cv::Mat(blur<T>::kernel_matrix).at<double>(l - (i - blur<T>::anchor_point.y), (m - (j - int(sizeof(T) * blur<T>::anchor_point.x))) / int(sizeof(T)));
							}
					}
					blur_image.at<uchar>(i, j) = temp / count;
				}
		}
};

int main()
{
	cv::Mat img = cv::imread("../../img/lenna.jpg"), output1, output2;
	if (img.empty())
		return -1;
	mean_blur<cv::Vec3b> mb(cv::Size(11, 11), cv::Point(-1, -1));
	mb.do_blur(img, output1);
	gaussian_blur<cv::Vec3b> gb(cv::Size(11, 11), cv::Point(-1, -1), 3.0, 3.0);
	gb.do_blur(img, output2);
	imshow("original", img);
	imshow("mean_blur", output1);
	imshow("gaussian_blur", output2);
	cv::waitKey(0);
	return 0;
}
