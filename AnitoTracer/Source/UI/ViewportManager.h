#pragma once
#include "ViewportScreen.h"
#include "UIManager.h"
#include <vector>

namespace GDEngine
{
	class ViewportManager
	{
	private:
		ViewportManager();
		~ViewportManager();
		ViewportManager(const ViewportManager&);
		ViewportManager& operator=(const ViewportManager&);

	private:
		std::vector<ViewportScreen*> viewports;

	public:
		static ViewportManager* getInstance();
		static void initialize();
		static void destroy();
		void update();

		void createViewport();
		void deleteViewport(ViewportScreen* viewport);
		void deleteAllViewports();
		void addViewport(UIScreen* viewport);
		void setNumViewports(int count);
		std::vector<ViewportScreen*> getViewports();


	private:
		static ViewportManager* P_SHARED_INSTANCE;

	};
}