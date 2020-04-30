#include "Calibration.h"

void CameraCalibration::InitCalibration(int CornersHor, int CornersVer)
{
	_CornersVer = CornersVer;
	_CornersHor = CornersHor;
	
	_Squares = CornersHor * CornersVer;
	
	// Get the checkerboard corner
	for(int j = 0 ; j < _Squares ; j++)
	{
		_CheckerCorner.push_back(cv::Point3f(j / _CornersHor, j % _CornersHor, 0.0f));
	}	
	
	_BoardSize = cv::Size(_CornersHor, _CornersVer);
}

int CameraCalibration::LoadImages(std::string Path)
{
	std::vector<std::string> FileNames;
	cv::Mat Image;
	cv::Mat GrayImage;

	// Read the files from the directory
	#ifdef WINDOWS
		HANDLE Dir;
		WIN32_FIND_DATA FileData;

		if ((Dir = FindFirstFile((Path + "/*").c_str(), &FileData)) == INVALID_HANDLE_VALUE)
		{
			std::cout << "[ERROR] Can not open directory " << Path << std::endl;
			return -1;
		}
				
		do 
		{
			const string FileName = FileData.cFileName;
			const string FullFileName = Path + "/" + FileName;
			const bool IsDirectory = (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

			if (FileName[0] == '.')
			{
				continue;
			}

			if (IsDirectory)
			{
				continue;
			}

			FileNames.push_back(FullFileName);
		} while (FindNextFile(Dir, &FileData));

		FindClose(Dir);
	#else
		DIR* Dir;
		class dirent *ent;
		class stat st;

		Dir = opendir(Path.c_str());
		if(!Dir)
		{
			std::cout << "[ERROR] Can not open directory " << Path << std::endl;
			return -1;
		}	
	
		while ((ent = readdir(Dir)) != NULL) 
		{
			const std::string FileName = ent->d_name;
			const std::string FullFileName = Path + "/" + FileName;

			if (FileName[0] == '.')
			{
				continue;
			}

			if (stat(FullFileName.c_str(), &st) == -1)
			{
				continue;
			}

			const bool IsDirectory = (st.st_mode & S_IFDIR) != 0;

			if (IsDirectory)
			{
				continue;
			}

			FileNames.push_back(FileName);
		}
	
		closedir(Dir);
	#endif
	
	_CalibrationDone = false;
	
	for (int i = 0; i < FileNames.size(); i++)
	{
		Image = cv::imread(Path + "//" + FileNames[i]);	

		if (!Image.data)
		{
			std::cout << "[ERROR] Could not open or find image " << (i + 1) << "!" << std::endl;
			return -1;
		}
		else
		{
			std::cout << "[INFO] Processing image " << (i + 1) << "/" << FileNames.size() << "..." << std::endl;
		}
			
		// Convert image to gray scale
		cv::cvtColor(Image, GrayImage, cv::COLOR_RGB2GRAY);
			
		// Search the chessboard corners
		_Found = cv::findChessboardCorners(GrayImage, _BoardSize, _Corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
		
		// Get the image size
		_ImageSize = Image.size();
		
		if (_Found)
		{
			cv::cornerSubPix(GrayImage, _Corners, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			cv::drawChessboardCorners(Image, _BoardSize, _Corners, _Found);
							
			_ImagePoints.push_back(_Corners);
			_ObjectPoints.push_back(_CheckerCorner);
		}
	}
	
	return 0;
}

void CameraCalibration::PerformCalibration()
{
	_IntrinsicParameters.ptr<float>(0)[0] = 1;
	_IntrinsicParameters.ptr<float>(1)[1] = 1;
			
	cv::calibrateCamera(_ObjectPoints, _ImagePoints, _ImageSize, _IntrinsicParameters, _DistortionCoeff, _RotationVectors, _TranslationVectors);
	
	std::cout << "[INFO] Calibration done!" << std::endl;
	_CalibrationDone = true;
}

int CameraCalibration::Save(std::string Path)
{
	if (_CalibrationDone)
	{
		// Open a new file
		cv::FileStorage fs(Path, cv::FileStorage::WRITE);
	
		// Write the parameter into the file
		fs << "Intrinsic" << _IntrinsicParameters;
		fs << "Distortion" << _DistortionCoeff;
		fs << "BoardWidth" << _CornersHor;
		fs << "BoardHeight" << _CornersVer;
	
		// Close the file
		fs.release();

		return 0;
	}

	return -1;	
}

void CameraCalibration::Load(std::string Path)
{
	// Open the file
	cv::FileStorage fs(Path, cv::FileStorage::READ);

	// Read tje parameter
	fs["Intrinsic"] >> _IntrinsicParameters;
	fs["Distortion"] >> _DistortionCoeff;
	
	// Close the file
	fs.release();
	
	_CalibrationDone = true;
}

void CameraCalibration::UndistortImage(cv::Mat* Input, cv::Mat* Output)
{	
	cv::undistort(*Input, *Output, _IntrinsicParameters, _DistortionCoeff);
}

cv::Point CameraCalibration::GetFocalLength(void)
{
	return cv::Point(_IntrinsicParameters.at<double>(0, 0), _IntrinsicParameters.at<double>(1, 1));
}

cv::Point CameraCalibration::GetOpticalCenter(void)
{
	return cv::Point(_IntrinsicParameters.at<double>(0, 2), _IntrinsicParameters.at<double>(1, 2));
}

cv::Point CameraCalibration::GetSkew(void)
{
	return cv::Point(_IntrinsicParameters.at<double>(0, 1));
}

std::vector<cv::Mat> CameraCalibration::GetRotationVectors(void)
{
	return _RotationVectors;
}

std::vector<cv::Mat> CameraCalibration::GetTranslationVectors(void)
{
	return _TranslationVectors;
}