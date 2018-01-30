# image-processing-algorithum
OpenCV based Image Processing Algorithm.

# *spatial-filter*

## class

```c++
template <typename T>
class blur {
	public:
		blur(cv::Point point, cv::Mat kernel, bool normalize);
		virtual void do_blur(const cv::Mat& original_image, cv::Mat& blur_image);
	protected:
		cv::Size kernel_size;
		cv::Point anchor_point;
		cv::Mat kernel_matrix;
		bool is_normalized;
};

template <typename T>
class box_blur: public blur<T> {
	public:
		box_blur(cv::Size size, cv::Point point, bool normalize);
};

template <typename T>
class mean_blur: public box_blur<T> {
	public:
		mean_blur(cv::Size size, cv::Point);
};

/*
 * Gaussian Function
 * in tex:
 * $G(x,y)=Ae^{\frac{-x^2}{2\sigma^2_x}+\frac{-y^2}{2\sigma^2_y}}$
 */
template <typename T>
class gaussian_blur: public blur<T> {
	public:
		gaussian_blur(cv::Size size, cv::Point point, double sigma_x, double sigma_y);
		void do_blur(const cv::Mat& original_image, cv::Mat& blur_image);
};
```

## example

+ Mean Blur & Gaussian Blur.

```c++
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
```
