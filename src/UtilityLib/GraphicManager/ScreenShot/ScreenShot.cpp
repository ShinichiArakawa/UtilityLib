

#include "ScreenShot.h"
#include <Windows.h>
#include <d3d11.h>
#include <Shlwapi.h>
#include <assert.h>
#include <functional>
#include <wincodec.h>
#include <comdef.h>
#include "../TextureImage/TextureImage.h"

#if 0
bool g_WIC2 = false;

IWICImagingFactory* _GetWIC()
{
	static INIT_ONCE s_initOnce = INIT_ONCE_STATIC_INIT;

	IWICImagingFactory* factory = nullptr;
	InitOnceExecuteOnce(&s_initOnce,
		[](PINIT_ONCE, PVOID, LPVOID *factory) -> BOOL
	{
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8) || defined(_WIN7_PLATFORM_UPDATE)
		HRESULT hr = CoCreateInstance(
			CLSID_WICImagingFactory2,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IWICImagingFactory2),
			factory
		);

		if (SUCCEEDED(hr))
		{
			// WIC2 is available on Windows 10, Windows 8.x, and Windows 7 SP1 with KB 2670838 installed
			g_WIC2 = true;
			return TRUE;
		}
		else
		{
			hr = CoCreateInstance(
				CLSID_WICImagingFactory1,
				nullptr,
				CLSCTX_INPROC_SERVER,
				__uuidof(IWICImagingFactory),
				factory
			);
			return SUCCEEDED(hr) ? TRUE : FALSE;
		}
#else
		return SUCCEEDED(CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			__uuidof(IWICImagingFactory),
			factory)) ? TRUE : FALSE;
#endif
	}, nullptr, reinterpret_cast<LPVOID*>(&factory));

	return factory;
}

DXGI_FORMAT EnsureNotTypeless(DXGI_FORMAT fmt)
{
	// Assumes UNORM or FLOAT; doesn't use UINT or SINT
	switch (fmt)
	{
	case DXGI_FORMAT_R32G32B32A32_TYPELESS: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case DXGI_FORMAT_R32G32B32_TYPELESS:    return DXGI_FORMAT_R32G32B32_FLOAT;
	case DXGI_FORMAT_R16G16B16A16_TYPELESS: return DXGI_FORMAT_R16G16B16A16_UNORM;
	case DXGI_FORMAT_R32G32_TYPELESS:       return DXGI_FORMAT_R32G32_FLOAT;
	case DXGI_FORMAT_R10G10B10A2_TYPELESS:  return DXGI_FORMAT_R10G10B10A2_UNORM;
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:     return DXGI_FORMAT_R8G8B8A8_UNORM;
	case DXGI_FORMAT_R16G16_TYPELESS:       return DXGI_FORMAT_R16G16_UNORM;
	case DXGI_FORMAT_R32_TYPELESS:          return DXGI_FORMAT_R32_FLOAT;
	case DXGI_FORMAT_R8G8_TYPELESS:         return DXGI_FORMAT_R8G8_UNORM;
	case DXGI_FORMAT_R16_TYPELESS:          return DXGI_FORMAT_R16_UNORM;
	case DXGI_FORMAT_R8_TYPELESS:           return DXGI_FORMAT_R8_UNORM;
	case DXGI_FORMAT_BC1_TYPELESS:          return DXGI_FORMAT_BC1_UNORM;
	case DXGI_FORMAT_BC2_TYPELESS:          return DXGI_FORMAT_BC2_UNORM;
	case DXGI_FORMAT_BC3_TYPELESS:          return DXGI_FORMAT_BC3_UNORM;
	case DXGI_FORMAT_BC4_TYPELESS:          return DXGI_FORMAT_BC4_UNORM;
	case DXGI_FORMAT_BC5_TYPELESS:          return DXGI_FORMAT_BC5_UNORM;
	case DXGI_FORMAT_B8G8R8A8_TYPELESS:     return DXGI_FORMAT_B8G8R8A8_UNORM;
	case DXGI_FORMAT_B8G8R8X8_TYPELESS:     return DXGI_FORMAT_B8G8R8X8_UNORM;
	case DXGI_FORMAT_BC7_TYPELESS:          return DXGI_FORMAT_BC7_UNORM;
	default:                                return fmt;
	}
}

HRESULT CaptureTexture(_In_ ID3D11DeviceContext* pContext,
	_In_ ID3D11Resource* pSource,
	D3D11_TEXTURE2D_DESC& desc,
	Microsoft::WRL::ComPtr<ID3D11Texture2D>& pStaging)
{
	if (!pContext || !pSource)
		return E_INVALIDARG;

	D3D11_RESOURCE_DIMENSION resType = D3D11_RESOURCE_DIMENSION_UNKNOWN;
	pSource->GetType(&resType);

	if (resType != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
		return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	HRESULT hr = pSource->QueryInterface(pTexture.GetAddressOf());
	if (FAILED(hr))
		return hr;

	assert(pTexture);

	pTexture->GetDesc(&desc);

	Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
	pContext->GetDevice(d3dDevice.GetAddressOf());

	if (desc.SampleDesc.Count > 1)
	{
		// MSAA content must be resolved before being copied to a staging texture
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTemp;
		hr = d3dDevice->CreateTexture2D(&desc, 0, pTemp.GetAddressOf());
		if (FAILED(hr))
			return hr;

		assert(pTemp);

		DXGI_FORMAT fmt = EnsureNotTypeless(desc.Format);

		UINT support = 0;
		hr = d3dDevice->CheckFormatSupport(fmt, &support);
		if (FAILED(hr))
			return hr;

		if (!(support & D3D11_FORMAT_SUPPORT_MULTISAMPLE_RESOLVE))
			return E_FAIL;

		for (UINT item = 0; item < desc.ArraySize; ++item)
		{
			for (UINT level = 0; level < desc.MipLevels; ++level)
			{
				UINT index = D3D11CalcSubresource(level, item, desc.MipLevels);
				pContext->ResolveSubresource(pTemp.Get(), index, pSource, index, fmt);
			}
		}

		desc.BindFlags = 0;
		desc.MiscFlags &= D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.Usage = D3D11_USAGE_STAGING;

		hr = d3dDevice->CreateTexture2D(&desc, 0, pStaging.ReleaseAndGetAddressOf());
		if (FAILED(hr))
			return hr;

		assert(pStaging);

		pContext->CopyResource(pStaging.Get(), pTemp.Get());
	}
	else if ((desc.Usage == D3D11_USAGE_STAGING) && (desc.CPUAccessFlags & D3D11_CPU_ACCESS_READ))
	{
		// Handle case where the source is already a staging texture we can use directly
		pStaging = pTexture;
	}
	else
	{
		// Otherwise, create a staging texture from the non-MSAA source
		desc.BindFlags = 0;
		desc.MiscFlags &= D3D11_RESOURCE_MISC_TEXTURECUBE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.Usage = D3D11_USAGE_STAGING;

		hr = d3dDevice->CreateTexture2D(&desc, 0, pStaging.ReleaseAndGetAddressOf());
		if (FAILED(hr))
			return hr;

		assert(pStaging);

		pContext->CopyResource(pStaging.Get(), pSource);
	}

	return S_OK;
}

HRESULT SaveWICTextureToFile(ID3D11DeviceContext* pContext, ID3D11Resource* pSource,
	REFGUID guidContainerFormat, const wchar_t* fileName,
	const GUID* targetFormat, std::function<void(IPropertyBag2*)> setCustomProps = nullptr)
{
	if (!fileName)
		return E_INVALIDARG;

	D3D11_TEXTURE2D_DESC desc = {};
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pStaging;
	HRESULT hr = CaptureTexture(pContext, pSource, desc, pStaging);
	if (FAILED(hr))
		return hr;

	// Determine source format's WIC equivalent
	WICPixelFormatGUID pfGuid;
	bool sRGB = false;
	switch (desc.Format)
	{
	case DXGI_FORMAT_R32G32B32A32_FLOAT:            pfGuid = GUID_WICPixelFormat128bppRGBAFloat; break;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:            pfGuid = GUID_WICPixelFormat64bppRGBAHalf; break;
	case DXGI_FORMAT_R16G16B16A16_UNORM:            pfGuid = GUID_WICPixelFormat64bppRGBA; break;
	case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:    pfGuid = GUID_WICPixelFormat32bppRGBA1010102XR; break; // DXGI 1.1
	case DXGI_FORMAT_R10G10B10A2_UNORM:             pfGuid = GUID_WICPixelFormat32bppRGBA1010102; break;
	case DXGI_FORMAT_B5G5R5A1_UNORM:                pfGuid = GUID_WICPixelFormat16bppBGRA5551; break;
	case DXGI_FORMAT_B5G6R5_UNORM:                  pfGuid = GUID_WICPixelFormat16bppBGR565; break;
	case DXGI_FORMAT_R32_FLOAT:                     pfGuid = GUID_WICPixelFormat32bppGrayFloat; break;
	case DXGI_FORMAT_R16_FLOAT:                     pfGuid = GUID_WICPixelFormat16bppGrayHalf; break;
	case DXGI_FORMAT_R16_UNORM:                     pfGuid = GUID_WICPixelFormat16bppGray; break;
	case DXGI_FORMAT_R8_UNORM:                      pfGuid = GUID_WICPixelFormat8bppGray; break;
	case DXGI_FORMAT_A8_UNORM:                      pfGuid = GUID_WICPixelFormat8bppAlpha; break;

	case DXGI_FORMAT_R8G8B8A8_UNORM:
		pfGuid = GUID_WICPixelFormat32bppRGBA;
		break;

	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		pfGuid = GUID_WICPixelFormat32bppRGBA;
		sRGB = true;
		break;

	case DXGI_FORMAT_B8G8R8A8_UNORM: // DXGI 1.1
		pfGuid = GUID_WICPixelFormat32bppBGRA;
		break;

	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: // DXGI 1.1
		pfGuid = GUID_WICPixelFormat32bppBGRA;
		sRGB = true;
		break;

	case DXGI_FORMAT_B8G8R8X8_UNORM: // DXGI 1.1
		pfGuid = GUID_WICPixelFormat32bppBGR;
		break;

	case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: // DXGI 1.1
		pfGuid = GUID_WICPixelFormat32bppBGR;
		sRGB = true;
		break;

	default:
		return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
	}

	IWICImagingFactory* pWIC = _GetWIC();
	if (!pWIC)
		return E_NOINTERFACE;

	Microsoft::WRL::ComPtr<IWICStream> stream;
	hr = pWIC->CreateStream(stream.GetAddressOf());
	if (FAILED(hr))
		return hr;

	hr = stream->InitializeFromFilename(fileName, GENERIC_WRITE);
	if (FAILED(hr))
		return hr;

	AutoDeleteFileWic delonfail(stream, fileName);

	Microsoft::WRL::ComPtr<IWICBitmapEncoder> encoder;
	hr = pWIC->CreateEncoder(guidContainerFormat, 0, encoder.GetAddressOf());
	if (FAILED(hr))
		return hr;

	hr = encoder->Initialize(stream.Get(), WICBitmapEncoderNoCache);
	if (FAILED(hr))
		return hr;

	Microsoft::WRL::ComPtr<IWICBitmapFrameEncode> frame;
	Microsoft::WRL::ComPtr<IPropertyBag2> props;
	hr = encoder->CreateNewFrame(frame.GetAddressOf(), props.GetAddressOf());
	if (FAILED(hr))
		return hr;

	if (targetFormat && memcmp(&guidContainerFormat, &GUID_ContainerFormatBmp, sizeof(WICPixelFormatGUID)) == 0 && g_WIC2)
	{
		// Opt-in to the WIC2 support for writing 32-bit Windows BMP files with an alpha channel
		PROPBAG2 option = {};
		option.pstrName = const_cast<wchar_t*>(L"EnableV5Header32bppBGRA");

		VARIANT varValue;
		varValue.vt = VT_BOOL;
		varValue.boolVal = VARIANT_TRUE;
		(void)props->Write(1, &option, &varValue);
	}

	if (setCustomProps)
	{
		setCustomProps(props.Get());
	}

	hr = frame->Initialize(props.Get());
	if (FAILED(hr))
		return hr;

	hr = frame->SetSize(desc.Width, desc.Height);
	if (FAILED(hr))
		return hr;

	hr = frame->SetResolution(72, 72);
	if (FAILED(hr))
		return hr;

	// Pick a target format
	WICPixelFormatGUID targetGuid;
	if (targetFormat)
	{
		targetGuid = *targetFormat;
	}
	else
	{
		// Screenshots dont typically include the alpha channel of the render target
		switch (desc.Format)
		{
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8) || defined(_WIN7_PLATFORM_UPDATE)
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			if (g_WIC2)
			{
				targetGuid = GUID_WICPixelFormat96bppRGBFloat;
			}
			else
			{
				targetGuid = GUID_WICPixelFormat24bppBGR;
			}
			break;
#endif

		case DXGI_FORMAT_R16G16B16A16_UNORM: targetGuid = GUID_WICPixelFormat48bppBGR; break;
		case DXGI_FORMAT_B5G5R5A1_UNORM:     targetGuid = GUID_WICPixelFormat16bppBGR555; break;
		case DXGI_FORMAT_B5G6R5_UNORM:       targetGuid = GUID_WICPixelFormat16bppBGR565; break;

		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R16_FLOAT:
		case DXGI_FORMAT_R16_UNORM:
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_A8_UNORM:
			targetGuid = GUID_WICPixelFormat8bppGray;
			break;

		default:
			targetGuid = GUID_WICPixelFormat24bppBGR;
			break;
		}
	}

	hr = frame->SetPixelFormat(&targetGuid);
	if (FAILED(hr))
		return hr;

	if (targetFormat && memcmp(targetFormat, &targetGuid, sizeof(WICPixelFormatGUID)) != 0)
	{
		// Requested output pixel format is not supported by the WIC codec
		return E_FAIL;
	}

	// Encode WIC metadata
	Microsoft::WRL::ComPtr<IWICMetadataQueryWriter> metawriter;
	if (SUCCEEDED(frame->GetMetadataQueryWriter(metawriter.GetAddressOf())))
	{
		PROPVARIANT value;
		PropVariantInit(&value);

		value.vt = VT_LPSTR;
		value.pszVal = const_cast<char*>("DirectXTK");

		if (memcmp(&guidContainerFormat, &GUID_ContainerFormatPng, sizeof(GUID)) == 0)
		{
			// Set Software name
			(void)metawriter->SetMetadataByName(L"/tEXt/{str=Software}", &value);

			// Set sRGB chunk
			if (sRGB)
			{
				value.vt = VT_UI1;
				value.bVal = 0;
				(void)metawriter->SetMetadataByName(L"/sRGB/RenderingIntent", &value);
			}
			else
			{
				// add gAMA chunk with gamma 1.0
				value.vt = VT_UI4;
				value.uintVal = 100000; // gama value * 100,000 -- i.e. gamma 1.0
				(void)metawriter->SetMetadataByName(L"/gAMA/ImageGamma", &value);

				// remove sRGB chunk which is added by default.
				(void)metawriter->RemoveMetadataByName(L"/sRGB/RenderingIntent");
			}
		}
		else
		{
			// Set Software name
			(void)metawriter->SetMetadataByName(L"System.ApplicationName", &value);

			if (sRGB)
			{
				// Set EXIF Colorspace of sRGB
				value.vt = VT_UI2;
				value.uiVal = 1;
				(void)metawriter->SetMetadataByName(L"System.Image.ColorSpace", &value);
			}
		}
	}

	D3D11_MAPPED_SUBRESOURCE mapped;
	hr = pContext->Map(pStaging.Get(), 0, D3D11_MAP_READ, 0, &mapped);
	if (FAILED(hr))
		return hr;

	if (memcmp(&targetGuid, &pfGuid, sizeof(WICPixelFormatGUID)) != 0)
	{
		// Conversion required to write
		Microsoft::WRL::ComPtr<IWICBitmap> source;
		hr = pWIC->CreateBitmapFromMemory(desc.Width, desc.Height, pfGuid,
			mapped.RowPitch, mapped.RowPitch * desc.Height,
			reinterpret_cast<BYTE*>(mapped.pData), source.GetAddressOf());
		if (FAILED(hr))
		{
			pContext->Unmap(pStaging.Get(), 0);
			return hr;
		}

		Microsoft::WRL::ComPtr<IWICFormatConverter> FC;
		hr = pWIC->CreateFormatConverter(FC.GetAddressOf());
		if (FAILED(hr))
		{
			pContext->Unmap(pStaging.Get(), 0);
			return hr;
		}

		BOOL canConvert = FALSE;
		hr = FC->CanConvert(pfGuid, targetGuid, &canConvert);
		if (FAILED(hr) || !canConvert)
		{
			return E_UNEXPECTED;
		}

		hr = FC->Initialize(source.Get(), targetGuid, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeMedianCut);
		if (FAILED(hr))
		{
			pContext->Unmap(pStaging.Get(), 0);
			return hr;
		}

		WICRect rect = { 0, 0, static_cast<INT>(desc.Width), static_cast<INT>(desc.Height) };
		hr = frame->WriteSource(FC.Get(), &rect);
		if (FAILED(hr))
		{
			pContext->Unmap(pStaging.Get(), 0);
			return hr;
		}
	}
	else
	{
		// No conversion required
		hr = frame->WritePixels(desc.Height, mapped.RowPitch, mapped.RowPitch * desc.Height, reinterpret_cast<BYTE*>(mapped.pData));
		if (FAILED(hr))
			return hr;
	}

	pContext->Unmap(pStaging.Get(), 0);

	hr = frame->Commit();
	if (FAILED(hr))
		return hr;

	hr = encoder->Commit();
	if (FAILED(hr))
		return hr;

	delonfail.clear();

	return S_OK;
}

HRESULT ScreenShot(ID3D11DeviceContext *argContext, IDXGISwapChain *argSwapChain)
{
	/*Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	HRESULT hr = argSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID)&backBuffer);
	if (SUCCEEDED(hr))
		hr = SaveWICTextureToFile(argContext, backBuffer.Get(), GUID_ContainerFormatBmp, L"SCREENSHOT.BMP");
*/
	return S_OK;
}

void OutputDX11(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDC, IDXGISwapChain* pSwapChain, ID3D11RenderTargetView* pRenderTargetView)
{
	//(1)DirectX11でレンダリングする関数.フリップはしない//
	//RenderingDX11();


	//(2)バックバッファのフォーマットを取得//
	ID3D11Texture2D* pBackBuffer = NULL;
	HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) return;

	D3D11_TEXTURE2D_DESC descBackBuffer;
	pBackBuffer->GetDesc(&descBackBuffer);
	pBackBuffer->Release();


	//(3)CPU読み出し可能なバッファをGPU上に作成//
	D3D11_TEXTURE2D_DESC Texture2DDesc;
	Texture2DDesc.ArraySize = 1;
	Texture2DDesc.BindFlags = 0;
	Texture2DDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	Texture2DDesc.Format = descBackBuffer.Format;
	Texture2DDesc.Height = descBackBuffer.Height;
	Texture2DDesc.Width = descBackBuffer.Width;
	Texture2DDesc.MipLevels = 1;
	Texture2DDesc.MiscFlags = 0;
	Texture2DDesc.SampleDesc.Count = 1;
	Texture2DDesc.SampleDesc.Quality = 0;
	Texture2DDesc.Usage = D3D11_USAGE_STAGING;

	ID3D11Texture2D *hCaptureTexture;
	pd3dDevice->CreateTexture2D(&Texture2DDesc, 0, &hCaptureTexture);


	//(4)作成したCPU読み込み可能バッファにGPU上でデータをコピー//
	ID3D11Resource *hResource;
	pRenderTargetView->GetResource(&hResource);
	pd3dDC->CopyResource(hCaptureTexture, hResource);
	hResource->Release();

	//(5)GPU上の読み込み可能バッファのメモリアドレスのマップを開く//
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	pd3dDC->Map(hCaptureTexture, 0, D3D11_MAP_READ, 0, &mappedResource);

	//(6)CPU上のメモリにバッファを確保//
	double width = descBackBuffer.Width;
	double height = descBackBuffer.Height;
	double src_stride = mappedResource.RowPitch;    //(注)descBackBuffer.Width * 4とは必ずしも一致しない
	size_t buffer_size = src_stride * height;
	BYTE *bmp_buffer = new BYTE[buffer_size];

	//(7)GPU上の読み込み可能バッファからCPU上のバッファへ転送//
	CopyMemory(bmp_buffer, mappedResource.pData, buffer_size);
	pd3dDC->Unmap(hCaptureTexture, 0);
	hCaptureTexture->Release();

	//(8)CPU上のバッファの画像をファイル書き出し//
	auto image = std::make_unique<Utility::Image>(DirectX::XMFLOAT2(width, height), src_stride);    //src_strideを考慮して画像を出力すべし
	auto tex = std::make_shared<Utility::Texture>(std::move(image), pd3dDevice);
		
	delete[] bmp_buffer;

}
#endif
