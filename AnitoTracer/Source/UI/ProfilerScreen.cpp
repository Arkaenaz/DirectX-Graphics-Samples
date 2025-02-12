#include "pch.h"
#include "ProfilerScreen.h"

#include "EngineProfiling.h"

namespace EngineProfiling
{
	extern float cpuTime;
	extern float gpuTime;
}

ProfilerScreen::ProfilerScreen() : UIScreen("Profiler Screen") {}
ProfilerScreen::~ProfilerScreen() {}

void ProfilerScreen::drawUI()
{
	ImGui::Begin("Profiler", &isActive);

    float cpuTime = EngineProfiling::cpuTime;
    float gpuTime = EngineProfiling::gpuTime;

    ImGui::Text("CPU %7.3f ms, GPU %7.3f ms\n", cpuTime, gpuTime);

	ImGui::Text("%f ms/frame, %f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}
