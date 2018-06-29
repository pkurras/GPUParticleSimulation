// EXTERNAL INCLUDES
#include <cstdio>
#include <cwchar>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
// INTERNAL INCLUDES
#include "renderer.h"
#include "types.h"

#pragma warning(push)
#pragma warning(disable:4996)

constexpr float clearColor[4] = { 0.0f, 0.03f, 0.06f, 1.0f };

Renderer::Renderer() :
	pIDXGIFactory(nullptr),
	ppAdapters(nullptr),
	pIDXGISwapChain(nullptr),
	pDevice(nullptr),
	pContext(nullptr),
	pBackbuffer(nullptr),
	pDepthBuffer(nullptr)
{ }
Renderer::~Renderer()
{
	// Clean up all the renderer vars
	LOG("Removing IDXGIAdapters");
	free(this->ppAdapters);
	LOG("Removing IDXGISwapChain");
	SAFE_RELEASE(this->pIDXGISwapChain);
	LOG("Removing D3D11Device");
	SAFE_RELEASE(this->pDevice);
	LOG("Removing D3D11DeviceContext");
	SAFE_RELEASE(this->pContext);
	LOG("Removing Buffers");
	SAFE_RELEASE(this->pBackbuffer);
	SAFE_RELEASE(this->pDepthBuffer);
}

HRESULT Renderer::Initialize(uint64 handle, Math::Vec2 res)
{
	HRESULT hr;

	this->handle = handle;

	// Create a DXGIFactory Interface so that we can create a swapchain.
	V_RETURN(CreateDXGIFactory2(NULL, __uuidof(IDXGIFactory), (void **)&this->pIDXGIFactory));

	this->EnumerateAdapters();

	// Create the device and device context
	// Debug layer might be active for testing purpose
	// FIXME: Turn off the debug layer
	V_RETURN(D3D11CreateDevice(
		static_cast<IDXGIAdapter*>(this->ppAdapters[0]),
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		reinterpret_cast<ID3D11Device**>(&this->pDevice),
		NULL,
		reinterpret_cast<ID3D11DeviceContext**>(&this->pContext)
	));

	// Make some settings for the swap chain now
	DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 2;
	scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.SampleDesc.Count = 1;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsd = { 0 };
	fsd.RefreshRate = { 1, 60 };
	fsd.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_CENTERED;
	fsd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	fsd.Windowed = TRUE;

	LOG("Creating Swapchain for window (%" PRIu64 ")", this->handle);

	// Create the swap chain for the specified window
	V_RETURN(this->pIDXGIFactory->CreateSwapChainForHwnd(
		this->pDevice,
		reinterpret_cast<HWND>(this->handle),
		&scd,
		&fsd,
		nullptr,
		reinterpret_cast<IDXGISwapChain1**>(&this->pIDXGISwapChain))
	);

	// Enable or Disable DXGI to monitor the ALT+Enter mode switch
	// and react to it with fullscreen or windowed mode
	V_RETURN(this->pIDXGIFactory->MakeWindowAssociation(
		reinterpret_cast<HWND>(this->handle),
		DXGI_MWA_NO_ALT_ENTER)
	);

	// Release the factory now (no longer needed)
	SAFE_RELEASE(this->pIDXGIFactory);

	// Get the Backbuffer from the SwapChain and plug it into the CreateRenderTargetView
	// So that we can put our PixelShader results into it later on
	ID3D11Texture2D1 *pBackBuffer;
	V_RETURN(this->pIDXGISwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer)
	);
	V_RETURN(this->pDevice->CreateRenderTargetView(
		pBackBuffer,
		NULL,
		reinterpret_cast<ID3D11RenderTargetView**>(&this->pBackbuffer))
	);

	// Settings for the DepthBuffer
	// We pull the data from the Backbuffer so that we don't need to specify everything :)
	D3D11_TEXTURE2D_DESC1 depthBufferDesc;
	pBackBuffer->GetDesc1(&depthBufferDesc);
	depthBufferDesc.BindFlags	= D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.Format		= DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.Usage		= D3D11_USAGE_DEFAULT;

	// Release the Backbuffer pointer now
	pBackBuffer->Release();

	// Create the DepthStencilView now
	ID3D11Texture2D1 *pDepthBuffer;
	V_RETURN(this->pDevice->CreateTexture2D1(&depthBufferDesc, NULL, &pDepthBuffer));
	V_RETURN(this->pDevice->CreateDepthStencilView(pDepthBuffer, NULL, &this->pDepthBuffer));
	pDepthBuffer->Release();

	LOG("Setting Rendertargets");

	// Set the Backbuffer and the DepthBuffer as render targets
	// in the output merger stage of the render pipeline
	this->pContext->OMSetRenderTargets(
		1,
		reinterpret_cast<ID3D11RenderTargetView**>(&this->pBackbuffer),
		this->pDepthBuffer
	);

	// Specify the viewport
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX	= 0;
	viewport.TopLeftY	= 0;
	viewport.Width		= static_cast<float>(depthBufferDesc.Width);
	viewport.Height		= static_cast<float>(depthBufferDesc.Height);
	viewport.MinDepth	= 0.0f;
	viewport.MaxDepth	= 1.0f;

	LOG("Setting Viewport (%i x %i)",
		depthBufferDesc.Width,
		depthBufferDesc.Height
	);

	// Set the viewport to the rasterizer stage
	this->pContext->RSSetViewports(1, &viewport);

	return hr;
}

void Renderer::ClearFrame()
{
	// Clear the Backbuffer and the DepthBuffer
	this->pContext->ClearRenderTargetView(this->pBackbuffer, clearColor);
	this->pContext->ClearDepthStencilView(this->pDepthBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void Renderer::PresentFrame()
{
	// Swap the Backbuffer with the Frontbuffer
	this->pIDXGISwapChain->Present(1, 0);
}

HRESULT Renderer::CreateShaderResourceView(ID3D11Buffer** ppBuffer, UINT iNumElements, ID3D11ShaderResourceView** ppSRV)
{
	LOG("Creating ShaderResourceView (0x%p)", ppSRV);

	// Settings for the ShaderResourceView
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.ElementWidth = iNumElements;

	// Set the ShaderResourceView
	return pDevice->CreateShaderResourceView(*ppBuffer, &srvDesc, ppSRV);
}
HRESULT Renderer::CreateUnorderedAccessView(ID3D11Buffer** ppBuffer, UINT iNumElements, ID3D11UnorderedAccessView** ppUAV)
{
	LOG("Creating UnorderedAccessView (0x%p)", ppUAV);

	// Settings for the UnorderedAccessView
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));

	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = iNumElements;

	// Set the UnorderedAccessView
	return pDevice->CreateUnorderedAccessView(*ppBuffer, &uavDesc, ppUAV);
}

HRESULT Renderer::CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	LOG("Compiling shader (%s -> %s)", szFileName, szEntryPoint);

	if (!ppBlobOut)
		return E_INVALIDARG;

	// Get the current executable path
	char str[MAX_PATH] = { 0 };
	HRESULT hr = GetCurrentDirectoryA(sizeof(str), str);

	if (FAILED(hr))
		return hr;

	// read the string length of the path and the filename
	size_t pathLen = strlen(str);
	size_t fileNameLen = strlen(szFileName);

	// check if the path length + filename length + 2 extra symbols
	// is smaller than the MAX_PATH var
	if ((pathLen + fileNameLen + 2) < MAX_PATH)
	{
		// add backslash to the current path
		str[pathLen] = '\\';
		// copy the filename in
		memcpy(str + pathLen + 1, szFileName, fileNameLen);
		// set the string terminator at the end
		str[pathLen + fileNameLen + 1] = '\0';
	}

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows
	// the shaders to be optimized and to run exactly the way they will run in
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;

#if D3D_COMPILER_VERSION >= 46

	// convert the path to wide chars (because D3DCompileFromFile demands it)
	WCHAR path[MAX_PATH];
	mbstowcs(path, str, pathLen + fileNameLen + 2);

	hr = D3DCompileFromFile(path, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
#else
	hr = D3DX11CompileFromFile(str, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, nullptr, ppBlobOut, &pErrorBlob, nullptr);
#endif
	if (FAILED(hr))
	{
		if (pErrorBlob)
			LOG((char*)pErrorBlob->GetBufferPointer());

		SAFE_RELEASE(pErrorBlob);
		return hr;
	}
	SAFE_RELEASE(pErrorBlob);

	return S_OK;
}

void Renderer::EnumerateAdapters()
{
	size_t numAdapters = 0;
	// This interface sucks. We need to iterate once to find the number of adapters
	// because we use 'continuous memory' allocation here and thus need to make this
	// extra iteration. Thanks Microsoft.
	IDXGIAdapter4* pAdapter = nullptr;
	while (this->pIDXGIFactory->EnumAdapters1(numAdapters, reinterpret_cast<IDXGIAdapter1**>(&pAdapter)) != DXGI_ERROR_NOT_FOUND)
	{
		numAdapters++;
	}

	LOG("%i graphics adapters found.", numAdapters);

	// Allocate space for the adapters using typeless allocation via malloc since we can't
	// allocate space from an interface like IDXGIAdapter1 where there are pure virtuals.
	this->ppAdapters = static_cast<IDXGIAdapter3**>(malloc(sizeof(IDXGIAdapter1*) * numAdapters));

	numAdapters = 0;
	// Iterate again, this time put the adapters into the allocated array.
	while (this->pIDXGIFactory->EnumAdapters1(numAdapters, reinterpret_cast<IDXGIAdapter1**>(&pAdapter)) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC3 desc = { 0 };
		pAdapter->GetDesc3(&desc);

		char description[MAX_PATH];
		wcstombs(description, desc.Description, MAX_PATH);

		LOG("Adapter #%i: %s", numAdapters, description);

		this->ppAdapters[numAdapters] = pAdapter;
		numAdapters++;
	}

}

#pragma warning(pop)
