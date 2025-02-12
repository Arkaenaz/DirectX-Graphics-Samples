#pragma once
#include "UIScreen.h"

#include "../../Libs/LegitProfiler/ImGuiProfilerRenderer.h"
#include "../../Libs/LegitProfiler/ProfilerTask.h"

class ProfilerScreen : public UIScreen
{
private:
	ImGuiUtils::ProfilerGraph* profiler;
	legit::ProfilerTask* task;

	public:
		ProfilerScreen();
		~ProfilerScreen();

		void drawUI();
};

