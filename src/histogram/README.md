# image-processing-algorithum
OpenCV based Image Processing Algorithm.

# *histogram*

## class

```c++
template <typename T>
class histogram {
	public:
		histogram(const cv::Mat& image);
		~histogram();
		void equalization(cv::Mat& equalize_image);
		/*
		uchar idx:
			- 0: draw the histogram of original image.
			- 1: draw the histogram of the equalized image.
		 */
		void draw(uchar idx, std::vector<cv::Mat>& draw_image);
	private:
		cv::Mat original_image;/*a copy of the input image*/
		const int size = 256;/*the size of histogram*/
		std::array<std::array<std::array<int, 256>, sizeof(T)>, 2> n;/*count the number of each pixel value*/
		std::array<std::array<std::array<int, 256>, sizeof(T)>, 2> sumofn;/*to sum up 'n'*/
		std::array<std::array<int, sizeof(T)>, 2> max_val;/*store the maximum number of pixels*/
};
```

## example

+ Process a grayscale image.

```c++
int main()
{
	cv::Mat img = cv::imread("../../img/lenna.jpg", 0);
	if (img.empty())
		return -1;
	histogram<uchar> h(img);
	cv::Mat equal;
	h.equalization(equal);
	std::vector<cv::Mat> histogram;
	h.draw(0, histogram);//draw the histogram of original image.
	cv::imshow("original_image", img);
	cv::imshow("equalized_image", equal);
	cv::imshow("histogram_channel1", histogram.at(0));
	cv::waitKey(0);
	return 0;
}
```

+ Convert a RGB image.

```c++
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
```
