/*
* Copyright (c) 2017-2018 CelePixel Technology Co. Ltd. All Rights Reserved
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

#ifndef CELEXTYPES_H
#define CELEXTYPES_H

#include <stdint.h>


#ifndef UNUSED
	#ifdef _GNUC_
		#define UNUSED __attribute__(unused)
	#elif _MSC_VER
		#define UNUSED __declspec(deprecated("Variable unused!"))
	#else
		#define UNUSED
	#endif
#endif


UNUSED constexpr auto SLIDER_DELAY			= 100;
UNUSED constexpr auto MAX_LOG_LINES			= 100;

UNUSED constexpr auto MAX_PAGE_COUNT		= 100000;

UNUSED constexpr auto PIPEOUT_TIMER			= 10;
constexpr auto EVENT_SIZE					= 4;
constexpr auto PIXELS_PER_COL				= 768;
constexpr auto PIXELS_PER_ROW				= 640;
constexpr auto PIXELS_NUMBER				= PIXELS_PER_COL*PIXELS_PER_ROW;

UNUSED constexpr auto CELEX5_COL			= 1280;
UNUSED constexpr auto CELEX5_ROW			= 800;
UNUSED constexpr auto CELEX5_PIXELS_NUMBER	= 1024000;

UNUSED constexpr auto MIRROR_VERTICAL		= 1;
UNUSED constexpr auto MIRROR_HORIZONTAL		= 1;

UNUSED constexpr auto FILE_COMMANDS			= "commands.xml";
UNUSED constexpr auto FILE_SEQUENCES		= "sequences.xml";
UNUSED constexpr auto FILE_SLIDERS			= "sliders.xml";
UNUSED constexpr auto FILE_CELEX5_CFG		= "CeleX5_Commands.xml";
UNUSED constexpr auto FILE_CELEX5_CFG_NEW	= "CeleX5_Commands_New.xml";

UNUSED constexpr auto SEQUENCE_LAYOUT_WIDTH	= 3;			// 7
UNUSED constexpr auto SLIDER_LAYOUT_WIDTH	= 1;			// 4
UNUSED constexpr auto DIALOG_LAYOUT_WIDTH	= 2;

constexpr auto FPN_CALCULATION_TIMES		= 5;

UNUSED constexpr auto TIMER_CYCLE			= 25000000;		// 1s
UNUSED constexpr auto HARD_TIMER_CYCLE		= 262144;		// 2^17=131072; 2^18=262144


/*
	*) Why use these Datatypes?
	- The CeleX4-Camera is 768x640 px, so:
		=> 768/640: 2^10, so why not std::bitset<10> (like "typedef std::bitset<10> bit10") or BitField uint16:t x : 10 = 768 ?
	- Brightness is Grayscale 0-255, so:
		=> 0-255: 2^8, so why not unint8_t (or to be conform std::bitset<8>) ?
	- Polarity is as i suppose only -1/1/0,
		=> 3 Values: 2^4, so why not std::bitset<4> and map the possible Values?
	- Timestamp t, in Frames, so:
		=> Max Time is: 2^32 Frames (or ~4 1/2 Years nonstop at 30 FPS)
		=> 2^16 would be not enough (~32 min at 30 FPS)

	*) Why is this even in here? its redefined in celex4.h with "more" specific data types
*/
typedef struct EventData {
	uint16_t col;
	uint16_t row;
	uint16_t brightness;
	uint16_t polarity;						// -1: intensity weakened; 1: intensity increased; 0 intensity unchanged
	uint32_t t;
} EventData;

#endif // CELEXTYPES_H
