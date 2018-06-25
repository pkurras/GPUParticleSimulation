#pragma once

// EXTERNAL INCLUDES
#include <d3d11.h>
// INTERNAL INCLUDES
#include "math/vec2.h"
#include "types.h"
#include "utils.h"

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

class Renderer
{
public:

	/**
	 * @brief Construct a new Renderer object
	 */
	Renderer();
	/**
	 * @brief Destroy the Renderer object
	 */
	~Renderer();

	/**
	 * @brief	This method initializes the renderer
	 * 
	 * @param	handle is the main window handle to which the image will be sent
	 * @param	res is the resolution of the window - it is used for the buffer sizes
	 * @return	HRESULT is the resulting error code ('S_OK' means 'no error')
	 */
	HRESULT Initialize(uint64 handle, Math::Vec2 res);

	/**
	 * @brief	This method clears the backbuffer
	 * 			Call it before calling PresentFrame
	 */
	void ClearFrame(void);
	/**
	 * @brief	This method swaps the back and front buffer
	 */
	void PresentFrame(void);

	/**
	 * @brief 	This helper method generates a StructuredBuffer
	 * 			and attaches a ShaderResourceView and an UnorderedAccessView
	 * @tparam	T is the data type to be stored in the buffer
	 * @param	iNumElements is the maximum number of elements that will fit inside the buffer
	 * @param	ppBuffer is the returned buffer that is generated by the function
	 * @param	ppSRV is the attached ShaderResourceView
	 * @param	ppUAV is the attached UnorderedAccessView
	 * @param	pInitialData is the data that is stored inside the buffer from the beginning
	 * @return	HRESULT is the resulting error code ('S_OK' means 'no error')
	 */
	template <class T>
	HRESULT GenerateStructuredBuffer(UINT iNumElements, ID3D11Buffer** ppBuffer, ID3D11ShaderResourceView** ppSRV, ID3D11UnorderedAccessView** ppUAV, const T* pInitialData)
	{
		LOG("Generating StructuredBuffer (0x%p)", ppBuffer);

		HRESULT hr = S_OK;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = iNumElements * sizeof(T);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.StructureByteStride = sizeof(T);

		D3D11_SUBRESOURCE_DATA bufferInitData;
		ZeroMemory(&bufferInitData, sizeof(bufferInitData));
		bufferInitData.pSysMem = pInitialData;

		V_RETURN(this->device->CreateBuffer(&bufferDesc, (pInitialData) ? &bufferInitData : NULL, ppBuffer));

		V_RETURN(this->CreateShaderResourceView(ppBuffer, iNumElements, ppSRV));
		V_RETURN(this->CreateUnorderedAccessView(ppBuffer, iNumElements, ppUAV));

		return hr;
	}

	/**
	 * @brief 	This helper method generates an AppendStructuredBuffer
	 * 			and attaches a ShaderResourceView and an UnorderedAccessView
	 * @tparam	T is the data type to be stored in the buffer
	 * @param	iNumElements is the maximum number of elements that will fit inside the buffer
	 * @param	ppBuffer is the returned buffer that is generated by the function
	 * @param	ppSRV is the attached ShaderResourceView
	 * @param	ppUAV is the attached UnorderedAccessView
	 * @param	pInitialData is the data that is stored inside the buffer from the beginning
	 * @return	HRESULT is the resulting error code ('S_OK' means 'no error')
	 */
	template <class T>
	HRESULT GenerateAppendStructuredBuffer(UINT iNumElements, ID3D11Buffer** ppBuffer, ID3D11ShaderResourceView** ppSRV, ID3D11UnorderedAccessView** ppUAV, const T* pInitialData)
	{
		LOG("Generating StructuredBuffer (0x%p)", ppBuffer);

		HRESULT hr = S_OK;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.ByteWidth = iNumElements * sizeof(T);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.StructureByteStride = sizeof(T);

		D3D11_SUBRESOURCE_DATA bufferInitData;
		ZeroMemory(&bufferInitData, sizeof(bufferInitData));
		bufferInitData.pSysMem = pInitialData;

		V_RETURN(this->device->CreateBuffer(&bufferDesc, (pInitialData) ? &bufferInitData : NULL, ppBuffer));

		V_RETURN(this->CreateShaderResourceView(ppBuffer, iNumElements, ppSRV));

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(uavDesc));
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;
		uavDesc.Buffer.NumElements = iNumElements;

		V_RETURN(device->CreateUnorderedAccessView(*ppBuffer, &uavDesc, ppUAV));

		return hr;
	}

	/**
	 * @brief 	This method generates a ConstantBuffer
	 * @tparam	T is the data type to be stored in the buffer
	 * @param	ppBuffer is the returned buffer that is generated by the function
	 * @return	HRESULT is the resulting error code ('S_OK' means 'no error')
	 */
	template <class T>
	HRESULT GenerateConstantBuffer(ID3D11Buffer** ppBuffer)
	{
		LOG("Generating ConstantBuffer (0x%p)", ppBuffer);

		HRESULT hr = S_OK;

		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage			= D3D11_USAGE_DEFAULT;
		desc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags		= 0;
		desc.ByteWidth		= sizeof(T);

		V_RETURN(this->device->CreateBuffer(&desc, NULL, ppBuffer));

		return hr;
	}

	/**
	 * @brief	This method generates a DrawIndirectBuffer which is a modified
	 * 			structured buffer
	 * 
	 * @tparam	T is the data type to be stored in the buffer
	 * @param	ppBuffer is the returned buffer that is generated by the function
	 * @param	pInitialData is the data that is stored inside the buffer from the beginning
	 * @return	HRESULT is the resulting error code ('S_OK' means 'no error')
	 */
	template <class T>
	HRESULT GenerateIndirectDrawIndirectBuffer(ID3D11Buffer** ppBuffer, const T* pInitialData)
	{
		LOG("Generating DrawIndirectBuffer (0x%p)", ppBuffer);

		HRESULT hr = S_OK;

		// Create the draw indirect buffer
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.ByteWidth = sizeof(UINT) * 4;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData = { 0 };
		initData.pSysMem = pInitialData;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		V_RETURN(this->device->CreateBuffer(&bufferDesc, &initData, ppBuffer));

		return hr;
	}

	/**
	 * @brief	This method creates a ShaderResourceView
	 * @param	ppBuffer is the buffer on which the ShaderResoureView is observing
	 * @param	iNumElements is the maximum number of elements that are observed
	 * @param	ppSRV is the returned ShaderResoureView
	 * @return	HRESULT is the resulting error code ('S_OK' means 'no error')
	 */
	HRESULT CreateShaderResourceView(ID3D11Buffer** ppBuffer, UINT iNumElements, ID3D11ShaderResourceView** ppSRV);
	
	/**
	 * @brief	This method creates an UnorderedAccessView
	 * @param	ppBuffer is the buffer on which the UnorderedAccessView is observing
	 * @param	iNumElements is the maximum number of elements that are observed
	 * @param	ppUAV is the returned UnorderedAccessView
	 * @return	HRESULT is the resulting error code ('S_OK' means 'no error')
	 */
	HRESULT CreateUnorderedAccessView(ID3D11Buffer** ppBuffer, UINT iNumElements, ID3D11UnorderedAccessView** ppUAV);

	/**
	 * @brief 	This method compiles a shader from a specified filename
	 * 			It uses the directory of the main executable.
	 * 
	 * @param 	szFileName is the filename of the shader to be loaded
	 * @param	szEntryPoint is the entry point of the shader program
	 * @param	szShaderModel is the desired shader model version
	 * @param	ppBlobOut is the resulting shader binary blob
	 * @return	HRESULT is the resulting error code ('S_OK' means 'no error')
	 */
	HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

protected:

	uint64 handle;
	IDXGISwapChain* swapChain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* backbuffer;
	ID3D11DepthStencilView* depthBuffer;

};