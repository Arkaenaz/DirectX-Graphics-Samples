#include "pch.h"
#include "SceneToolsScreen.h"

#include "EngineState.h"

SceneToolsScreen::SceneToolsScreen() : UIScreen("Profiler Screen") {}
SceneToolsScreen::~SceneToolsScreen() {}

void SceneToolsScreen::drawUI()
{
	ImGui::Begin("Scene Tools", &isActive, ImGuiWindowFlags_NoTitleBar);

	if (EngineState::getInstance()->getState() == EngineState::PLAY) {
		if (this->buttonCentered("Stop")) {
			// Stop Game
			EngineState::getInstance()->setState(EngineState::EDIT);
		}
	}
	else {
		if (this->buttonCentered("Play")) {
			// Start Game Simulation
			EngineState::getInstance()->setState(EngineState::PLAY);
		}
	}

	ImGui::End();
}
