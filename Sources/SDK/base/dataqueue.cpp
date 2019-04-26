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

#include "dataqueue.h"
#include <iostream>
#include <cstring>
#include "../include/celextypes.h"

using namespace std;

DataQueue::DataQueue() : m_size(0) {}
DataQueue::~DataQueue() {}

void DataQueue::push(unsigned char* pData, long length) {
    unsigned char* pBuffer = new unsigned char[length];
    memcpy(pBuffer, pData, length);
    DataInfo dataIn;
    dataIn.pData    = pBuffer;
    dataIn.length   = length;
    m_queue.push(dataIn);
    m_size += dataIn.length;
}

void DataQueue::pop(unsigned char*& pData, long* length) {
    if (m_queue.size() <=0) return;

    DataInfo dataOut = m_queue.front();
    m_queue.pop();
    m_size -= dataOut.length;

    if (!dataOut.pData) {
        *length = 0;
    } else {
        pData   = dataOut.pData;
        *length = dataOut.length;
    }
}

unsigned long DataQueue::size() { return m_size; }

void DataQueue::clear() {
    while (m_queue.size() > 0) {
        DataInfo dataToDelete;
        dataToDelete = m_queue.front();
        delete [] dataToDelete.pData;
        m_queue.pop();
    }

    m_size = 0;
}

//----------------------------------------
//------------- CirDataQueue -------------
CirDataQueue::CirDataQueue(int queueCapacity) : m_iHead(0), m_iTail(0), m_iQueueLenth(0), m_iQueueCapacity(queueCapacity) {
    for (int i = 0; i < queueCapacity; ++i) {
        dataIn.pData = new unsigned char[PIXELS_NUMBER];

        DataInfo dataIn;
        for (int i = 0; i < PIXELS_NUMBER; ++i) dataIn.pData[i] = 0;
        
		dataIn.length = 0;
        m_queue.push_back(dataIn);
    }
}

CirDataQueue::~CirDataQueue() { m_queue.clear(); }

int CirDataQueue::getLength() { return m_iQueueLenth; }

int CirDataQueue::getCapacity() { return m_iQueueCapacity; }

bool CirDataQueue::enqueue(unsigned char *pData) {
	if (0 == m_iQueueCapacity) return false;

    if (isFull()) {
        std::cout << "CirDataQueue::enqueue: queue is full!";
        return false;
    } else {
        memcpy(m_queue[m_iTail].pData, pData, PIXELS_NUMBER);
        ++m_iTail;
        m_iTail = m_iTail % m_iQueueCapacity;
        ++m_iQueueLenth;

        return true;
    }
}

bool CirDataQueue::dequeue(unsigned char *&pData) {
    if (isEmpty()) {
        return false;
    } else {
        DataInfo dataOut = m_queue[m_iHead];
        m_iHead++;
        m_iHead = m_iHead % m_iQueueCapacity;
        --m_iQueueLenth;                        // why --[...] ??
        pData = dataOut.pData;

        return true;
    }
}

bool CirDataQueue::isEmpty() {
    return m_iQueueLength == 0;
}

bool CirDataQueue::isFull() {
    return m_iQueueLenth == m_iQueueCapacity;
}

void CirDataQueue::clear() {
    std::cout << "CirDataQueue::clear" << std::endl;
    m_iHead         = 0;
    m_iTail         = 0;
    m_iQueueLenth   = 0;
}

unsigned char* CirDataQueue::head() {
    return (0 != m_iQueueCapacity) ? m_queue[m_iTail].pData : NULL;
}