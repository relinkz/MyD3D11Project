#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include "Entity.h"
#include "Camera.h"

class GraphicsEngine
{
public:
	virtual ~GraphicsEngine();

	void Init(HWND &hWnd);
	void Render();
protected:
	IDXGISwapChain*			m_sc;
	ID3D11Device*			m_device;
	ID3D11DeviceContext*	m_deviceContext;
	ID3D11Texture2D*		m_backbuffer;
	ID3D11Texture2D*		m_depthStencil;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RenderTargetView* m_rtw;
	ID3D11Buffer*			m_constantBuffer;

	ID3D10Blob*				m_VSBlob;
	ID3D10Blob*				m_PSBlob;

	ID3D11VertexShader*		m_vertexshader;
	ID3D11PixelShader*		m_pixelshader;

	// worldMatixes, temp solution
	Entity m_entity;

	Camera m_cam;
	float m_orbitRot;
	
	DirectX::XMMATRIX m_view;
	DirectX::XMMATRIX m_projection;

	bool createSwapChainDeviceAndContext(HWND &hWnd);
	bool setupRenderTargetView();
	bool setupViewport();

	HRESULT setInputLayout();

	// shaders
	HRESULT createVertexShader();
	HRESULT createPixelShader();

	// buffers
	HRESULT createAndSetVertexBuffer();
	HRESULT createConstantBuffer();
	HRESULT createDepthBuffer();

	// helpers
	void renderEntities();
	void setupMatrixes();

	void resetToNullptr();
};

#endif // !GRAPHICSENGINE_H
