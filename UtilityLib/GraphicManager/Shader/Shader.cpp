
#include "Shader.h"
#include "../../Loader/Loader.h"
#include "../../Singleton/Singleton.h"
#include "../../DeviceResources/DeviceResources.h"
#include "InputLayout.h"
#include "../../String/String.h"
#include "../../Archive/Archive.h"

std::string Utility::CShader::csoPath_ = "";
bool Utility::CShader::canUseArchive_ = false;

void Utility::CShader::CsoPath(std::string argPath)
{
	csoPath_ = argPath;
}

std::string Utility::CShader::CsoPath()
{
	return csoPath_;
}

void Utility::CShader::CanUseArchive(bool argCanUseArchive)
{
	canUseArchive_ = argCanUseArchive;
}


template<typename T>
HRESULT Utility::Shader<T>::Create(ID3D11DeviceContext * argContext, const char * argFilename)
{
	auto Fullpath = (csoPath_ + argFilename + ".cso");

	HRESULT hr = S_OK;


	if (canUseArchive_)
	{
		char* memory = nullptr;
		int size = 0;
		Singleton<Archive>::Get()->Read(Fullpath.c_str(), &memory, &size);
		hr = create_(argContext, memory, size);
	}
	else
	{
		Loader loader;
		bool isLoaded = loader.Load(Fullpath.c_str());
		assert(isLoaded && ".cso file Loading failed...");

		const char *Memory = loader.Data();
		const size_t Size = loader.Size();
		hr = create_(argContext, Memory, Size);

	}

	return hr;
}

template class Utility::Shader<ID3D11VertexShader>;
template class Utility::Shader<ID3D11PixelShader>;
template class Utility::Shader<ID3D11GeometryShader>;
template class Utility::Shader<ID3D11ComputeShader>;
template class Utility::Shader<ID3D11HullShader>;
