#pragma once
class Exposer
{
public:
	Exposer();
	~Exposer();
	void ExposeMethods(std::vector<GetSetinterface*>* methods, const GameContext& gameContext);

private:
	
	float m_StandardItemWidth = 60;

};

