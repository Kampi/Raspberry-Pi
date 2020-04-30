#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		std::cout << "Wrong parameter count! Expect 5!" << std::endl;
		return -1;
	}

	// Black fields
	int NumY = std::atoi(argv[1]);
	int NumX = std::atoi(argv[2]);

	// Square size of each field
	int Size = std::atoi(argv[3]);

	int imageWidth = Size * NumX;
	int imageHeight = Size * NumY;

	// Create a new white output image
	Mat OutputImage(imageHeight, imageWidth, CV_8UC3);
	OutputImage.setTo(Scalar(255, 255, 255));

	for (int y = 0; y < NumY; y++)
	{
		for (int x = 0; x < NumX; x++)
		{
			Vec3b intensity;
			unsigned char ucColor = 0x00;
			int blackWhiteDecision = (x + (y % 2)) % 2;
			if (blackWhiteDecision == 1)
			{
				ucColor = 0xFF;
			}

			intensity[0] = ucColor;
			intensity[1] = ucColor;
			intensity[2] = ucColor;

			for (int xx = 0; xx < Size; xx++)
			{
				for (int yy = 0; yy < Size; yy++)
				{
					OutputImage.at<Vec3b>(y * Size + yy, x * Size + xx) = intensity;
				}
			}
		}
	}

	// Save the output image
	imwrite(argv[4], OutputImage);
}
