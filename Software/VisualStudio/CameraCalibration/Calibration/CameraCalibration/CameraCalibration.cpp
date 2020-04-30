/* Run with 
 *	CameraCalibration -calib 8 6 <OutputPath>
 *		8 - Horizontal Pattern
 *		6 - Vertical Pattern
 *	CameraCalibration -load <InputPath>	
 */

#include <thread>
#include "Calibration.h"

using namespace std;
using namespace cv;

VideoCapture Camera;
CameraCalibration Calibration;

Mat UndistortedImage;
Mat ResizedImage;

/*
 * Thread for reading new images
 */
void CaptureImage(void)
{	
	Mat Image;
	
	if (Camera.isOpened())
	{
		Camera >> Image;
		Calibration.UndistortImage(&Image, &UndistortedImage);
	}
	else
	{
		cout << "[ERROR] Camera not open!" << endl;
		exit(-1);
	}
}

int main(int argc, char* argv[])
{
	int Status;
	thread CameraThread;
	
	// Command line parsing
	if (argc == 3)
	{
		string argv1 = argv[1];
		string argv2 = argv[2];
		
		if (argv1 == "-load")
		{
			Calibration.Load(argv2);
		}
		else
		{
			cout << "[ERROR] Wrong parameter " << argv1 << endl;
			return -1;
		}
	}	
	else if (argc == 6)
	{
		string argv1 = argv[1];
		string argv2 = argv[2];
		string argv5 = argv[5];
		
		if (argv1 == "-calib")
		{
			Calibration.InitCalibration(atoi(argv[3]), atoi(argv[4]));
			Status = Calibration.LoadImages(argv5 + "//Calibration");
	
			if (Status == 0)
			{
				Calibration.PerformCalibration();
				Calibration.Save(argv5 + "//CalibrationData.xml");
			}
		}
		else
		{
			cout << "[ERROR] Wrong parameter " << argv1 << endl;
			return -1;
		}
	}
	else
	{
		cout << "[ERROR] Wrong parameter!" << endl;
		return -1;
	}
	
	cout << "[INFO] Focal length: " << Calibration.GetFocalLength() << endl;
	cout << "[INFO] Optical center: " << Calibration.GetOpticalCenter() << endl;
	
	Camera = VideoCapture(0);
	
	if (!Camera.isOpened())
	{
		return -1;
	}

	while (1)
	{	
		CameraThread = std::thread(CaptureImage);
		CameraThread.join();
		
		namedWindow("Display Image");
		imshow("Display Image", UndistortedImage);
		
		waitKey(20);
	}

	return 0;
}
