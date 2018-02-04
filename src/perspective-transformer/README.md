# image-processing-algorithum
OpenCV based Image Processing Algorithm.

# *perspective-transformer*

## class

```c++
template <typename T>
class perspective_transformer {
	public:
		perspective_transformer(cv::Mat& matrix);
		void transform(const cv::Mat& original_image, cv::Mat& output_image);
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
```

## example

+ Affine Transformation

If ```t13 = 0; t23 = 0; t33 = 1```, it's affine transformation.

```c++
/* example 1: identical transformation
 * |  1   0   0 |
 * |  0   1   0 |
 * |  0   0   1 |
 * cv::Mat matrix = (cv::Mat_<double>(3, 3) << 1, 0, 0,
											   0, 1, 0,
											   0, 0, 1);
											   
int main()
{
	cv::Mat img = cv::imread("../../img/lenna.jpg"), new_img;
	if (img.empty())
		return -1;
	cv::Mat matrix = (cv::Mat_<double>(3, 3) << 1, 0, 0,
											    0, 1, 0,
											    0, 0, 1);
	perspective_transformer<cv::Vec3b> pt(matrix);
	pt.transform(img, new_img);
	cv::imshow("original", img);
	cv::imshow("new", new_img);
	cv::waitKey(0);
	return 0;
}

/*
 * example 2: shrink and shift
 * |  0.5   0     0 |
 * |  0     0.5   0 |
 * |  100   100   1 |
 * cv::Mat matrix = (cv::Mat_<double>(3, 3) << 0.5,   0, 0, 
											   0,   0.5, 0,
											   100, 100, 1);
 */
	 
 int main()
{
	cv::Mat img = cv::imread("../../img/lenna.jpg"), new_img;
	if (img.empty())
		return -1;
	cv::Mat matrix = (cv::Mat_<double>(3, 3) << 0.5,   0, 0, 
											    0,   0.5, 0,
											    100, 100, 1);
	perspective_transformer<cv::Vec3b> pt(matrix);
	pt.transform(img, new_img);
	cv::imshow("original", img);
	cv::imshow("new", new_img);
	cv::waitKey(0);
	return 0;
}
```

```c++
/*
 * example 3: shrink and shift
 * |  cos(PI / 4)   sin(PI / 4)     0 |
 * |  -sin(PI / 4)  cos(PI / 4)     0 |
 * |  0             0               1 |
 * cv::Mat matrix = (cv::Mat_<double>(3, 3) << cos(PI / 4),  sin(PI / 4), 0, 
											   -sin(PI / 4), cos(PI / 4), 0, 
											   0,            0,           1);
 */
int main()
{
	cv::Mat img = cv::imread("../../img/lenna.jpg"), new_img;
	if (img.empty())
		return -1;
	cv::Mat matrix = (cv::Mat_<double>(3, 3) << cos(PI / 4),  sin(PI / 4), 0, 
												-sin(PI / 4), cos(PI / 4), 0, 
												0,            0,           1);
	perspective_transformer<cv::Vec3b> pt(matrix);
	pt.transform(img, new_img);
	cv::imshow("original", img);
	cv::imshow("new", new_img);
	cv::waitKey(0);
	return 0;
}
```

+ perspective-transformation