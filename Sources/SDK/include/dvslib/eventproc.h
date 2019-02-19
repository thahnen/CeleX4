/* 
* Copyright (c) 2017-2018  CelePixel Technology Co. Ltd.  All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef CELEX_IMGPROC_H
#define CELEX_IMGPROC_H

#ifdef _WIN32
	#ifdef CELEX_API_EXPORTS
		#define CELEX_EXPORTS __declspec(dllexport)
	#else
		#define CELEX_EXPORTS __declspec(dllimport)
	#endif
#else
	#ifdef CELEX_LIBRARY
		#define CELEX_EXPORTS
	#else
		#define CELEX_EXPORTS
	#endif
#endif


#ifndef COMPATIBILITY_VERSION
	#ifdef __has_include
		#if __has_include(<opencv/opencv2/opencv.hpp>)
			#include <opencv/opencv2/opencv.hpp>
			#define COMPATIBILITY_VERSION 4
		#elif __has_include(<opencv2/opencv.hpp>)
			#include <opencv2/opencv.hpp>
			#define COMPATIBILITY_VERSION 2
		#else
			#error "Keine installierte OpenCV-Version gefunden!"
		#endif
	#else
		// Not compiled with C++17 so just use the new one!
		#include <opencv/opencv2/opencv.hpp>
		#define COMPATIBILITY_VERSION 4
	#endif
#else
	#if COMPATIBILITY_VERSION == 4
		#include <opencv/opencv2/opencv.hpp>
	#else
		#include <opencv2/opencv.hpp>
	#endif
#endif


namespace dvs {
	CELEX_EXPORTS int segmentationByMultislice(const cv::Mat& multislicebyte, double ratio, cv::Mat& segimage);
	CELEX_EXPORTS int denoisingMaskByEventTime(const cv::Mat& countEventImg, double timelength, cv::Mat& denoiseMaskImg);
	CELEX_EXPORTS void denoisingByNeighborhood(const cv::Mat& countEventImg, cv::Mat& denoisedImg);
}

#endif // CELEX_IMGPROC_H