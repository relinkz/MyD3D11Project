#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;

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

	bool CreateSwapChainDeviceAndContext(HWND &hWnd);
	bool SetupRenderTargetView();
	bool SetupViewport();
};

#endif // !GRAPHICSENGINE_H
