/*******************************************************************************
 * Copyright (c) 2005 - 2013 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *  Alois Zoitl - initial API and implementation and/or initial documentation
 *  Martin Melik-Merkumians - updates timer handler to use monotonic clock
 *******************************************************************************/
#include <fortenew.h>
#include "pctimeha.h"
#include "../../core/devexec.h"
#include <time.h>
#include <sys/time.h>
#include "../utils/timespec_utils.h"

CTimerHandler* CTimerHandler::createTimerHandler(CDeviceExecution& pa_poDeviceExecution){
  return new CPCTimerHandler(pa_poDeviceExecution);
}

CPCTimerHandler::CPCTimerHandler(CDeviceExecution& pa_poDeviceExecution) : CTimerHandler(pa_poDeviceExecution)  {
}

CPCTimerHandler::~CPCTimerHandler(){
  disableHandler();
}

void CPCTimerHandler::run(){
  struct timespec stReq;
  stReq.tv_sec = 0;
  stReq.tv_nsec = (1000000 / getTicksPerSecond()) * 1000;
  
  struct timespec stOldTime;
  struct timespec stNewTime;
  struct timespec stReqTime;
  // Timer interval is 1ms
  stReqTime.tv_sec = 0;
  stReqTime.tv_nsec = (1000000 / getTicksPerSecond()) * 1000;
  struct timespec stDiffTime;
  struct timespec stRemainingTime = { 0, 0 };

  clock_gettime(CLOCK_MONOTONIC, &stOldTime);
  while(isAlive()){

    nanosleep(&stReq, NULL);

    clock_gettime(CLOCK_MONOTONIC, &stNewTime);

    timespecSub(&stNewTime, &stOldTime, &stDiffTime);

    timespecAdd(&stRemainingTime, &stDiffTime, &stRemainingTime);

    while(!timespecLessThan(&stRemainingTime, &stReqTime)){
      nextTick();
      timespecSub(&stRemainingTime, &stReqTime, &stRemainingTime);
    }
    stOldTime = stNewTime;  // in c++ this should work fine
  } 
}

void CPCTimerHandler::enableHandler(void){
  start();
}

void CPCTimerHandler::disableHandler(void){
  end(); 
}

void CPCTimerHandler::setPriority(int ){
  //TODO think on hwo to handle this.
}

int CPCTimerHandler::getPriority(void) const {
  //TODO think on hwo to handle this.
  return 1;
}
