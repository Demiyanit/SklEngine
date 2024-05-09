#pragma once

class IRenderer {
public: 
	virtual void Render()=0;
	virtual ~IRenderer()=default;
private:
	friend class Engine;
};