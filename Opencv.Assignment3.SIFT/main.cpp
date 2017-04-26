#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include "pch.h"
#include <conio.h>
#include "UnpackedSIFT.h"
#include "UnpackedSIFT.cpp"



using namespace cv;
using namespace std;
using namespace MySift;
using xfeatures2d::SIFT;
using xfeatures2d::SiftDescriptorExtractor;
using xfeatures2d::SiftFeatureDetector;

//Declarations
void RunPrebuiltSiftDetector(const Mat input, vector<KeyPoint>& keypoints, Mat& descriptors);
void RunMySiftDetector(const Mat input, vector<KeyPoint>& keypoints, Mat& descriptors);
Mat rotateImage(const Mat src, float angle);
string filePrompt();

int main(int argc, const char* argv[])
{
	string filename = filePrompt();

	//string filename;
	//if (argc < 2) {
	//	filename = string("Input_Data\\blocks_L-150x150.png");
	//} else {
	//	filename = argv[1];
	//}
	//assert(!filename.empty());

	printf("Reading file from %s\n", filename.c_str());
	cv::Mat input= cv::imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
	
	if (!input.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		_getch();
		return -1;
	}

	const Mat warpedInput = rotateImage(input, 45);

	cv::Mat des1, des2; //descriptors
	cv::Mat wdes1, wdes2; //descriptors

	vector<KeyPoint> kp1, kp2; //keypoints
	vector<KeyPoint> wkp1, wkp2; //keypoints warped


	Mat outKeyPoints1, outKeyPoints2, outMatch1, outMatch2; //image outputs
	
	vector<DMatch> matches1, matches2;

	BFMatcher matcher; //matcher
	try {
		/*Running Premade*/
		RunPrebuiltSiftDetector(input, kp1, des1);//premade for QoS
		RunPrebuiltSiftDetector(warpedInput, wkp1, wdes1);//premade for QoS

		cv::drawKeypoints(input, kp1, outKeyPoints1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		cv::imshow("SIFT_premade keypoints", outKeyPoints1);

		matcher.match(des1, wdes1, matches1, noArray());

		drawMatches(input, kp1, warpedInput, wkp1, matches1, outMatch1);
		imshow("SIFT_premade descriptor matches", outMatch1);


		/*Running mine*/
		RunMySiftDetector(input, kp2, des2);
		RunMySiftDetector(warpedInput, wkp2, wdes2);

		cv::drawKeypoints(input, kp2, outKeyPoints2,Scalar::all(-1),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
		cv::imshow("My SIFT keypoints", outKeyPoints2);

		matcher.match(des2, wdes2, matches2, noArray());

		drawMatches(input, kp2, warpedInput, wkp2, matches2, outMatch2);
		imshow("My SIFT descriptor matches", outMatch2);

		waitKey(0);
		_getch();
	}
	catch (Exception e) {
		cout << "Error: " << e.what() << endl;
		_getch();
		return -1;
	}

	return 0;
}


/// <summary>
/// Runs the prebuilt sift detector.
/// </summary>
/// <param name="input">The input image.</param>
/// <param name="output">The output image.</param>
void RunPrebuiltSiftDetector(const Mat input, vector<KeyPoint>& keypoints, Mat& descriptors)
{
	Ptr<SIFT> detector = SIFT::create();
	detector->detect(input, keypoints);

	// Add results to image and show
	cv::Ptr<SiftDescriptorExtractor> SDE = SiftFeatureDetector::create();
	SDE->compute(input, keypoints, descriptors);

}

/// <summary>
/// Runs my sift detector.
/// </summary>
/// <param name="input">The input image.</param>
/// <param name="keypoints">The keypoints.</param>
/// <param name="descriptors">The descriptors.</param>
void RunMySiftDetector(const Mat input, vector<KeyPoint>& keypoints, Mat& descriptors)
{
	Ptr<UnpackedSIFT> siftcpu = makePtr<UnpackedSIFT>();

	siftcpu->detectImpl(input, keypoints,Mat());
	siftcpu->computeImpl(input, keypoints, descriptors);
	//siftcpu->detect(input, keypoints);
	//siftcpu->compute(input, keypoints, descriptors);
}

Mat rotateImage(const Mat src, float angle) 
{
	Mat dest;
	cv::Point2f center(src.cols / 2.0f, src.rows / 2.0f);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	cv::Rect bbox = cv::RotatedRect(center, src.size(), angle).boundingRect();
	rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;


	cv::warpAffine(src, dest, rot, bbox.size());
	return dest;
}

string filePrompt() {
	char filename[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "Image Files (*.jpeg, *.png, *.jpg) | *.jpeg; *.png; *.jpg; ";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a File";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		std::cout << "You chose the file \"" << filename << "\"\n";
	}
	else
	{
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: std::cout << "You cancelled.\n";
		}
	}
	return filename;
}