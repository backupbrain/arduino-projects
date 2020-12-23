/**
 * This class represents an Arduino CPU
 * 
 * It is a singleton (you cannot create more than one instance) because
 * an Arduino only has one CPU
 * 
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2015-01-28
 */

#include "CPU.h"
CPU* CPU::cpuInstance(NULL);

CPU::CPU(){}

CPU::CPU(const CPU& cpu){}

CPU::~CPU()
{
    cpuInstance = NULL;
}

const CPU& CPU::Instance()
{
    if (cpuInstance == NULL)
    {
        cpuInstance = new CPU();
    }

    return *(cpuInstance); // want to avoid pointer as user may deallocate it, so i used const referense
}

