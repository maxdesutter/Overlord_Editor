#include  "stdafx.h"
#include "BaseBehaviorComponent.h"
#include "BinaryReader.h"

BaseBehaviorComponent::BaseBehaviorComponent()
{
}


BaseBehaviorComponent::~BaseBehaviorComponent()
{
}



void BaseBehaviorComponent::Draw(const GameContext& gameContext)
{
	BehaviorDraw(gameContext);
}

void BaseBehaviorComponent::Update(const GameContext& gameContext)
{
	if(gameContext.pGameTime->IsRunning())
		BehaviorUpdate(gameContext);
}

void BaseBehaviorComponent::Initialize(const GameContext& gameContext)
{
	BehaviorInitialize(gameContext);
}

void BaseBehaviorComponent::WriteToFile(BinaryWriter* writer)
{
	writer->Write(unsigned char(8));
	writer->WriteString(m_ComponentClassName);
	writer->Write((int)GetRegisteredMethods()->size());


	for(int i =0;i < (int)GetRegisteredMethods()->size();i++)
	{
		writer->Write((int)GetRegisteredMethods()->at(i)->type);
		switch (GetRegisteredMethods()->at(i)->type)
		{
		case TYPE_UNKNOWN:
			{
				
			} break;
		case TYPE_INT:
			{
				writer->Write(sizeof(int));
				GetSetMethods<int>* methodStruct = ((GetSetMethods<int>*)GetRegisteredMethods()->at(i));
				writer->Write(methodStruct->Get());
			} break;
		case TYPE_UINT:
			{
				writer->Write(sizeof(unsigned int));

				GetSetMethods<unsigned int>* methodStruct = ((GetSetMethods<unsigned int>*)GetRegisteredMethods()->at(i));
				writer->Write(methodStruct->Get());

			}break;
		case TYPE_BOOL:
			{
				writer->Write(sizeof(bool));

				GetSetMethods<bool>* methodStruct = ((GetSetMethods<bool>*)GetRegisteredMethods()->at(i));
				writer->Write(methodStruct->Get());
			} break;
		case TYPE_FLOAT:
			{
				writer->Write(sizeof(float));

				GetSetMethods<float>* methodStruct = ((GetSetMethods<float>*)GetRegisteredMethods()->at(i));
				writer->Write(methodStruct->Get());
			} break;
		case TYPE_STRING:
			{
				//string already stores size this i sjust throw away value
			writer->Write(sizeof(int));
			GetSetMethods<std::string>* methodStruct = ((GetSetMethods<std::string>*)GetRegisteredMethods()->at(i));
				writer->WriteString(methodStruct->Get());
			} break;
		case TYPE_XMFLOAT3:
			{
				writer->Write(sizeof(DirectX::XMFLOAT3));

				GetSetMethods<DirectX::XMFLOAT3>* methodStruct = ((GetSetMethods<DirectX::XMFLOAT3>*)GetRegisteredMethods()->at(i));
				writer->Write(methodStruct->Get());
			} break;
		case TYPE_XMFLOAT4:
			{
			writer->Write(sizeof(DirectX::XMFLOAT4));

				GetSetMethods<DirectX::XMFLOAT4>* methodStruct = ((GetSetMethods<DirectX::XMFLOAT4>*)GetRegisteredMethods()->at(i));
				writer->Write(methodStruct->Get());
			} break;
		default: ;
		}
	}
}

void BaseBehaviorComponent::ReadinParameters(BinaryReader* reader)
{
	int numcomponents = reader->Read<int>();
	for (int i = 0; i < numcomponents; i++)
	{
		ReflectionTypes type = (ReflectionTypes)reader->Read<int>();
		unsigned int size = reader->Read<unsigned int>();

		//check for size 
		if(i>= (int)GetRegisteredMethods()->size())
		{
			if (type != ReflectionTypes::TYPE_STRING)
			{
				reader->MoveBufferPosition(size);
				continue;
			}
			int sizestring = reader->Read< int>();
			reader->MoveBufferPosition(sizestring);
			continue;
		}
		if(type!= GetRegisteredMethods()->at(i)->type)
		{
			if (type != ReflectionTypes::TYPE_STRING)
			{
				reader->MoveBufferPosition(size);
				continue;
			}
			int sizestring = reader->Read< int>();
			reader->MoveBufferPosition(sizestring);
			continue;
		}
		switch (GetRegisteredMethods()->at(i)->type)
		{
		case TYPE_UNKNOWN:
		{

		} break;
		case TYPE_INT:
		{
			GetSetMethods<int>* methodStruct = ((GetSetMethods<int>*)GetRegisteredMethods()->at(i));
			methodStruct->Set( reader->Read<int>());
		} break;
		case TYPE_UINT:
		{
			GetSetMethods<unsigned int>* methodStruct = ((GetSetMethods<unsigned int>*)GetRegisteredMethods()->at(i));
			methodStruct->Set(reader->Read<unsigned int>());

		}break;
		case TYPE_BOOL:
		{
			GetSetMethods<bool>* methodStruct = ((GetSetMethods<bool>*)GetRegisteredMethods()->at(i));
			methodStruct->Set(reader->Read<bool>());
		} break;
		case TYPE_FLOAT:
		{
			GetSetMethods<float>* methodStruct = ((GetSetMethods<float>*)GetRegisteredMethods()->at(i));
			methodStruct->Set(reader->Read<float>());
		} break;
		case TYPE_STRING:
		{
			GetSetMethods<std::string>* methodStruct = ((GetSetMethods<std::string>*)GetRegisteredMethods()->at(i));
			methodStruct->Set(reader->ReadRegString());
		} break;
		case TYPE_XMFLOAT3:
		{
			GetSetMethods<DirectX::XMFLOAT3>* methodStruct = ((GetSetMethods<DirectX::XMFLOAT3>*)GetRegisteredMethods()->at(i));
			methodStruct->Set(reader->Read<DirectX::XMFLOAT3>());
		} break;
		case TYPE_XMFLOAT4:
		{
			GetSetMethods<DirectX::XMFLOAT4>* methodStruct = ((GetSetMethods<DirectX::XMFLOAT4>*)GetRegisteredMethods()->at(i));
			methodStruct->Set(reader->Read<DirectX::XMFLOAT4>());
		} break;
		default:;
		}
	}
}
