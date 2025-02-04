#pragma once

class EngineState
{
public:
	enum EditorState { EDIT = 0, PLAY, PAUSE };
	static EngineState* getInstance();
	static void initialize();
	static void destroy();

	EditorState getState() { return this->state; }
	void setState(EditorState engState) { this->state = engState; }

private:
	EngineState();
	~EngineState();
	EngineState(EngineState const&) {};
	EngineState& operator = (EngineState const&) {};
	static EngineState* sharedInstance;

	EditorState state;
};

