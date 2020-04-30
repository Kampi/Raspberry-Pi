#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv)
{
	Mat Frame;
	VideoCapture Camera(0);
	
	if (!Camera.isOpened())
	{
		return -1;
	}
	
	while (1)
	{
		Camera >> Frame;

		namedWindow("Display Image", WINDOW_AUTOSIZE);
		imshow("Display Image", Frame);

		waitKey(1);
	}

	return 0;
}