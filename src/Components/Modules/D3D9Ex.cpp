#include "STDInclude.hpp"

namespace Components
{
#pragma region D3D9Device

	HRESULT D3D9Ex::D3D9Device::QueryInterface(REFIID riid, void** ppvObj)
	{
		*ppvObj = nullptr;

		HRESULT hRes = m_pIDirect3DDevice9->QueryInterface(riid, ppvObj);
		if (hRes == NOERROR) *ppvObj = this;
		return hRes;
	}

	ULONG D3D9Ex::D3D9Device::AddRef()
	{
		return m_pIDirect3DDevice9->AddRef();
	}

	ULONG D3D9Ex::D3D9Device::Release()
	{
		ULONG count = m_pIDirect3DDevice9->Release();
		if (!count) delete this;
		return count;
	}

	HRESULT D3D9Ex::D3D9Device::TestCooperativeLevel()
	{
		return m_pIDirect3DDevice9->TestCooperativeLevel();
	}

	UINT D3D9Ex::D3D9Device::GetAvailableTextureMem()
	{
		return m_pIDirect3DDevice9->GetAvailableTextureMem();
	}

	HRESULT D3D9Ex::D3D9Device::EvictManagedResources()
	{
		return m_pIDirect3DDevice9->EvictManagedResources();
	}

	HRESULT D3D9Ex::D3D9Device::GetDirect3D(IDirect3D9** ppD3D9)
	{
		return m_pIDirect3DDevice9->GetDirect3D(ppD3D9);
	}

	HRESULT D3D9Ex::D3D9Device::GetDeviceCaps(D3DCAPS9* pCaps)
	{
		return m_pIDirect3DDevice9->GetDeviceCaps(pCaps);
	}

	HRESULT D3D9Ex::D3D9Device::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
	{
		return m_pIDirect3DDevice9->GetDisplayMode(iSwapChain, pMode);
	}

	HRESULT D3D9Ex::D3D9Device::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
	{
		return m_pIDirect3DDevice9->GetCreationParameters(pParameters);
	}

	HRESULT D3D9Ex::D3D9Device::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
	{
		return m_pIDirect3DDevice9->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
	}

	void D3D9Ex::D3D9Device::SetCursorPosition(int X, int Y, DWORD Flags)
	{
		return m_pIDirect3DDevice9->SetCursorPosition(X, Y, Flags);
	}

	BOOL D3D9Ex::D3D9Device::ShowCursor(BOOL bShow)
	{
		return m_pIDirect3DDevice9->ShowCursor(bShow);
	}

	HRESULT D3D9Ex::D3D9Device::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
	{
		return m_pIDirect3DDevice9->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
	}

	HRESULT D3D9Ex::D3D9Device::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
	{
		return m_pIDirect3DDevice9->GetSwapChain(iSwapChain, pSwapChain);
	}

	UINT D3D9Ex::D3D9Device::GetNumberOfSwapChains()
	{
		return m_pIDirect3DDevice9->GetNumberOfSwapChains();
	}

	HRESULT D3D9Ex::D3D9Device::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
	{
		return m_pIDirect3DDevice9->Reset(pPresentationParameters);
	}

	HRESULT D3D9Ex::D3D9Device::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
	{
		return m_pIDirect3DDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}

	HRESULT D3D9Ex::D3D9Device::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
	{
		return m_pIDirect3DDevice9->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
	}

	HRESULT D3D9Ex::D3D9Device::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
	{
		return m_pIDirect3DDevice9->GetRasterStatus(iSwapChain, pRasterStatus);
	}

	HRESULT D3D9Ex::D3D9Device::SetDialogBoxMode(BOOL bEnableDialogs)
	{
		return m_pIDirect3DDevice9->SetDialogBoxMode(bEnableDialogs);
	}

	void D3D9Ex::D3D9Device::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
	{
		return m_pIDirect3DDevice9->SetGammaRamp(iSwapChain, Flags, pRamp);
	}

	void D3D9Ex::D3D9Device::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp)
	{
		return m_pIDirect3DDevice9->GetGammaRamp(iSwapChain, pRamp);
	}

	HRESULT D3D9Ex::D3D9Device::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
	{
		if (Pool == D3DPOOL_MANAGED) { Pool = D3DPOOL_DEFAULT; Usage |= D3DUSAGE_DYNAMIC; }

		return m_pIDirect3DDevice9->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
	}

	HRESULT D3D9Ex::D3D9Device::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
	{
		if (Pool == D3DPOOL_MANAGED) { Pool = D3DPOOL_DEFAULT; Usage |= D3DUSAGE_DYNAMIC; }

		return m_pIDirect3DDevice9->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
	}

	HRESULT D3D9Ex::D3D9Device::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
	{
		if (Pool == D3DPOOL_MANAGED) { Pool = D3DPOOL_DEFAULT; Usage |= D3DUSAGE_DYNAMIC; }

		return m_pIDirect3DDevice9->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
	}

	HRESULT D3D9Ex::D3D9Device::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
	{
		if (Pool == D3DPOOL_MANAGED) { Pool = D3DPOOL_DEFAULT; Usage |= D3DUSAGE_DYNAMIC; }

		return m_pIDirect3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
	}

	HRESULT D3D9Ex::D3D9Device::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
	{
		if (Pool == D3DPOOL_MANAGED) { Pool = D3DPOOL_DEFAULT; Usage |= D3DUSAGE_DYNAMIC; }

		return m_pIDirect3DDevice9->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
	}

	HRESULT D3D9Ex::D3D9Device::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
	{
		return m_pIDirect3DDevice9->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
	}

	HRESULT D3D9Ex::D3D9Device::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
	{
		return m_pIDirect3DDevice9->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
	}

	HRESULT D3D9Ex::D3D9Device::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
	{
		return m_pIDirect3DDevice9->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
	}

	HRESULT D3D9Ex::D3D9Device::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
	{
		return m_pIDirect3DDevice9->UpdateTexture(pSourceTexture, pDestinationTexture);
	}

	HRESULT D3D9Ex::D3D9Device::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
	{
		return m_pIDirect3DDevice9->GetRenderTargetData(pRenderTarget, pDestSurface);
	}

	HRESULT D3D9Ex::D3D9Device::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
	{
		return m_pIDirect3DDevice9->GetFrontBufferData(iSwapChain, pDestSurface);
	}

	HRESULT D3D9Ex::D3D9Device::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
	{
		return m_pIDirect3DDevice9->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
	}

	HRESULT D3D9Ex::D3D9Device::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
	{
		return m_pIDirect3DDevice9->ColorFill(pSurface, pRect, color);
	}

	HRESULT D3D9Ex::D3D9Device::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
	{
		if (Pool == D3DPOOL_MANAGED) { Pool = D3DPOOL_DEFAULT; }

		return m_pIDirect3DDevice9->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
	}

	HRESULT D3D9Ex::D3D9Device::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
	{
		return m_pIDirect3DDevice9->SetRenderTarget(RenderTargetIndex, pRenderTarget);
	}

	HRESULT D3D9Ex::D3D9Device::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
	{
		return m_pIDirect3DDevice9->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
	}

	HRESULT D3D9Ex::D3D9Device::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
	{
		return m_pIDirect3DDevice9->SetDepthStencilSurface(pNewZStencil);
	}

	HRESULT D3D9Ex::D3D9Device::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
	{
		return m_pIDirect3DDevice9->GetDepthStencilSurface(ppZStencilSurface);
	}

	HRESULT D3D9Ex::D3D9Device::BeginScene()
	{
		return m_pIDirect3DDevice9->BeginScene();
	}

	HRESULT D3D9Ex::D3D9Device::EndScene()
	{
		return m_pIDirect3DDevice9->EndScene();
	}

	HRESULT D3D9Ex::D3D9Device::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
	{
		return m_pIDirect3DDevice9->Clear(Count, pRects, Flags, Color, Z, Stencil);
	}

	HRESULT D3D9Ex::D3D9Device::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
	{
		return m_pIDirect3DDevice9->SetTransform(State, pMatrix);
	}

	HRESULT D3D9Ex::D3D9Device::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
	{
		return m_pIDirect3DDevice9->GetTransform(State, pMatrix);
	}

	HRESULT D3D9Ex::D3D9Device::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
	{
		return m_pIDirect3DDevice9->MultiplyTransform(State, pMatrix);
	}

	HRESULT D3D9Ex::D3D9Device::SetViewport(CONST D3DVIEWPORT9* pViewport)
	{
		return m_pIDirect3DDevice9->SetViewport(pViewport);
	}

	HRESULT D3D9Ex::D3D9Device::GetViewport(D3DVIEWPORT9* pViewport)
	{
		return m_pIDirect3DDevice9->GetViewport(pViewport);
	}

	HRESULT D3D9Ex::D3D9Device::SetMaterial(CONST D3DMATERIAL9* pMaterial)
	{
		return m_pIDirect3DDevice9->SetMaterial(pMaterial);
	}

	HRESULT D3D9Ex::D3D9Device::GetMaterial(D3DMATERIAL9* pMaterial)
	{
		return m_pIDirect3DDevice9->GetMaterial(pMaterial);
	}

	HRESULT D3D9Ex::D3D9Device::SetLight(DWORD Index, CONST D3DLIGHT9* pLight)
	{
		return m_pIDirect3DDevice9->SetLight(Index, pLight);
	}

	HRESULT D3D9Ex::D3D9Device::GetLight(DWORD Index, D3DLIGHT9* pLight)
	{
		return m_pIDirect3DDevice9->GetLight(Index, pLight);
	}

	HRESULT D3D9Ex::D3D9Device::LightEnable(DWORD Index, BOOL Enable)
	{
		return m_pIDirect3DDevice9->LightEnable(Index, Enable);
	}

	HRESULT D3D9Ex::D3D9Device::GetLightEnable(DWORD Index, BOOL* pEnable)
	{
		return m_pIDirect3DDevice9->GetLightEnable(Index, pEnable);
	}

	HRESULT D3D9Ex::D3D9Device::SetClipPlane(DWORD Index, CONST float* pPlane)
	{
		return m_pIDirect3DDevice9->SetClipPlane(Index, pPlane);
	}

	HRESULT D3D9Ex::D3D9Device::GetClipPlane(DWORD Index, float* pPlane)
	{
		return m_pIDirect3DDevice9->GetClipPlane(Index, pPlane);
	}

	HRESULT D3D9Ex::D3D9Device::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
	{
		return m_pIDirect3DDevice9->SetRenderState(State, Value);
	}

	HRESULT D3D9Ex::D3D9Device::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
	{
		return m_pIDirect3DDevice9->GetRenderState(State, pValue);
	}

	HRESULT D3D9Ex::D3D9Device::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
	{
		return m_pIDirect3DDevice9->CreateStateBlock(Type, ppSB);
	}

	HRESULT D3D9Ex::D3D9Device::BeginStateBlock()
	{
		return m_pIDirect3DDevice9->BeginStateBlock();
	}

	HRESULT D3D9Ex::D3D9Device::EndStateBlock(IDirect3DStateBlock9** ppSB)
	{
		return m_pIDirect3DDevice9->EndStateBlock(ppSB);
	}

	HRESULT D3D9Ex::D3D9Device::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
	{
		return m_pIDirect3DDevice9->SetClipStatus(pClipStatus);
	}

	HRESULT D3D9Ex::D3D9Device::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
	{
		return m_pIDirect3DDevice9->GetClipStatus(pClipStatus);
	}

	HRESULT D3D9Ex::D3D9Device::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture)
	{
		return m_pIDirect3DDevice9->GetTexture(Stage, ppTexture);
	}

	HRESULT D3D9Ex::D3D9Device::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
	{
		return m_pIDirect3DDevice9->SetTexture(Stage, pTexture);
	}

	HRESULT D3D9Ex::D3D9Device::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
	{
		return m_pIDirect3DDevice9->GetTextureStageState(Stage, Type, pValue);
	}

	HRESULT D3D9Ex::D3D9Device::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
	{
		return m_pIDirect3DDevice9->SetTextureStageState(Stage, Type, Value);
	}

	HRESULT D3D9Ex::D3D9Device::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
	{
		return m_pIDirect3DDevice9->GetSamplerState(Sampler, Type, pValue);
	}

	HRESULT D3D9Ex::D3D9Device::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
	{
		return m_pIDirect3DDevice9->SetSamplerState(Sampler, Type, Value);
	}

	HRESULT D3D9Ex::D3D9Device::ValidateDevice(DWORD* pNumPasses)
	{
		return m_pIDirect3DDevice9->ValidateDevice(pNumPasses);
	}

	HRESULT D3D9Ex::D3D9Device::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
	{
		return m_pIDirect3DDevice9->SetPaletteEntries(PaletteNumber, pEntries);
	}

	HRESULT D3D9Ex::D3D9Device::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
	{
		return m_pIDirect3DDevice9->GetPaletteEntries(PaletteNumber, pEntries);
	}

	HRESULT D3D9Ex::D3D9Device::SetCurrentTexturePalette(UINT PaletteNumber)
	{
		return m_pIDirect3DDevice9->SetCurrentTexturePalette(PaletteNumber);
	}

	HRESULT D3D9Ex::D3D9Device::GetCurrentTexturePalette(UINT *PaletteNumber)
	{
		return m_pIDirect3DDevice9->GetCurrentTexturePalette(PaletteNumber);
	}

	HRESULT D3D9Ex::D3D9Device::SetScissorRect(CONST RECT* pRect)
	{
		return m_pIDirect3DDevice9->SetScissorRect(pRect);
	}

	HRESULT D3D9Ex::D3D9Device::GetScissorRect(RECT* pRect)
	{
		return m_pIDirect3DDevice9->GetScissorRect(pRect);
	}

	HRESULT D3D9Ex::D3D9Device::SetSoftwareVertexProcessing(BOOL bSoftware)
	{
		return m_pIDirect3DDevice9->SetSoftwareVertexProcessing(bSoftware);
	}

	BOOL D3D9Ex::D3D9Device::GetSoftwareVertexProcessing()
	{
		return m_pIDirect3DDevice9->GetSoftwareVertexProcessing();
	}

	HRESULT D3D9Ex::D3D9Device::SetNPatchMode(float nSegments)
	{
		return m_pIDirect3DDevice9->SetNPatchMode(nSegments);
	}

	float D3D9Ex::D3D9Device::GetNPatchMode()
	{
		return m_pIDirect3DDevice9->GetNPatchMode();
	}

	HRESULT D3D9Ex::D3D9Device::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
	{
		return m_pIDirect3DDevice9->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
	}

	HRESULT D3D9Ex::D3D9Device::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
	{
		return m_pIDirect3DDevice9->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}

	HRESULT D3D9Ex::D3D9Device::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
	{
		return m_pIDirect3DDevice9->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
	}

	HRESULT D3D9Ex::D3D9Device::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
	{
		return m_pIDirect3DDevice9->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
	}

	HRESULT D3D9Ex::D3D9Device::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
	{
		return m_pIDirect3DDevice9->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
	}

	HRESULT D3D9Ex::D3D9Device::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
	{
		return m_pIDirect3DDevice9->CreateVertexDeclaration(pVertexElements, ppDecl);
	}

	HRESULT D3D9Ex::D3D9Device::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
	{
		return m_pIDirect3DDevice9->SetVertexDeclaration(pDecl);
	}

	HRESULT D3D9Ex::D3D9Device::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
	{
		return m_pIDirect3DDevice9->GetVertexDeclaration(ppDecl);
	}

	HRESULT D3D9Ex::D3D9Device::SetFVF(DWORD FVF)
	{
		return m_pIDirect3DDevice9->SetFVF(FVF);
	}

	HRESULT D3D9Ex::D3D9Device::GetFVF(DWORD* pFVF)
	{
		return m_pIDirect3DDevice9->GetFVF(pFVF);
	}

	HRESULT D3D9Ex::D3D9Device::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
	{
		return m_pIDirect3DDevice9->CreateVertexShader(pFunction, ppShader);
	}

	HRESULT D3D9Ex::D3D9Device::SetVertexShader(IDirect3DVertexShader9* pShader)
	{
		return m_pIDirect3DDevice9->SetVertexShader(pShader);
	}

	HRESULT D3D9Ex::D3D9Device::GetVertexShader(IDirect3DVertexShader9** ppShader)
	{
		return m_pIDirect3DDevice9->GetVertexShader(ppShader);
	}

	HRESULT D3D9Ex::D3D9Device::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
	{
		return m_pIDirect3DDevice9->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
	}

	HRESULT D3D9Ex::D3D9Device::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
	{
		return m_pIDirect3DDevice9->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
	}

	HRESULT D3D9Ex::D3D9Device::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
	{
		return m_pIDirect3DDevice9->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
	}

	HRESULT D3D9Ex::D3D9Device::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
	{
		return m_pIDirect3DDevice9->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
	}

	HRESULT D3D9Ex::D3D9Device::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
	{
		return m_pIDirect3DDevice9->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
	}

	HRESULT D3D9Ex::D3D9Device::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
	{
		return m_pIDirect3DDevice9->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
	}

	HRESULT D3D9Ex::D3D9Device::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
	{
		return m_pIDirect3DDevice9->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
	}

	HRESULT D3D9Ex::D3D9Device::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* OffsetInBytes, UINT* pStride)
	{
		return m_pIDirect3DDevice9->GetStreamSource(StreamNumber, ppStreamData, OffsetInBytes, pStride);
	}

	HRESULT D3D9Ex::D3D9Device::SetStreamSourceFreq(UINT StreamNumber, UINT Divider)
	{
		return m_pIDirect3DDevice9->SetStreamSourceFreq(StreamNumber, Divider);
	}

	HRESULT D3D9Ex::D3D9Device::GetStreamSourceFreq(UINT StreamNumber, UINT* Divider)
	{
		return m_pIDirect3DDevice9->GetStreamSourceFreq(StreamNumber, Divider);
	}

	HRESULT D3D9Ex::D3D9Device::SetIndices(IDirect3DIndexBuffer9* pIndexData)
	{
		return m_pIDirect3DDevice9->SetIndices(pIndexData);
	}

	HRESULT D3D9Ex::D3D9Device::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
	{
		return m_pIDirect3DDevice9->GetIndices(ppIndexData);
	}

	HRESULT D3D9Ex::D3D9Device::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
	{
		return m_pIDirect3DDevice9->CreatePixelShader(pFunction, ppShader);
	}

	HRESULT D3D9Ex::D3D9Device::SetPixelShader(IDirect3DPixelShader9* pShader)
	{
		return m_pIDirect3DDevice9->SetPixelShader(pShader);
	}

	HRESULT D3D9Ex::D3D9Device::GetPixelShader(IDirect3DPixelShader9** ppShader)
	{
		return m_pIDirect3DDevice9->GetPixelShader(ppShader);
	}

	HRESULT D3D9Ex::D3D9Device::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
	{
		// Use real bad readptr check here, cause the query takes too long
		// TODO: Fix the actual error!
		if (IsBadReadPtr(pConstantData, Vector4fCount * 16))
		{
			//Logger::Print("Invalid shader constant array!\n");
			return D3DERR_INVALIDCALL;
		}

		return m_pIDirect3DDevice9->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
	}

	HRESULT D3D9Ex::D3D9Device::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
	{
		return m_pIDirect3DDevice9->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
	}

	HRESULT D3D9Ex::D3D9Device::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
	{
		return m_pIDirect3DDevice9->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
	}

	HRESULT D3D9Ex::D3D9Device::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
	{
		return m_pIDirect3DDevice9->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
	}

	HRESULT D3D9Ex::D3D9Device::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
	{
		return m_pIDirect3DDevice9->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
	}

	HRESULT D3D9Ex::D3D9Device::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
	{
		return m_pIDirect3DDevice9->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
	}

	HRESULT D3D9Ex::D3D9Device::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
	{
		return m_pIDirect3DDevice9->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
	}

	HRESULT D3D9Ex::D3D9Device::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
	{
		return m_pIDirect3DDevice9->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
	}

	HRESULT D3D9Ex::D3D9Device::DeletePatch(UINT Handle)
	{
		return m_pIDirect3DDevice9->DeletePatch(Handle);
	}

	HRESULT D3D9Ex::D3D9Device::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
	{
		return m_pIDirect3DDevice9->CreateQuery(Type, ppQuery);
	}

#pragma endregion

#pragma region D3D9

	HRESULT __stdcall D3D9Ex::D3D9::QueryInterface(REFIID riid, void** ppvObj)
	{
		*ppvObj = nullptr;

		HRESULT hRes = m_pIDirect3D9->QueryInterface(riid, ppvObj);

		if (hRes == NOERROR)
		{
			*ppvObj = this;
		}

		return hRes;
	}

	ULONG __stdcall D3D9Ex::D3D9::AddRef()
	{
		return m_pIDirect3D9->AddRef();
	}

	ULONG __stdcall D3D9Ex::D3D9::Release()
	{
		ULONG count = m_pIDirect3D9->Release();
		if (!count) delete this;
		return count;
	}

	HRESULT __stdcall D3D9Ex::D3D9::RegisterSoftwareDevice(void* pInitializeFunction)
	{
		return m_pIDirect3D9->RegisterSoftwareDevice(pInitializeFunction);
	}

	UINT __stdcall D3D9Ex::D3D9::GetAdapterCount()
	{
		return m_pIDirect3D9->GetAdapterCount();
	}

	HRESULT __stdcall D3D9Ex::D3D9::GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier)
	{
		return m_pIDirect3D9->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
	}

	UINT __stdcall D3D9Ex::D3D9::GetAdapterModeCount(UINT Adapter, D3DFORMAT Format)
	{
		return m_pIDirect3D9->GetAdapterModeCount(Adapter, Format);
	}

	HRESULT __stdcall D3D9Ex::D3D9::EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode)
	{
		return m_pIDirect3D9->EnumAdapterModes(Adapter, Format, Mode, pMode);
	}

	HRESULT __stdcall D3D9Ex::D3D9::GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode)
	{
		return m_pIDirect3D9->GetAdapterDisplayMode(Adapter, pMode);
	}

	HRESULT __stdcall D3D9Ex::D3D9::CheckDeviceType(UINT iAdapter, D3DDEVTYPE DevType, D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed)
	{
		return m_pIDirect3D9->CheckDeviceType(iAdapter, DevType, DisplayFormat, BackBufferFormat, bWindowed);
	}

	HRESULT __stdcall D3D9Ex::D3D9::CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat)
	{
		return m_pIDirect3D9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
	}

	HRESULT __stdcall D3D9Ex::D3D9::CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels)
	{
		return m_pIDirect3D9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
	}

	HRESULT __stdcall D3D9Ex::D3D9::CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat)
	{
		return m_pIDirect3D9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
	}

	HRESULT __stdcall D3D9Ex::D3D9::CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat)
	{
		return m_pIDirect3D9->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
	}

	HRESULT __stdcall D3D9Ex::D3D9::GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps)
	{
		return m_pIDirect3D9->GetDeviceCaps(Adapter, DeviceType, pCaps);
	}

	HMONITOR __stdcall D3D9Ex::D3D9::GetAdapterMonitor(UINT Adapter)
	{
		return m_pIDirect3D9->GetAdapterMonitor(Adapter);
	}

	HRESULT __stdcall D3D9Ex::D3D9::CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface)
	{
		HRESULT hres = m_pIDirect3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
		*ppReturnedDeviceInterface = new D3D9Ex::D3D9Device(*ppReturnedDeviceInterface);
		return hres;
	}

#pragma endregion

	IDirect3D9* __stdcall D3D9Ex::Direct3DCreate9Stub(UINT sdk)
	{
		if (Dvar::Var("r_useD3D9Ex").get<bool>())
		{
			IDirect3D9Ex* test = nullptr;
			if (FAILED(Direct3DCreate9Ex(sdk, &test))) return nullptr;

			return (new D3D9Ex::D3D9(test));
		}
		else
		{
			return Direct3DCreate9(sdk);
		}
	}

	D3D9Ex::D3D9Ex()
	{
		if (Dedicated::IsEnabled()) return;

		Dvar::Register<bool>("r_useD3D9Ex", true, Game::dvar_flag::DVAR_FLAG_SAVED, "Use extended d3d9 interface!");

		// Hook Interface creation
		Utils::Hook::Set(0x6D74D0, D3D9Ex::Direct3DCreate9Stub);
	}
}
