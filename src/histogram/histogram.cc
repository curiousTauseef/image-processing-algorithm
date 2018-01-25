#include <opencv2/opencv.hpp>
#include <vector>

template <typename T>
class histogram {
	public:
		histogram(const cv::Mat& image)
		{
			if (image.empty())
				return;
			original_image = image.clone();
			for (int i = 0; i < original_image.rows; i++)
				for (int j = 0; j < original_image.cols * sizeof(T); j++)
					for (int k = 0; k < sizeof(T); k++) {
						if (k > 0)
							j++;
						n[0][k][original_image.at<uchar>(i, j)]++;
					}
			for (int i = 0; i < sizeof(T); i++) {
				for (int j = 0; j < 256; j++) {
					if (j == 0)
						sumofn[0][i][j] = n[0][i][j];
					else
						sumofn[0][i][j] = n[0][i][j] + sumofn[0][i][j - 1];
					if (n[0][i][j] > max_val[0][i])
						max_val[0][i] = n[0][i][j];
				}
			}
		}
		~histogram()
		{
			original_image.release();
		}
		void equalization(cv::Mat& equalize_image)
		{
			if (equalize_image.empty())
				equalize_image.create(original_image.size(), original_image.type());
			for (int i = 0; i < original_image.rows; i++)
				for (int j = 0; j < original_image.cols * sizeof(T); j++)
					for (int k = 0; k < sizeof(T); k++) {
						if (k > 0)
							j++;
						equalize_image.at<uchar>(i, j) = 255 * sumofn[0][k][original_image.at<uchar>(i, j)] / (float)(original_image.rows * original_image.cols);
						n[1][k][equalize_image.at<uchar>(i, j)]++;
					}
			for (int i = 0; i < sizeof(T); i++) {
				for (int j = 0; j < 256; j++) {
					if (j == 0)
						sumofn[1][i][j] = n[1][i][j];
					else
						sumofn[1][i][j] = n[1][i][j] + sumofn[1][i][j - 1];
					if (n[1][i][j] > max_val[1][i])
						max_val[1][i] = n[1][i][j];
				}
			}
		}
		/*
		uchar idx:
			- 0: draw the histogram of original image.
			- 1: draw the histogram of the equalized image.
		 */
		void draw(uchar idx, std::vector<cv::Mat>& draw_image)
		{
			if (idx > 1)
				return;
			for (int i = 0; i < sizeof(T); i++) {
				cv::Mat new_image(size, size, CV_8U, cv::Scalar(0));
				for (int j = 0; j < size; j++)
					rectangle(new_image, cv::Point(j, size), cv::Point((j + 1), size - (n[idx][i][j] / (float)max_val[idx][i] * size * 0.9)), cv::Scalar(255));
				draw_image.push_back(new_image);
			}
		}
	private:
		cv::Mat original_image;/*a copy of the input image*/
		const int size = 256;/*the size of histogram*/
		std::array<std::array<std::array<int, 256>, sizeof(T)>, 2> n{};/*count the number of each pixel value*/
		std::array<std::array<std::array<int, 256>, sizeof(T)>, 2> sumofn{};/*to sum up 'n'*/
		std::array<std::array<int, sizeof(T)>, 2> max_val{};/*store the maximum number of pixels*/
};

int main()
{
	cv::Mat img = cv::imread("../../img/lenna.jpg");
	if (img.empty())
		return -1;
	histogram<cv::Vec3b> h(img);
	cv::Mat equal;
	h.equalization(equal);
	std::vector<cv::Mat> histogram;
	h.draw(0, histogram);//draw the histogram of original image.
	cv::imshow("original_image", img);
	cv::imshow("equalized_image", equal);
	cv::imshow("histogram_channel1", histogram.at(0));
	cv::imshow("histogram_channel2", histogram.at(1));
	cv::imshow("histogram_channel3", histogram.at(2));
	cv::waitKey(0);
	return 0;
}
