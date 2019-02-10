#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H



class GraphicsEngine
{
public:
	virtual ~GraphicsEngine();

	void Init(HWND &hWnd);
	void render();
protected:
	bool CreateSwapChainDeviceAndContext();
	bool SetupRenderTargetView();
	bool SetupViewport();
};

#endif // !GRAPHICSENGINE_H
