#include "ViewportManager.h"

namespace GDEngine {
	ViewportManager* ViewportManager::P_SHARED_INSTANCE = nullptr;

	ViewportManager::ViewportManager()
	{
		Logger::log(this, "Initialized");
	}

	ViewportManager::~ViewportManager()
	{
		Logger::log(this, "Destroyed");
	}

	ViewportManager::ViewportManager(const ViewportManager&) {}

	ViewportManager* ViewportManager::getInstance()
	{
		//if (P_SHARED_INSTANCE == NULL)
		//	P_SHARED_INSTANCE = new ViewportManager();

		return P_SHARED_INSTANCE;
	}

	void ViewportManager::initialize()
	{
		if (P_SHARED_INSTANCE)
		{
			Logger::throw_exception("Viewport Manager already created");
		}
		P_SHARED_INSTANCE = new ViewportManager();
	}

	void ViewportManager::destroy()
	{
		delete P_SHARED_INSTANCE;
	}

	void ViewportManager::update()
	{
		for (ViewportScreen* viewport : this->viewports)
		{
			viewport->draw();
		}

	}

	void ViewportManager::createViewport()
	{
		ViewportScreen* viewport = new ViewportScreen(this->viewports.size());

		this->viewports.push_back(viewport);
		//UIManager::getInstance()->addViewport(viewport);
	}

	void ViewportManager::deleteViewport(ViewportScreen* viewport)
	{
		int index = 0;
		for (int i = 0; i < this->viewports.size(); i++)
		{
			if (this->viewports[i] == viewport)
			{
				this->viewports.erase(this->viewports.begin() + index);
			}

			index++;
		}
		delete viewport;
	}

	void ViewportManager::deleteAllViewports()
	{
		for (ViewportScreen* viewport : this->viewports)
		{
			delete viewport;
		}

		this->viewports.clear();
	}

	void ViewportManager::addViewport(UIScreen* viewport)
	{
		ViewportScreen* c_viewport = (ViewportScreen*)viewport;
		this->viewports.push_back(c_viewport);

	}

	void ViewportManager::setNumViewports(int count)
	{
		while (viewports.size() > count)
		{
			this->deleteViewport(viewports.back());
		}

		while (viewports.size() < count)
		{
			this->createViewport();
		}
	}

	std::vector<ViewportScreen*> ViewportManager::getViewports()
	{
		return this->viewports;
	}
}