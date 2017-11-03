#ifndef CONSTANTS_H
#define CONSTANTS_H

static int _ADCNumSteps=2;
static int _halfPacketSize=363;
static int _ADCShiftBlockSize=16;
static int _ADCBlocksPerStep=22;
static int _inputsPerDevice=8;
static int _channelsInPacket=2;
static int _numOfDevices=4;
static int _overlap=0;

static int _srcPort=1024;
static int _dstPort=4096;
static int _listenPort=1024;


static int _nChannels=(_inputsPerDevice*_channelsInPacket*_numOfDevices);
static int _bufferSize=(_ADCShiftBlockSize*_ADCBlocksPerStep);
static int _historyBufferSize=(_ADCShiftBlockSize*_ADCBlocksPerStep*_ADCNumSteps - _overlap*(_ADCNumSteps-1));
static int _scanBufferSize=(_ADCShiftBlockSize*_ADCBlocksPerStep*_ADCNumSteps);


#endif // CONSTANTS_H
