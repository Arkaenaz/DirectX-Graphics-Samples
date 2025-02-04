#include "pch.h"
#include "EngineState.h"

EngineState* EngineState::sharedInstance = nullptr;
EngineState* EngineState::getInstance()
{
	return sharedInstance;
}

void EngineState::initialize()
{
	sharedInstance = new EngineState();
	//sharedInstance->init();
}

void EngineState::destroy()
{
	delete sharedInstance;
}

EngineState::EngineState() { this->state = EDIT; }
EngineState::~EngineState() {}