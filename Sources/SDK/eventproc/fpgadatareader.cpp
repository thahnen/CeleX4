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

#include "fpgadatareader.h"


extern uint64_t g_ulEventFrameNo;
extern uint64_t g_ulFixedEventNo;
extern bool isGetFixedVec;

#define gravity			9.80665			// this is based on what? This constant changes based on your position on the globe -> Ger: 9,81
#define accResolution	0.000598569
#define resolution		0.000133162
#define magResolution	0.146489


// Byte 0: {1'b0, X[6:0]}
// Byte 1: {1'b0, C[0], X[8:7], A[3:0]}
unsigned int FPGADataReader::getColumn(unsigned char data[EVENT_SIZE]) {
	unsigned int last7bit = 0;
	last7bit += static_cast<unsigned char>(data[0]) & 0x7F;
	unsigned int result = last7bit + ((static_cast<unsigned char>(data[1]) & 0x30) << 3);

	if ((data[1] & 0x40) != 0) result = PIXELS_PER_COL - 1 - result;
	
	return result;
}


// Byte 0: {1'b1, Y[6:0]}
// Byte 1: {1'b1, Y[9:7], T[3:0]}
unsigned int FPGADataReader::getRow(unsigned char data[EVENT_SIZE]) {
	unsigned int last7bit = 0;
	last7bit += static_cast<unsigned char>(data[0]) & 0x7F;
	return last7bit + ((static_cast<unsigned char>(data[1]) & 0x70) << 3);
}


// Byte 1: {1'b1, Y[9,7], T[3:0]}
// Byte 2: {1'b1, T[10:4]}
// Byte 3: {1'b1, T[17:11]}
unsigned int FPGADataReader::getTimeStamp(unsigned char data[EVENT_SIZE]) {
	unsigned int last4bit = 0;
	last4bit += static_cast<unsigned int>(data[1] & 0x0F);
	unsigned int mid7bit = 0;
	mid7bit += static_cast<unsigned int>(data[2] & 0x7F);
	return last4bit + (mid7bit << 4) + (static_cast<unsigned int>(data[3] & 0x7F) << 11);
}


// Byte 1: {1'b0, C[0], X[8:7], A[3:0]}
// Byte 2: {3'b0, A[8:4]}
unsigned int FPGADataReader::getBrightness(unsigned char data[EVENT_SIZE]) { //ADC
	unsigned int last4bit = 0;
	last4bit += static_cast<unsigned int>(data[1] & 0x0F);
	return last4bit + (static_cast<unsigned int>(data[2] & 0x1F) << 4);
}


bool FPGADataReader::isForcefirePixel(unsigned char data[EVENT_SIZE]) { //column event from force fire, carrying ADC
	return ((unsigned char)data[3] & 0x01) == 1;
}


// Byte 0: {1'b0, X[6:0]}
// Byte 1: {1'b0, C[0], X[8:7], A[3:0]}
// Byte 2: {3'b0, A[8:4]}
// Byte 3: {8'b0}
bool FPGADataReader::isColumnEvent(unsigned char data[EVENT_SIZE]) {
	bool result = (((unsigned char)data[0] & 0x80) == 0);
	
	if (result) s_uiEventType = (unsigned int)((unsigned char)data[3] & 0x03);
	
	return result;
}


// Byte 0: {1'b1, Y[6:0]}
// Byte 1: {1'b1, Y[9:7], T[3:0]}
// Byte 2: {1'b1, T[10:4]}
// Byte 3: {1'b1, T[17:11]}
bool FPGADataReader::isRowEvent(unsigned char data[EVENT_SIZE]) {
	return (((unsigned char)data[0] & 0x80) > 0 && ((unsigned char)data[1] & 0x80) > 0 &&
			((unsigned char)data[2] & 0x80) > 0 && ((unsigned char)data[3] & 0xC0) == 0x80);
}


// Byte 0: {8'b1}
// Byte 1: {8'b1}
// Byte 2: {8'b1}
// Byte 3: {8'b1}
bool FPGADataReader::isSpecialEvent(unsigned char data[EVENT_SIZE]) {
	unsigned char a = (unsigned char)data[0];
	unsigned char b = (unsigned char)data[1];
	unsigned char c = (unsigned char)data[2];
	unsigned char d = (unsigned char)data[3];

	bool result = (d == 0xff && b == 0xff && c == 0xff);

	if (result) s_uiSpecialEventType = (unsigned int)(d & 0x03);

	return result;
}


unsigned int FPGADataReader::MapTime(unsigned char data[EVENT_SIZE]) {
	if (isRowEvent(data)) {
		s_uiLastRow = s_uiCurrentRow;
		s_uiLastTFromFPGA = s_uiTFromFPGA;
		s_uiCurrentRow = getRow(data);
		s_uiTFromFPGA = getTimeStamp(data);
	}

	return 1;
}


// Flag_GYROS_A[4:0]=5'b00_101
// Flag_GYROS_A[4:0]={GYROS_A[31:30], GYROS_A[23], GYROS_A[15], GYROS_A[7]}
// Flag_GYROS_B[4:0]=5'b01_101
// Flag_ACC_A[4:0]=5'b10_101
// Flag_ACC_B[4:0]=5'b11_101
FPGADataReader::IMUDATAType FPGADataReader::isIMUData(unsigned char data[4]) {
	if ((data[0] & 0x80) == 0x80 && (data[1] & 0x80) == 0 && (data[2] & 0x80) == 0x80) {
		//Flag_GYROS_A[4:0]={GYROS_A[31:30], GYROS_A[23], GYROS_A[15], GYROS_A[7]}
		if (0x00 == (data[3] & 0xC0)) {
			s_IMUData.x_GYROS = getIMU_X(data) * resolution;

			if (isGetFixedVec) {
				s_IMUData.t_GYROS = s_uiFixedEventTCounter;
				s_IMUData.frameNo = g_ulFixedEventNo;
			} else {
				s_IMUData.t_GYROS = s_uiEventTCounter;
				s_IMUData.frameNo = g_ulEventFrameNo + 1;
			}

			return FPGADataReader::GYROS_A_DATA;
		} else if (0x40 == (data[3] & 0xC0)) {
			s_IMUData.y_GYROS = getIMU_Y(data) * resolution;
			s_IMUData.z_GYROS = getIMU_Z(data) * resolution;
			return FPGADataReader::GYROS_B_DATA;
		} else if (0x80 == (data[3] & 0xC0)) {
			s_IMUData.x_ACC = getIMU_X(data) * accResolution;
			return FPGADataReader::ACC_A_DATA;
		} else if (0xC0 == (data[3] & 0xC0)) {
			s_IMUData.y_ACC = getIMU_Y(data) * accResolution;
			s_IMUData.z_ACC = getIMU_Z(data) * accResolution;

			if (isGetFixedVec) s_IMUData.t_ACC = s_uiFixedEventTCounter;
			else s_IMUData.t_ACC = s_uiEventTCounter;

			return FPGADataReader::ACC_B_DATA;
		}
	} else if ((data[0] & 0x80) == 0 && (data[1] & 0x80) == 0 && (data[2] & 0x80) == 0x80) {
		if (0x00 == (data[3] & 0xC0)) {
			s_IMUData.x_MAG = getIMU_X(data) * magResolution;
			return FPGADataReader::GYROS_OFST_A_DATA;
		} else if (0x40 == (data[3] & 0xC0)) {
			s_IMUData.y_MAG = getIMU_Y(data) * magResolution;
			s_IMUData.z_MAG = getIMU_Z(data) * magResolution;

			if (isGetFixedVec) s_IMUData.t_MAG = s_uiFixedEventTCounter;
			else s_IMUData.t_MAG = s_uiEventTCounter;

			return FPGADataReader::GYROS_OFST_B_DATA;
		} else if (0x80 == (data[3] & 0xC0)) {
			s_IMUData.x_TEMP = getIMU_X(data);
			return FPGADataReader::ACC_OFST_A_DATA;
		} else if (0xC0 == (data[3] & 0xC0)) {
			return FPGADataReader::ACC_OFST_B_DATA;
		}
	}

	return FPGADataReader::NO_IMU_DATA;
}


// X_GYROS[15:0] = { GYROS_A[17:16], GYROS_A[14:8], GYROS_A[6:0] }
// Y_GYROS[5:0] = { GYROS_A[29:24] }
// Timer_GYROS[4:1] = { GYROS_A[22:19] }
int16_t FPGADataReader::getIMU_X(unsigned char data[4]) {
	int16_t low7bit = static_cast<int16_t>(data[0] & 0x7F);
	int16_t mid7bit = static_cast<int16_t>(data[1] & 0x7F);
	int16_t high2bit = static_cast<int16_t>(data[2] & 0x03);

	s_uiHighGYROS_T = static_cast<uint16_t>(data[2] & 0x78) >> 2;

	s_iLowGYROS_Y = static_cast<int16_t>(data[3] & 0x3F);

	return low7bit + (mid7bit << 7) + (high2bit << 14);
}


// Y_GYROS[5:0] = { GYROS_A[29:24] }
// Y_GYROS[15:6] = { GYROS_B[10:8], GYROS_B[6:0] }
int16_t FPGADataReader::getIMU_Y(unsigned char data[4]) {
	int16_t low6bit = s_iLowGYROS_Y;
	int16_t mid7bit = static_cast<int16_t>(data[0] & 0x7F);
	int16_t high3bit = static_cast<int16_t>(data[1] & 0x07);

	return low6bit + (mid7bit << 6) + (high3bit << 13);
}


// Z_GYROS[15:0] = { GYROS_B[29:24], GYROS_B[21:16], GYROS_B[14:11] }
int16_t FPGADataReader::getIMU_Z(unsigned char data[4]) {
	int16_t low4bit = static_cast<int16_t>(data[1] & 0x78);
	int16_t mid6bit = static_cast<int16_t>(data[2] & 0x3F);
	int16_t high6bit = static_cast<int16_t>(data[3] & 0x3F);

	return (low4bit >> 3) + (mid6bit << 4) + (high6bit << 10);
}


// Timer_GYROS[4:1] = { GYROS_A[22:19] }
// Timer_GYROS[0] = { GYROS_B[22] }
uint16_t FPGADataReader::getIMU_T(unsigned char data[4]) {
	return s_uiHighGYROS_T + (static_cast<uint16_t>(data[2] & 0x40) >> 6);
}


unsigned int FPGADataReader::s_uiCurrentRow         = PIXELS_PER_COL;
unsigned int FPGADataReader::s_uiLastRow            = PIXELS_PER_COL;
unsigned int FPGADataReader::s_uiTFromFPGA          = 0;
unsigned int FPGADataReader::s_uiLastTFromFPGA      = 0;
unsigned int FPGADataReader::s_uiMapT               = 0;
unsigned int FPGADataReader::s_uiEventType          = 0;
unsigned int FPGADataReader::s_uiSpecialEventType   = 0;

int16_t FPGADataReader::s_iLowGYROS_Y               = 0;
uint16_t FPGADataReader::s_uiHighGYROS_T            = 0;
IMUData FPGADataReader::s_IMUData;                  // this is useless isnt it?
uint32_t FPGADataReader::s_uiEventTCounter          = 0;
uint32_t FPGADataReader::s_uiFixedEventTCounter     = 0;

