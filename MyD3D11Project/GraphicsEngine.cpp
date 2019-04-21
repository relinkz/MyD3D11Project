#include "stdafx.h"
#include "GraphicsEngine.h"
#include <D3DX11async.h>

static const float W_WIDTH = 1920.0f;
static const float W_HEIGHT = 1080.0f;

struct ConstantBuffer
{
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
};

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

	if (m_constantBuffer)
	{
		m_constantBuffer->Release();
		m_constantBuffer = nullptr;
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

	createVertexShader();

	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	createConstantBuffer();
	
	setInputLayout();

	createAndSetVertexBuffer();

	createPixelShader();

	m_deviceContext->VSSetShader(m_vertexshader, NULL, 0);
	m_deviceContext->PSSetShader(m_pixelshader, NULL, 0);

	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	m_orbitRot = 0.0f;
}

void GraphicsEngine::Render()
{
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };  // RGBA
	m_deviceContext->ClearRenderTargetView(m_rtw, clearColor);
	
	renderEntities();

	m_sc->Present(0, 0);
}

bool GraphicsEngine::createSwapChainDeviceAndContext(HWND &hWnd)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = static_cast<UINT>(W_WIDTH);
	swapChainDesc.BufferDesc.Height = static_cast<UINT>(W_HEIGHT);

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
	vp.Width = W_WIDTH;
	vp.Height = W_HEIGHT;
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

	//SemanticName, SemanticIndex, Format, Inputslot, AlignedByByteOffset, InputSlotClass, InstanceDataStepRate
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

	ID3D11Buffer*			m_vertexBuffer;

	VertexData test = m_entity.getVertexDescription();
	hr = m_device->CreateBuffer(&test.desc, &test.subdata, &m_vertexBuffer);

	if (FAILED(hr))
	{
		OutputDebugStringA("vertexbuffer creation failure");
	}

	UINT stride = Entity::sizeOfVertex();
	UINT offset = 0;

	m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	/*
	//index buffer
	ID3D11Buffer*			m_indexBuffer;
	test = m_entity.getIndexDescription();
	hr = m_device->CreateBuffer(&test.desc, &test.subdata, &m_indexBuffer);

	if (FAILED(hr))
	{
		OutputDebugStringA("IndexBuffer creation failure");
	}

	m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);


	*/
	return hr;
}

HRESULT GraphicsEngine::createConstantBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	hr = m_device->CreateBuffer(&bd, NULL, &m_constantBuffer);
	if (FAILED(hr))
	{
		OutputDebugStringA("ConstantBuffer creation failure");
	}

	return hr;
}

void GraphicsEngine::renderEntities()
{
	ConstantBuffer cb;
	cb.mView		= DirectX::XMMatrixTranspose(m_view);
	cb.mProjection	= DirectX::XMMatrixTranspose(m_projection);
	
	m_orbitRot += 0.0005;

	DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.0, 1.0, 1.0);
	DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0.0, m_orbitRot, 0.0);
	//DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(m_orbitRot, 0.0, 0.0);
	//DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(0.0, 0.0, -m_orbitRot);
	//DirectX::XMFLOAT3 rotation = DirectX::XMFLOAT3(m_orbitRot, m_orbitRot, 0.0);

	DirectX::XMFLOAT3 translate = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	cb.mWorld = DirectX::XMMatrixTranspose(m_entity.getTransform(scale, rotation, translate));

	m_deviceContext->UpdateSubresource(m_constantBuffer, 0, NULL, &cb, 0, 0);
	m_deviceContext->Draw(3 * m_entity.getNrOfVertex(), 0);

	//m_deviceContext->DrawIndexed(Entity::indexCount, 0, 0);
}

void GraphicsEngine::setupMatrixes()
{
	// View Matrix
	DirectX::XMVECTOR eye	= DirectX::XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	DirectX::XMVECTOR at	= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up	= DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_view = DirectX::XMMatrixLookAtLH(eye, at, up);

	// projectionMatrix
	float aspectRatio = W_WIDTH / W_HEIGHT;

	m_projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, aspectRatio, 0.01f, 100.0f );
}

void GraphicsEngine::resetToNullptr()
{
	m_sc = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_backbuffer = nullptr;
	m_rtw = nullptr;
	m_constantBuffer = nullptr;
	m_VSBlob = nullptr;
	m_PSBlob = nullptr;
}
