#include "pch.h"
#include "ProfilerScreen.h"

#include "EngineProfiling.h"

#include "CommandContext.h"
#include "CommandListManager.h"

#include "Profiler.h"
#include "IconsFontAwesome4.h"

namespace Graphics { extern ID3D12Device* g_Device; }

namespace EngineProfiling
{
	extern float cpuTime;
	extern float gpuTime;
}

ProfilerScreen::ProfilerScreen() : UIScreen("Profiler Screen")
{
	size_t frames = ImGui::GetIO().Framerate;
	this->profiler = new ImGuiUtils::ProfilerGraph(frames);

	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	ImFontConfig fontConfig;
	//	fontConfig.MergeMode = true;
	//	fontConfig.GlyphMinAdvanceX = 15.0f; // Use if you want to make the icon monospaced
	//	fontConfig.PixelSnapH = true;
	//	fontConfig.GlyphOffset.y -= 2.5f;
	//	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	//	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FA, 15.0f, &fontConfig, icon_ranges);
	//}

	gCPUProfiler.Initialize(5, 1024);
	ID3D12CommandQueue* cmdQueue = Graphics::g_CommandManager.GetCommandQueue();
	Span<ID3D12CommandQueue*> queues(&cmdQueue, 1);
	gGPUProfiler.Initialize(Graphics::g_Device, queues, 5, 3, 1024, 128, 32);
}

ProfilerScreen::~ProfilerScreen()
{
	gCPUProfiler.Shutdown();
	gGPUProfiler.Shutdown();
}

void ProfilerScreen::drawUI()
{
	DrawProfilerHUD();

	ImGui::Begin("Profiler", &isActive);

    float cpuTime = EngineProfiling::cpuTime;
    float gpuTime = EngineProfiling::gpuTime;

	ImGuiIO& io = ImGui::GetIO();
	if (io.MouseDown[0])
		ImGui::Text("Mouse Down: %d", io.MouseDown[0]);
	ImGui::Text("Mouse Pos: (%f, %f)", io.MousePos.x, io.MousePos.y);
	ImGui::Text("CPU %7.3f ms, GPU %7.3f ms\n", cpuTime, gpuTime);

	ImGui::Text("%f ms/frame, %f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}
