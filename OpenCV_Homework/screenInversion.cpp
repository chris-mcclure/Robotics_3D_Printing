/*
Chris McClure
CS480 Robotics & 3D Printing
HW4
11/14/2018

The code was tested with onscreen pixels using a webcam.
The average latencies were
	1.) 3700000 ticks
	2.) 1113 ms
*/

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
//#include<conio.h>           // may have to modify this line if not using Windows

void createImage(int val, cv::Mat &img) {
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			img.at<uchar>(i, j) = val;
		}
	}
}

int main() {
	cv::VideoCapture capWebcam(0);            // declare a VideoCapture object and associate to webcam, 0 => use 1st webcam
	int64 last_colorchange = cv::getTickCount();

	if (!capWebcam.isOpened()) {                                // check if VideoCapture object was associated to webcam successfully
		std::cout << "error: capWebcam not accessed successfully." << std::endl; // if not, print error message to std out
//		_getch();                                                           may have to modify this line if not using Windows
		return(0);                                                          // and exit program
	}
	bool lastframe_black = true;

	cv::Mat imgOriginal;        // input image
	cv::Mat imgInverted;       // grayscale of input image

	char charCheckForEscKey = 0;
	float threshold = 30;
	float brightness = 0;

	while (charCheckForEscKey != 27 && capWebcam.isOpened()) {          
		bool frameRead = capWebcam.read(imgOriginal);
		if (!frameRead || imgOriginal.empty()) {
			std::cout << "frame not read" << std::endl;
			break;
		}
		
		cv::cvtColor(imgOriginal, imgInverted, CV_BGR2GRAY); //converts original image to grayscale
		for (int i = 0; i < imgInverted.rows; i++) {
			for (int j = 0; j < imgInverted.cols; j++) {
				cv::Scalar temp = cv::mean(imgInverted.at<uchar>(i, j)); //collects and stores average
				brightness = temp.val[0]; //sets brightness to value
			}
		}

		bool camera_sees_black = brightness < threshold; 

		if (lastframe_black != camera_sees_black) {
			lastframe_black = camera_sees_black;
			int64 color_change = cv::getTickCount();
			std::cout << "time since last change: " << (color_change - last_colorchange) << " ticks" << std::endl;
			std::cout << "time since last change: " << ((color_change - last_colorchange) / cv::getTickFrequency())*1000 << " ms" << std::endl;

			last_colorchange = cv::getTickCount();
		}

		if (camera_sees_black) {
			createImage(255, imgInverted);
		}
		else createImage(0, imgInverted);

		cv::namedWindow("imgInverted", CV_WINDOW_NORMAL);
		cv::imshow("imgInverted", imgInverted);

		charCheckForEscKey = cv::waitKey(1);
	}   
	return(0);
}


