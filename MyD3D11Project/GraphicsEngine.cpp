#include "stdafx.h"
#include "GraphicsEngine.h"
#include <D3D11.h>
#include <DirectXMath.h>
#include <D3DX11async.h>

GraphicsEngine::~GraphicsEngine()
{
	if (m_sc)
	{
		m_sc->Release();
		m_sc = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}

	if (m_backbuffer)
	{
		m_backbuffer->Release();
		m_backbuffer = nullptr;
	}

	if (m_rtw)
	{
		m_rtw->Release();
		m_rtw = nullptr;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	if (m_VSBlob)
	{
		m_VSBlob->Release();
		m_VSBlob = nullptr;
	}

	if (m_PSBlob)
	{
		m_PSBlob->Release();
		m_PSBlob = nullptr;
	}

	if (m_vertexshader)
	{
		m_vertexshader->Release();
		m_vertexshader = nullptr;
	}

	if (m_pixelshader)
	{
		m_pixelshader->Release();
		m_pixelshader = nullptr;
	}
}

void GraphicsEngine::Init(HWND & hWnd)
{
	resetToNullptr();

	createSwapChainDeviceAndContext(hWnd);

	setupRenderTargetView();

	setupViewport();

	setupMatrixes();

	createTriangle();
}

void GraphicsEngine::Render()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };  // RGBA
	m_deviceContext->ClearRenderTargetView(m_rtw, clearColor);

	m_deviceContext->VSSetShader(m_vertexshader, NULL, 0);
	m_deviceContext->PSSetShader(m_pixelshader, NULL, 0);
	m_deviceContext->Draw(3, 0);

	m_sc->Present(0, 0);
}

bool GraphicsEngine::createSwapChainDeviceAndContext(HWND &hWnd)
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

bool GraphicsEngine::setupRenderTargetView()
{
	HRESULT res = m_device->CreateRenderTargetView(m_backbuffer, NULL, &m_rtw);
	if (FAILED(res))
		return false;

	m_deviceContext->OMSetRenderTargets(1, &m_rtw, NULL);

	return true;
}

bool GraphicsEngine::setupViewport()
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

HRESULT GraphicsEngine::setInputLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",						// SemanticName
			0,								// SemanticIndex
			DXGI_FORMAT_R32G32B32_FLOAT, // Format
			0,								// InputSlot
			0,								// AlignedByteOffset
			D3D11_INPUT_PER_VERTEX_DATA,	// InputSlotClass
			0								// InstanceDataStepRate
		}
	};

	UINT numElements = ARRAYSIZE(layout);

	ID3D11InputLayout* vertexLayout = nullptr;

	if (!m_VSBlob)
	{
		MessageBox(NULL,
			L"No vsBlob, did you forgot to create vsshader?", L"Error", MB_OK);
	}
		
	hr = m_device->CreateInputLayout(layout, numElements, m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), &vertexLayout);
	m_deviceContext->IASetInputLayout(vertexLayout);

	return hr;
}

HRESULT GraphicsEngine::createVertexShader()
{
	HRESULT hr = S_OK;

	ID3DBlob* pVSBlobErr = nullptr;

	hr = D3DX11CompileFromFile(L"VertexShader.hlsl", NULL, NULL, "main", "vs_4_0", NULL, (1 << 0), 0, &m_VSBlob, &pVSBlobErr, NULL);

	if (FAILED(hr))
	{
		if (!pVSBlobErr)
			OutputDebugStringA((char*)pVSBlobErr->GetBufferPointer());
		if (pVSBlobErr)
			pVSBlobErr->Release();
	}
	if (pVSBlobErr)
		pVSBlobErr->Release();

	hr = m_device->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), NULL, &m_vertexshader);

	if (FAILED(hr))
	{
		m_VSBlob->Release();
	}

	return hr;
}

HRESULT GraphicsEngine::createPixelShader()
{
	HRESULT hr = S_OK;

	ID3DBlob* pPSBlobErr = nullptr;

	hr = D3DX11CompileFromFile(L"PixelShader.hlsl", NULL, NULL, "main", "ps_4_0", NULL, (1 << 0), 0, &m_PSBlob, &pPSBlobErr, NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
	}

	hr = m_device->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(), NULL, &m_pixelshader);


	return hr;
}

HRESULT GraphicsEngine::createAndSetVertexBuffer()
{
	HRESULT hr = S_OK;

	struct SimpleVertex
	{
		DirectX::XMFLOAT3 pos;
	};

	SimpleVertex vertices[] =
	{
		DirectX::XMFLOAT3(0.0f, 0.5f, 0.5f),
		DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f),
		DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f),
	};


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = vertices;

	hr = m_device->CreateBuffer(&bd, &initData, &m_vertexBuffer);

	if (FAILED(hr))
	{
		OutputDebugStringA("TEST");
	}

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set primitive topology
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return hr;
}

void GraphicsEngine::setupMatrixes()
{
	// World matrix
	this->m_world = DirectX::XMMatrixIdentity();

	// View Matrix
	DirectX::XMVECTOR eye	= DirectX::XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	DirectX::XMVECTOR at	= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up	= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_view = DirectX::XMMatrixLookAtLH(eye, at, up);

	// projectionMatrix
	float aspectRatio = 640.0f / 480.0f;

	m_projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, aspectRatio, 0.01f, 100.0f );
}

void GraphicsEngine::createTriangle()
{
	createVertexShader();

	setInputLayout();

	createAndSetVertexBuffer();

	createPixelShader();
}

void GraphicsEngine::resetToNullptr()
{
	m_sc = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_backbuffer = nullptr;
	m_rtw = nullptr;
	m_vertexBuffer = nullptr;
	m_VSBlob = nullptr;
	m_PSBlob = nullptr;
}
