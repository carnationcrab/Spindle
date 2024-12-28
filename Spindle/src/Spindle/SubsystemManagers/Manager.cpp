#include "Manager.h"
#include <cassert>

// initialize static instance
Manager Manager::sInstance;

Manager::Manager()
    : isInitialized(false)
{
}

Manager::~Manager()
{
    // ensure proper shutdown
    assert(!isInitialized && "Manager was not properly shut down!");
}

void Manager::startUp()
{
    assert(!isInitialized && "Manager already initialized!");

    // initialize subsystem
    isInitialized = true;

}

void Manager::shutDown()
{
    assert(isInitialized && "Manager not initialized!");

    stopAllProcesses();

    isInitialized = false;
}


void Manager::stopAllProcesses()
{
    assert(isInitialized && "Manager not initialized!");
}
