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


constexpr auto SLIDER_DELAY				= 100;
constexpr auto MAX_LOG_LINES			= 100;

constexpr auto MAX_PAGE_COUNT			= 100000;

constexpr auto PIPEOUT_TIMER			= 10;
constexpr auto EVENT_SIZE				= 4;
constexpr auto PIXELS_PER_COL			= 768;
constexpr auto PIXELS_PER_ROW			= 640;
constexpr auto PIXELS_NUMBER			= PIXELS_PER_COL*PIXELS_PER_ROW;

constexpr auto CELEX5_COL				= 1280;
constexpr auto CELEX5_ROW				= 800;
constexpr auto CELEX5_PIXELS_NUMBER		= 1024000;

constexpr auto MIRROR_VERTICAL			= 1;
constexpr auto MIRROR_HORIZONTAL		= 1;

constexpr auto FILE_COMMANDS			= "commands.xml";
constexpr auto FILE_SEQUENCES			= "sequences.xml";
constexpr auto FILE_SLIDERS				= "sliders.xml";
constexpr auto FILE_CELEX5_CFG			= "CeleX5_Commands.xml";
constexpr auto FILE_CELEX5_CFG_NEW		= "CeleX5_Commands_New.xml";

constexpr auto SEQUENCE_LAYOUT_WIDTH	= 3;			// 7
constexpr auto SLIDER_LAYOUT_WIDTH		= 1;			// 4
constexpr auto DIALOG_LAYOUT_WIDTH		= 2;

constexpr auto FPN_CALCULATION_TIMES	= 5;

constexpr auto TIMER_CYCLE				= 25000000;		// 1s
constexpr auto HARD_TIMER_CYCLE			= 262144;		// 2^17=131072; 2^18=262144


/*
	*) Why use these Datatypes?
	- The CeleX4-Camera is 768x640 px, so:
		=> 768/640: 2^10, so why not int with 10 Bitfields each
	- Brightness is Grayscale 0-255, so:
		=> 0-255: 2^8, so why not short int with 8 Bitfields PLUS
	- Polarity is as i suppose only -1/1/0,
		=> 3 Values: 2^4, so why not short int with 2 Bitfields!
	- Timestamp t is ok in my eyes!

	*) Using this:

		typedef struct EventData {
			int col : 10, row : 10;
			short int brightness : 8, polarity : 2;
			uint32_t t;
		} EventData;

 		=> the Struct-Size will decrease from 12 to 8 Bytes (needs 1/3 LESS Memory!)
*/
typedef struct EventData {
	uint16_t col;
	uint16_t row;
	uint16_t brightness;
	uint16_t polarity;						// -1: intensity weakened; 1: intensity increased; 0 intensity unchanged
	uint32_t t;
} EventData;

#endif // CELEXTYPES_H
