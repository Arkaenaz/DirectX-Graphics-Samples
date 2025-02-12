#pragma once

#include <d3d12.h>

class Viewport
{
private:
	D3D12_VIEWPORT m_viewport;

private:
	friend class DeviceContext;

public:
	Viewport(FLOAT topLeftX, FLOAT topLeftY, FLOAT width, FLOAT height, FLOAT minDepth,
		FLOAT maxDepth);
	~Viewport();

public:
	void setPosition(FLOAT topLeftX, FLOAT topLeftY);
	void setSize(FLOAT width, FLOAT height);
	void setDepth(FLOAT minDepth, FLOAT maxDepth);
	
};