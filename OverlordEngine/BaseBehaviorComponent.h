#pragma once
#include "BaseComponent.h"

class BinaryReader;
class BaseBehaviorComponent :public BaseComponent
{
public:
	BaseBehaviorComponent();
	virtual ~BaseBehaviorComponent();
	friend class ComponentFactory;
protected:
	virtual void BehaviorUpdate(const GameContext& gameContext)=0;
	virtual void BehaviorDraw(const GameContext& gameContext)=0;
	virtual void BehaviorInitialize(const GameContext& gameContext)=0;

private:
	void Draw(const GameContext& gameContext) final;
	void Update(const GameContext& gameContext) final;
	void Initialize(const GameContext& gameContext) final;
	void WriteToFile(BinaryWriter* writer) final;
	void ReadinParameters(BinaryReader* reader);


};

