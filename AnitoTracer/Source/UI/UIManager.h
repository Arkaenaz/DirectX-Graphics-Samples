#pragma once
#include "CommandContext.h"
#include "UIInclude.h"

class UINames {
public:
	typedef std::string String;
	const String PROFILER_SCREEN = "PROFILER SCREEN";
	const String MENU_SCREEN = "MENU SCREEN";
	const String INSPECTOR_SCREEN = "INSPECTOR SCREEN";
	const String HIERARCHY_SCREEN = "HIERARCHY SCREEN";
	const String CREDITS_SCREEN = "CREDITS SCREEN";
	const String COLOR_PICKER_SCREEN = "COLOR PICKER SCREEN";
	const String SCENE_TOOLS_SCREEN = "SCENE TOOLS SCREEN";
};

class UIScreen;

class UIManager
{
	public:
		// Config
		const int APP_NUM_FRAMES_IN_FLIGHT = 2;
		const int APP_NUM_BACK_BUFFERS = 2;
		static const int WINDOW_WIDTH = 1440;
		static const int WINDOW_HEIGHT = 900;

		static int RESIZE_WIDTH;
		static int RESIZE_HEIGHT;

		typedef std::string String;
		typedef std::vector<UIScreen*> UIScreenList;
		typedef std::unordered_map<String, UIScreen*> UIScreenTable;

	private:
		UIScreenList list;
		UIScreenTable table;

		static UIManager* sharedInstance;
		UIManager();
		~UIManager();
		UIManager(UIManager const&) {};
		UIManager& operator= (UIManager const&) {};

	public:
		static UIManager* getInstance();
		static void initialize();
		static void destroy();

		void update();
		void draw(GraphicsContext& CmdContext);
		void openWindow(String name);
		void drawDockspace();
		void setActive(std::string name);
		void addViewport(UIScreen* viewport);
		void setUIStyle();
		void AlignForWidth(float width, float alignment = 0.5f);
};

