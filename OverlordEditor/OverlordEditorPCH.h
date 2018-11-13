#pragma once

#include <string>
#include <vector>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>

#include <d3dx11effect.h>
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "DxEffects11_vc14_Debug.lib")
#else 
#pragma comment(lib, "DxEffects11_vc14_Release.lib")
#endif


#pragma region
//*****************************************************************************
//Declare templates for releasing interfaces and deleting objects 
//*****************************************************************************
template<class Interface>
inline void SafeRelease(Interface &pInterfaceToRelease)
{
	if (pInterfaceToRelease != nullptr)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = nullptr;
	}
}

//*****************************************************************************
//create macro to autimaticaly make get set methods 
//*****************************************************************************
#define GETSET(type,name)\
	MAKE_GET_SET (type,name,name)

#define MAKE_GET_SET(Type, MemberName, FaceName) \
            Type Get##FaceName() const { \
                return MemberName; \
          }; \
          void Set##FaceName(Type value) { \
                MemberName = value; \
}



#pragma endregion Templates & Macros





#include <GeneralStructs.h>
#include <MathHelper.h>
#include <PhysxHelper.h>
#include <VertexHelper.h>
#include <Logger.h>