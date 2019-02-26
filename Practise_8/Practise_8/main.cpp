#include <opencv2/opencv.hpp>
#include <iostream> // standard C++ I/O
using namespace cv; // OpenCV API is in the C++ "cv" namespace
using namespace std;
int main(int argc, char** argv)
{
	Mat img; // image object
	Mat img_f; // next image flow
	Mat imgInvert;       // grayscale of input image
	Mat imgCanny;         // Cannyimage of input image
	Mat imgBlur;
	Mat imgdiff;
	clock_t t;
	time_t start = time(0);
	bool saving = false; //check if the image has been saved

	std::vector<int> params; // file saving compression parameters

	int key;
	bool keepProcessing = true;

	// -- Question 1 and 2, capture image and video from camera --

	// grab an image from camera (here assume only 1 camera, device 0)
	VideoCapture cap(0); // video capture object
	if (!cap.isOpened()) {
		std::cout << "error: could not grab a frame" << std::endl;
		exit(0);
	}
	// loop and display up to N frames
	int nFrames = 50;

	cap >> img;
	VideoWriter videoOutput("video_saved.avi", CV_FOURCC('D', 'I', 'V', 'X'), 25, img.size(), true);


	while (keepProcessing) {
		for (int i = 0; i < nFrames; i++) {
			cap >> img; // retrieve the captured frame as an image
						// display image in window

						//store the video
			videoOutput << img; // send to video writer object 
			cap >> img_f;


			// -- Question 3 and 4, add image processing functions to the program
			GaussianBlur(img, imgBlur, cv::Size(5, 5), 3, 3);
			bitwise_not(img, imgInvert);
			cv::Canny(img, imgCanny, 50, 50, 3, true);
			absdiff(img_f, img, imgdiff);

			// declare windows
			cv::namedWindow("imgOriginal", CV_WINDOW_NORMAL);       // note: you can use CV_WINDOW_NORMAL which allows resizing the window
			cv::namedWindow("imgCanny", CV_WINDOW_NORMAL); // or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
														   // CV_WINDOW_AUTOSIZE is the default
			cv::namedWindow("imgInvert", CV_WINDOW_NORMAL);
			cv::namedWindow("imgBlur", CV_WINDOW_NORMAL);
			cv::namedWindow("imgdiff", CV_WINDOW_NORMAL);

			cv::imshow("imgOriginal", img);                 // show windows
			cv::imshow("imgCanny", imgCanny);                       //
			cv::imshow("imgInvert", imgInvert);
			cv::imshow("imgBlur", imgBlur);
			cv::imshow("imgdiff", imgdiff);

			key = waitKey(40);

			// -- Question 5, press the button "c" for saving the image.
			if (key == 'c') {
				// if user presses "c" then exit
				std::cout << "Save images!" << std::endl;
				params.push_back(CV_IMWRITE_JPEG_QUALITY);
				params.push_back(95);
				imwrite("save_image.jpg", img, params);
			}


			int seconds_since_start = difftime(time(0), start); // used for convert the time to int type
			if (((seconds_since_start % 3) == 0) && saving == false) {
				params.push_back(CV_IMWRITE_JPEG_QUALITY);
				params.push_back(100);
				imwrite("save_image_3s.jpg", img, params);
				saving = true; // image has been saved, and will be only saved once
			}


		}

	}
	return 0;
}