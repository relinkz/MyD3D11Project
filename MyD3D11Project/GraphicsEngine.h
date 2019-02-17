#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include <DirectXMath.h>

// forward declarations
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D10Blob;

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
	ID3D11RenderTargetView* m_rtw;
	ID3D11Buffer*			m_vertexBuffer;
	ID3D11Buffer*			m_indexBuffer;
	ID3D11Buffer*			m_constantBuffer;


	ID3D10Blob*				m_VSBlob;
	ID3D10Blob*				m_PSBlob;

	ID3D11VertexShader*		m_vertexshader;
	ID3D11PixelShader*		m_pixelshader;

	UINT					m_nrIndices;

	// worldMatixes
	DirectX::XMMATRIX m_cube1;
	DirectX::XMMATRIX m_cube2;
	DirectX::XMMATRIX m_cube3;

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
	void updateConstantBuffers() const;

	void setupMatrixes();

	void createTriangle();

	void resetToNullptr();
};

#endif // !GRAPHICSENGINE_H
