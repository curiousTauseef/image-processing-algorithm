# image-processing-algorithum
OpenCV based Image Processing Algorithm.

# *grayscale-transformer*

## example

+ Convert a grayscale image.

```c++
Mat img = imread("../../img/lenna.jpg", IMREAD_GRAYSCALE), new_img;
if (img.empty())
	return -1;
grayscale_transformer<uchar> gt(convert);
gt.transform(img, new_img);
```

+ Convert a RGB image.

First, read image using flag `IMREAD_COLOR`.
And then, Change the template value of grayscale_transformer from `uchar` to `Vec3b`.

```c++
Mat img = imread("../../img/lenna.jpg", IMREAD_COLOR), new_img;
if (img.empty())
	return -1;
grayscale_transformer<Vec3b> gt(convert);
gt.transform(img, new_img);
```

+ You can also write your own function/functor/lambda.
(if use lambda, should compile with `--std=c++11`)

```c++
grayscale_transformer<Vec3b> gt([](uchar gray)->uchar{return 255 - gray;});
gt.transform(img, new_img);
```