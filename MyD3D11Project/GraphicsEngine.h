#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H
#include <D3D11.h>

class GraphicsEngine
{
public:
	virtual ~GraphicsEngine();

	void Init(HWND &hWnd);
	void Render();
protected:
	IDXGISwapChain*			m_sc = nullptr;
	ID3D11Device*			m_device = nullptr;
	ID3D11DeviceContext*	m_deviceContext = nullptr;
	ID3D11Texture2D*		pBackbuffer = nullptr;
	ID3D11RenderTargetView* m_rtw = nullptr;

	bool CreateSwapChainDeviceAndContext(HWND &hWnd);
	bool SetupRenderTargetView();
	bool SetupViewport();
};

#endif // !GRAPHICSENGINE_H
