#include "OverlordEditorPCH.h"
#include "Exposer.h"
#include "Material.h"

#include "ImgUi.h"
#include "ModelAnimator.h"
Exposer::Exposer()
{
}


Exposer::~Exposer()
{
}

void Exposer::ExposeMethods(std::vector<GetSetinterface*>* methods, const GameContext& gameContext)
{

	for (auto method : (*methods))
	{
		
		if (method)
		{
			switch (method->type)
			{
			case TYPE_UNKNOWN:
			{

			} break;
			case TYPE_INT:
			{
				GetSetMethods<int>* methodStruct = ((GetSetMethods<int>*)method);
				int before = methodStruct->Get(), after = before;
				ImGui::DragInt(methodStruct->varName.c_str(), &after);
				if (before != after)
					methodStruct->Set(after);

			} break;
			case TYPE_UINT:
			{
				GetSetMethods<UINT>* methodStruct = ((GetSetMethods<UINT>*)method);
				int before = methodStruct->Get(), after;
				after = before;
				ImGui::InputInt(methodStruct->varName.c_str(), &after);
				if (after != before)
					methodStruct->Set(after);
			} break;
			case TYPE_BOOL:
			{
				GetSetMethods<bool>* methodStruct = ((GetSetMethods<bool>*)method);
				bool before = methodStruct->Get(), after;
				after = before;
				ImGui::Checkbox(methodStruct->varName.c_str(), &after);
				if (after != before)
					methodStruct->Set(after);
			} break;
			case TYPE_FLOAT:
			{
				GetSetMethods<float>* methodStruct = ((GetSetMethods<float>*)method);
				float variable = methodStruct->Get();
				ImGui::DragFloat(methodStruct->varName.c_str(), &variable,0.1f);
				methodStruct->Set(variable);
			} break;
			case TYPE_STRING:
			{
				ImGui::PushItemWidth(-1);
				GetSetMethods<std::string>* methodStruct = ((GetSetMethods<std::string>*)method);
				char* before = new char[100], *after = new char[100];

				strcpy_s(before, methodStruct->Get().length() + 1, methodStruct->Get().c_str());
				strcpy_s(after, methodStruct->Get().length() + 1, methodStruct->Get().c_str());

				ImGui::Text(methodStruct->varName.c_str());
				if (ImGui::InputText(methodStruct->varName.c_str(), after, 100, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					if (std::string(before) != std::string(after))
					{
						methodStruct->Set(std::string(after));
					}
				}

				delete[]before;
				delete[]after;
				ImGui::PopItemWidth();
			}break;
			case TYPE_MATERIAL:
			{
				//change for drag and drop
				std::vector<UINT>keys = gameContext.pMaterialManager->GetAllMaterialKeys();

				GetSetMethods<ReflectionMaterialStruct>* methodStruct = ((GetSetMethods<ReflectionMaterialStruct>*)method);
				std::wstring nameW = gameContext.pMaterialManager->GetMaterial(methodStruct->Get().materialID)->GetName();
				std::string name = std::string(nameW.begin(), nameW.end());

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));


				ImGui::Button(name.c_str(), ImVec2(100, 60));
				ImGui::PopStyleColor();

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_Material"))
					{
						IM_ASSERT(payload->DataSize == sizeof(UINT));
						UINT payloaddata = *(UINT*)payload->Data;

						methodStruct->Set(ReflectionMaterialStruct(payloaddata));
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				ImGui::Text("material");
			}break;
			case TYPE_MODEL:
			{
				GetSetMethods<ModelReflectionStruct*>* methodStruct = ((GetSetMethods<ModelReflectionStruct*>*)method);
				ModelReflectionStruct* model = methodStruct->Get();
				ImGui::Button(model->modelName.c_str(), ImVec2(100, 60));

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_Model"))
					{
						IM_ASSERT(payload->DataSize == sizeof(ModelReflectionStruct*));
						ModelReflectionStruct* data = *(ModelReflectionStruct**)payload->Data;
						methodStruct->Set(data);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				ImGui::Text("Model");

			}break;
			case TYPE_PxGeometryType:
			{

				GetSetMethods<physx::PxGeometryType::Enum>* methodStruct = ((GetSetMethods<physx::PxGeometryType::Enum>*)method);
				physx::PxGeometryType::Enum type = methodStruct->Get();
				const char* items[] = { "sphere", "plane","capsule", "box", "convex mesh" };
				if (type > 5)
					continue;
				const char* item_current = items[type];
				ImGui::PushItemWidth(200);
				if (ImGui::BeginCombo(methodStruct->varName.c_str(), items[type])) // The second parameter is the label previewed before opening the combo.
				{
					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						bool is_selected = (item_current == items[n]);
						if (ImGui::Selectable(items[n], is_selected))
						{
							methodStruct->Set(static_cast<physx::PxGeometryType::Enum>(n));
						}
						if (is_selected)
							ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
					}

					ImGui::EndCombo();
				}

				ImGui::PopItemWidth();
				int breakp = 0;
			}break;
			case TYPE_XMFLOAT3:
			{
				GetSetMethods<DirectX::XMFLOAT3>* methodStruct = ((GetSetMethods<DirectX::XMFLOAT3>*)method);
				DirectX::XMFLOAT3 value = methodStruct->Get();
				ImGui::PushItemWidth(m_StandardItemWidth * 3);


				float valBefore[3]{ value.x,value.y,value.z }, valAfter[3]{ value.x,value.y,value.z };
				ImGui::DragFloat3(methodStruct->varName.c_str(), valAfter, 0.1f);

				if (valBefore[0] != valAfter[0] || valBefore[1] != valAfter[1] || valBefore[2] != valAfter[2])
				{
					methodStruct->Set({ valAfter[0],valAfter[1] ,valAfter[2] });
				}

				ImGui::PopItemWidth();
			} break;
			case TYPE_XMFLOAT4:
			{
				GetSetMethods<DirectX::XMFLOAT4>* methodStruct = ((GetSetMethods<DirectX::XMFLOAT4>*)method);
				DirectX::XMFLOAT4 value = methodStruct->Get();
				ImGui::PushItemWidth(m_StandardItemWidth * 3);


				float valBefore[4]{ value.x,value.y,value.z,value.w }, valAfter[4]{ value.x,value.y,value.z,value.w };
				ImGui::DragFloat4(methodStruct->varName.c_str(), valAfter, 0.1f);

				if (valBefore[0] != valAfter[0] || valBefore[1] != valAfter[1] || valBefore[2] != valAfter[2] || valBefore[3] != valAfter[3])
				{
					methodStruct->Set({ valAfter[0],valAfter[1] ,valAfter[2],valAfter[3] });
				}

				ImGui::PopItemWidth();
			}break;
			case TYPE_PXTRANSFORM:
			{
				GetSetMethods<physx::PxTransform>* methodStruct = ((GetSetMethods<physx::PxTransform>*)method);
				physx::PxTransform pose = methodStruct->Get(), poseorig = pose;
				ImGui::PushItemWidth(m_StandardItemWidth * 3);

				//position
				float position[3]{ pose.p.x,pose.p.y ,pose.p.z };
				ImGui::DragFloat3("Position", position);
				if (ImGui::IsItemClicked(1))
				{
					if (ImGui::GetItemRectMin().x <ImGui::GetMousePos().x && ImGui::GetItemRectMin().x + m_StandardItemWidth > ImGui::GetMousePos().x)
						position[0] = 0;
					if (ImGui::GetItemRectMin().x + m_StandardItemWidth <ImGui::GetMousePos().x && ImGui::GetItemRectMin().x + 2 * m_StandardItemWidth > ImGui::GetMousePos().x)
						position[1] = 0;
					if (ImGui::GetItemRectMin().x + 2 * m_StandardItemWidth <ImGui::GetMousePos().x && ImGui::GetItemRectMin().x + 3 * m_StandardItemWidth > ImGui::GetMousePos().x)
						position[2] = 0;
				}

				pose.p.x = position[0];
				pose.p.y = position[1];
				pose.p.z = position[2];

				//rotation

				float rotation[3], OrigRotation[3], dif[3];



				OrigRotation[0] = rotation[0] = DirectX::XMConvertToDegrees(atan2(2 * pose.q.x*pose.q.w + 2 * pose.q.y*pose.q.z, 1 - 2 * pose.q.x*pose.q.x - 2 * pose.q.z*pose.q.z));
				OrigRotation[1] = rotation[1] = DirectX::XMConvertToDegrees(atan2(2 * pose.q.y*pose.q.w + 2 * pose.q.x*pose.q.z, 1 - 2 * pose.q.y*pose.q.y - 2 * pose.q.z*pose.q.z));
				OrigRotation[2] = rotation[2] = DirectX::XMConvertToDegrees(asin(2 * pose.q.x*pose.q.y + 2 * pose.q.z*pose.q.w));

				ImGui::DragFloat3("Rotation", rotation);
				if (ImGui::IsItemClicked(1))
				{
					if (ImGui::GetItemRectMin().x <ImGui::GetMousePos().x && ImGui::GetItemRectMin().x + m_StandardItemWidth > ImGui::GetMousePos().x)
						rotation[0] = 0;
					if (ImGui::GetItemRectMin().x + m_StandardItemWidth <ImGui::GetMousePos().x && ImGui::GetItemRectMin().x + 2 * m_StandardItemWidth > ImGui::GetMousePos().x)
						rotation[1] = 0;
					if (ImGui::GetItemRectMin().x + 2 * m_StandardItemWidth <ImGui::GetMousePos().x && ImGui::GetItemRectMin().x + 3 * m_StandardItemWidth > ImGui::GetMousePos().x)
						rotation[2] = 0;
				}

				dif[0] = roundf((rotation[0] - OrigRotation[0]) * 10) / 10;
				dif[1] = roundf((rotation[1] - OrigRotation[1]) * 10) / 10;
				dif[2] = roundf((rotation[2] - OrigRotation[2]) * 10) / 10;

				bool difb = false;
				if (dif[0] || dif[1] || dif[2])
				{
					difb = true;
					DirectX::XMVECTOR res = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(dif[0]),
						DirectX::XMConvertToRadians(dif[1]), DirectX::XMConvertToRadians(dif[2]));
					DirectX::XMFLOAT4 dest;
					DirectX::XMStoreFloat4(&dest, res);
					pose.q *= ToPxQuat(dest);
				}
				if (pose.p != poseorig.p || difb)
					methodStruct->Set(pose);



				ImGui::PopItemWidth();

			} break;
			case TYPE_PXMATERIAL:
			{
				GetSetMethods<PhysicsMaterialStruct*>* methodStruct = ((GetSetMethods<PhysicsMaterialStruct*>*)method);
				PhysicsMaterialStruct* material = methodStruct->Get();

				std::string name(material->materialName.begin(), material->materialName.end());

				ImGui::Button(name.c_str(), ImVec2(100, 60));

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_PhysxMaterial"))
					{
						IM_ASSERT(payload->DataSize == sizeof(PhysicsMaterialStruct*));
						PhysicsMaterialStruct* data = *(PhysicsMaterialStruct**)payload->Data;
						methodStruct->Set(data);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				ImGui::Text(methodStruct->varName.c_str());
			} break;
			case TYPE_TEXTURE:
				{
				GetSetMethods<TextureStruct*>* methodStruct = ((GetSetMethods<TextureStruct*>*)method);
				TextureStruct* texture = methodStruct->Get();

			
				std::string name = "none";
					if(texture)
						name =texture->name;

				ImGui::Button(name.c_str(), ImVec2(100, 60));

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_Textures"))
					{
						IM_ASSERT(payload->DataSize == sizeof(TextureStruct*));
						TextureStruct* data = *(TextureStruct**)payload->Data;
						methodStruct->Set(data);
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::SameLine();
				ImGui::Text(methodStruct->varName.c_str());
				} break;
			case TYPE_ANIMATOR:
				{
					GetSetMethods<ModelAnimator*>* methodStruct = ((GetSetMethods<ModelAnimator*>*)method);
					ModelAnimator* anim = methodStruct->Get();
					if (!anim)
						continue;
					if (ImGui::TreeNode("animator"))
					{
						ExposeMethods(anim->GetRegisteredMethods(), gameContext);
						ImGui::TreePop();
					}


				}break;
			default:
			{}
			}
		}
	}

}

