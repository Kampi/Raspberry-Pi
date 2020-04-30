#pragma once

#include <stdio.h>
#include <opencv2/opencv.hpp>

#ifdef WINDOWS
	#include <windows.h> 
#else
	#include <dirent.h>
	#include <sys/types.h>
	#include <sys/stat.h>
#endif

class CameraCalibration
{
	public:
		void InitCalibration(int CornersHor, int CornersVer);
		int LoadImages(std::string Path);
		void PerformCalibration(void);
		int Save(std::string Path);
		void Load(std::string Path);
		void UndistortImage(cv::Mat* Input, cv::Mat* Output);
	
		cv::Point GetOpticalCenter(void);
		cv::Point GetFocalLength(void);
		cv::Point GetSkew(void);
	
		std::vector<cv::Mat> GetRotationVectors(void);
		std::vector<cv::Mat> GetTranslationVectors(void);
	
	private:
		bool _Found = false;
		bool _CalibrationDone = false;
	
		int _Squares = 0;
		int _CornersVer = 0;
		int _CornersHor = 0;
	
		std::vector<cv::Point3f> _CheckerCorner;
		std::vector<cv::Point2f> _Corners;
	
		std::vector<std::vector<cv::Point3f>> _ObjectPoints;
		std::vector<std::vector<cv::Point2f>> _ImagePoints;

		cv::Size _ImageSize;
		cv::Size _BoardSize; 
	
		cv::Mat _IntrinsicParameters = cv::Mat(3, 3, CV_32FC1);
		cv::Mat _DistortionCoeff;
		std::vector<cv::Mat> _RotationVectors;
		std::vector<cv::Mat> _TranslationVectors;
};