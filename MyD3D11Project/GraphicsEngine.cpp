#include "stdafx.h"
#include "GraphicsEngine.h"
#include <D3D11.h>

GraphicsEngine::~GraphicsEngine()
{
}

void GraphicsEngine::Init(HWND & hWnd)
{
	m_sc			= nullptr;
	m_device		= nullptr;
	m_deviceContext = nullptr;
	m_backbuffer	= nullptr;
	m_rtw			= nullptr;

	CreateSwapChainDeviceAndContext(hWnd);

	SetupRenderTargetView();

	SetupViewport();
}

void GraphicsEngine::Render()
{
	float clearColor[4] = { 0.6f, 0.125f, 0.0f, 1.0f };  // RGBA
	m_deviceContext->ClearRenderTargetView(m_rtw, clearColor);

	m_sc->Present(0, 0);
}

bool GraphicsEngine::CreateSwapChainDeviceAndContext(HWND &hWnd)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = 640;
	swapChainDesc.BufferDesc.Height = 480;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	D3D_FEATURE_LEVEL requestedFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	UINT numLevelsRequested = 1;
	D3D_FEATURE_LEVEL featureLevelsSupported;


	HRESULT res = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&requestedFeatureLevel,
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_sc,
		&m_device,
		&featureLevelsSupported,
		&m_deviceContext);

	if (FAILED(res))
	{
		return FALSE;
	}

	res = m_sc->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_backbuffer);
	if (FAILED(res))
		return FALSE;

	return false;
}

bool GraphicsEngine::SetupRenderTargetView()
{
	HRESULT res = m_device->CreateRenderTargetView(m_backbuffer, NULL, &m_rtw);
	if (FAILED(res))
		return false;

	m_deviceContext->OMSetRenderTargets(1, &m_rtw, NULL);

	return true;
}

bool GraphicsEngine::SetupViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(640);
	vp.Height = static_cast<float>(480);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_deviceContext->RSSetViewports(1, &vp);

	return true;
}
