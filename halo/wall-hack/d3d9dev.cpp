/*	Direct3D9 Device */

#include <windows.h>
#include <vector>
#include "main.h"
#include "d3d9.h"

#define D3DHOOK_TEXTURES //comment this to disable texture hooking

bool Generate = true; // <-- to prevent flicker. will be explained later.
LPDIRECT3DTEXTURE9    texYellow, texRed;

int vCurNum = 5000;
int vLastHigh = 10000;
int vLastLow = 5;

int pCurNum = 5000;
int pLastHigh = 10000;
int pLastLow = 5;

bool vLogEnabled = false;
bool pLogEnabled = false;

IDirect3DTexture9 *pTexBlue;
IDirect3DTexture9 *pTexYellow;
IDirect3DTexture9 *pTexRed;
IDirect3DTexture9 *pTexGreen;
IDirect3DTexture9 *pTexPurple;
IDirect3DTexture9 *pTexOrange;

int CalculateDifference(int X, int Y)
{
	return (X + Y) / 2;
}

HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, IDirect3DTexture9 **ppD3Dtex, DWORD colour32)
{
	if (FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)))
		return E_FAIL;

	WORD colour16 = ((WORD)((colour32 >> 28) & 0xF) << 12)
		| (WORD)(((colour32 >> 20) & 0xF) << 8)
		| (WORD)(((colour32 >> 12) & 0xF) << 4)
		| (WORD)(((colour32 >> 4) & 0xF) << 0);

	D3DLOCKED_RECT d3dlr;
	(*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
	WORD *pDst16 = (WORD*)d3dlr.pBits;

	for (int xy = 0; xy < 8 * 8; xy++)
		*pDst16++ = colour16;

	(*ppD3Dtex)->UnlockRect(0);

	return S_OK;
}

HRESULT CD3DManager::Initialize()
{
	/*
	initialize Resources such as textures 
	(managed and unmanaged [D3DPOOL]), 
	vertex buffers, and other D3D rendering resources
	...
	m_pD3Ddev->CreateTexture(..., ..., &m_pD3Dtexture);
	*/
	return S_OK;
}

#pragma region unused

HRESULT CD3DManager::PreReset()
{
	/*
	release all UNMANAGED [D3DPOOL_DEFAULT] 
	textures, vertex buffers, and other 
	volitile resources
	...
	_SAFE_RELEASE(m_pD3Dtexture);
	*/
	return S_OK;
}

HRESULT CD3DManager::PostReset()
{
	/*
	re-initialize all UNMANAGED [D3DPOOL_DEFAULT]
	textures, vertex buffers, and other volitile 
	resources
	...
	m_pD3Ddev->CreateTexture(..., ..., &m_pD3Dtexture);
	*/
	return S_OK;
}

HRESULT CD3DManager::Release()
{
	/*
	Release all textures, vertex buffers, and 
	other resources
	...
	_SAFE_RELEASE(m_pD3Dtexture);
	*/
	return S_OK;
}

//-----------------------------------------------------------------------------

HRESULT APIENTRY hkIDirect3DDevice9::QueryInterface(REFIID riid, LPVOID *ppvObj) 
{
	return m_pD3Ddev->QueryInterface(riid, ppvObj);
}

ULONG APIENTRY hkIDirect3DDevice9::AddRef() 
{
	m_refCount++;
	return m_pD3Ddev->AddRef();
}

HRESULT APIENTRY hkIDirect3DDevice9::BeginScene() 
{
	return m_pD3Ddev->BeginScene();
}

HRESULT APIENTRY hkIDirect3DDevice9::BeginStateBlock() 
{
	return m_pD3Ddev->BeginStateBlock();
}

HRESULT APIENTRY hkIDirect3DDevice9::Clear(DWORD Count, CONST D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) 
{
	return m_pD3Ddev->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT APIENTRY hkIDirect3DDevice9::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect, D3DCOLOR color) 
{	
	return m_pD3Ddev->ColorFill(pSurface,pRect,color);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DSwapChain9 **ppSwapChain) 
{
	return m_pD3Ddev->CreateAdditionalSwapChain(pPresentationParameters, ppSwapChain);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture,pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality,Discard,ppSurface, pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer,pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateOffscreenPlainSurface(Width,Height,Format,Pool,ppSurface,pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader) 
{
	return m_pD3Ddev->CreatePixelShader(pFunction, ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery) 
{
	return m_pD3Ddev->CreateQuery(Type,ppQuery);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateRenderTarget(Width, Height, Format, MultiSample,MultisampleQuality, Lockable, ppSurface,pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB) 
{
	return m_pD3Ddev->CreateStateBlock(Type, ppSB);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) 
{
	HRESULT ret = m_pD3Ddev->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);

#ifdef D3DHOOK_TEXTURES
	if(ret == D3D_OK) { new hkIDirect3DTexture9(ppTexture, this, Width, Height, Format); }
#endif

	return ret;
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer,pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl) 
{
	return m_pD3Ddev->CreateVertexDeclaration(pVertexElements,ppDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader) 
{
	return m_pD3Ddev->CreateVertexShader(pFunction, ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle) 
{
	return m_pD3Ddev->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture,pSharedHandle);
}

HRESULT APIENTRY hkIDirect3DDevice9::DeletePatch(UINT Handle) 
{
	return m_pD3Ddev->DeletePatch(Handle);
}

#pragma endregion

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	
	if (vLogEnabled) {
		if (NumVertices < (UINT)vCurNum && NumVertices >= (UINT)vLastLow)
		{
			m_pD3Ddev->SetTexture(0, pTexBlue);
		}
		else if (NumVertices >(UINT)vCurNum && NumVertices <= (UINT)vLastHigh)
		{
			m_pD3Ddev->SetTexture(0, pTexRed);
		}
		else if (NumVertices == (UINT)vCurNum)
		{
			m_pD3Ddev->SetTexture(0, pTexYellow);
		}
	} else if (pLogEnabled) {
		if (primCount < (UINT)pCurNum && primCount >= (UINT)pLastLow)
		{
			m_pD3Ddev->SetTexture(0, pTexBlue);
			//m_pD3Ddev->SetTexture(0, pTexPurple);
		}
		else if (primCount >(UINT)pCurNum && primCount <= (UINT)pLastHigh)
		{
			m_pD3Ddev->SetTexture(0, pTexRed);
			//m_pD3Ddev->SetTexture(0, pTexOrange);
		}
		else if (primCount == (UINT)pCurNum)
		{
			m_pD3Ddev->SetTexture(0, pTexYellow);
			//m_pD3Ddev->SetTexture(0, pTexGreen);
		}
	} else {

#pragma region halo grunt parts
		const bool HALO_GRUNT_BODY1 =  ((primCount == 781 && NumVertices == 345)  || // (closest)
										(primCount == 689 && NumVertices == 311)  || // (next)
										(primCount == 563 && NumVertices == 264)  || // (next)
										(primCount == 460 && NumVertices == 208)  || // (next)
										(primCount == 402 && NumVertices == 207));   // (furthest)
		const bool HALO_GRUNT_BODY2 =  ((primCount == 866 && NumVertices == 443)  || // (closest)
										(primCount == 762 && NumVertices == 407)  || // (next)
										(primCount == 636 && NumVertices == 350)  || // (next)
										(primCount == 462 && NumVertices == 270)  || // (next)
										(primCount == 310 && NumVertices == 180));   // (furthest)
		const bool HALO_GRUNT_LIMBS =  ((primCount == 1151 && NumVertices == 578) || // (closest)
										(primCount == 977 && NumVertices == 500)  || // (next)
										(primCount == 764 && NumVertices == 402)  || // (next)
										(primCount == 476 && NumVertices == 261)  || // (next)
										(primCount == 299 && NumVertices == 161));   // (furthest)
		const bool HALO_GRUNT_HEAD1 =  ((primCount == 559 && NumVertices == 251)  || // (closest)
										(primCount == 496 && NumVertices == 232)  || // (next)
										(primCount == 435 && NumVertices == 208)  || // (next)
										(primCount == 292 && NumVertices == 156)  || // (next)
										(primCount == 164 && NumVertices == 81));    // (furthest)
		const bool HALO_GRUNT_HEAD2 =  ((primCount == 745 && NumVertices == 417)  || // (closest)
										(primCount == 901 && NumVertices == 529)  || // (next)
										(primCount == 718 && NumVertices == 404)  || // (next)
										(primCount == 512 && NumVertices == 284)  || // (next)
										(primCount == 198 && NumVertices == 99));    // (furthest)
		const bool HALO_GRUNT_SHELL1 = ((primCount == 124 && NumVertices == 54)   || // (closest)
										(primCount == 88 && NumVertices == 42)    || // (next)
										(primCount == 88 && NumVertices == 41)    || // (next)
										(primCount == 87 && NumVertices == 45)    || // (next)
										(primCount == 164 && NumVertices == 81));    // (furthest)
		const bool HALO_GRUNT_SHELL2 = ((primCount == 745 && NumVertices == 417)  || // (closest)
										(primCount == 901 && NumVertices == 529)  || // (next)
										(primCount == 718 && NumVertices == 404)  || // (next)
										(primCount == 512 && NumVertices == 284)  || // (next)
										(primCount == 197 && NumVertices == 99));    // (furthest)
#pragma endregion

		const bool HALO_ELITE_BODY1 =  ((primCount == 522 && NumVertices == 241) || // (closest #1)
										(primCount == 522 && NumVertices == 244) || // (closest #2)
										(primCount == 375 && NumVertices == 192) || // (next)
										(primCount == 360 && NumVertices == 181) || // (next)
										(primCount == 244 && NumVertices == 129) || // (next)
										(primCount == 274 && NumVertices == 169));  // (furthest)
		const bool HALO_ELITE_LEGS1 =  ((primCount == 977 && NumVertices == 489) || // (closest)
										(primCount == 780 && NumVertices == 418) || // (next)
										(primCount == 687 && NumVertices == 378) || // (next)
										(primCount == 432 && NumVertices == 256) || // (next)
										(primCount == 256 && NumVertices == 180));  // (furthest)
		const bool HALO_ELITE_LEGS2 = ((primCount == 956 && NumVertices == 499) ||  // (closest)
										(primCount == 796 && NumVertices == 428) || // (next)
										(primCount == 678 && NumVertices == 377) || // (next)
										(primCount == 397 && NumVertices == 213) || // (next)
										(primCount == 262 && NumVertices == 184));  // (furthest)
		const bool HALO_ELITE_ARMS1 =  ((primCount == 878 && NumVertices == 468) || // (closest)
										(primCount == 752 && NumVertices == 414) || // (next)
										(primCount == 701 && NumVertices == 399) || // (next)
										(primCount == 568 && NumVertices == 326) || // (next)
										(primCount == 516 && NumVertices == 340));  // (furthest)
		const bool HALO_ELITE_HEAD1 =  ((primCount == 532 && NumVertices == 291) || // (closest)
										(primCount == 413 && NumVertices == 237) || // (next)
										(primCount == 402 && NumVertices == 226) || // (next)
										(primCount == 289 && NumVertices == 156) || // (next)
										(primCount == 245 && NumVertices == 156));  // (furthest)

		const bool HALO_JACKAL_BODY1 = ((primCount == 688 && NumVertices == 338) ||  // (closest)
										(primCount == 651 && NumVertices == 324) ||  // (next)
										(primCount == 606 && NumVertices == 310) ||  // (next)
										(primCount == 519 && NumVertices == 270) ||  // (next)
										(primCount == 357 && NumVertices == 198));   // (furthest)
		const bool HALO_JACKAL_LEGS1 = ((primCount == 1261 && NumVertices == 699) || // (closest)
										(primCount == 1199 && NumVertices == 660) || // (next)
										(primCount == 1006 && NumVertices == 570) || // (next)
										(primCount == 466 && NumVertices == 250) ||  // (next)
										(primCount == 354 && NumVertices == 204));   // (furthest)
		const bool HALO_JACKAL_ARMS1 = ((primCount == 1055 && NumVertices == 489) || // (closest)
										(primCount == 918 && NumVertices == 435) ||  // (next)
										(primCount == 654 && NumVertices == 323) ||  // (next)
										(primCount == 453 && NumVertices == 237) ||  // (next)
										(primCount == 210 && NumVertices == 119));   // (furthest)
		const bool HALO_JACKAL_HAIR1 = ((primCount == 134 && NumVertices == 95) ||   // (closest)
										(primCount == 137 && NumVertices == 92) ||   // (next)
										(primCount == 128 && NumVertices == 86) ||   // (next)
										(primCount == 92 && NumVertices == 64) ||    // (next)
										(primCount == 73 && NumVertices == 51));     // (furthest)

		const bool HALO_GRUNT = HALO_GRUNT_BODY1 || HALO_GRUNT_BODY2 || HALO_GRUNT_LIMBS || HALO_GRUNT_HEAD1 || HALO_GRUNT_HEAD2 || HALO_GRUNT_SHELL1 || HALO_GRUNT_SHELL2;
		const bool HALO_ELITE = HALO_ELITE_BODY1 || HALO_ELITE_LEGS1 || HALO_ELITE_LEGS2 || HALO_ELITE_ARMS1 || HALO_ELITE_HEAD1;
		const bool HALO_JACKAL = HALO_JACKAL_BODY1 || HALO_JACKAL_LEGS1 || HALO_JACKAL_ARMS1 || HALO_JACKAL_HAIR1;

		if (HALO_GRUNT || HALO_ELITE || HALO_JACKAL) {
			DWORD dwOldZEnable = D3DZB_TRUE;
			m_pD3Ddev->SetTexture(0, pTexYellow);
			m_pD3Ddev->GetRenderState(D3DRS_ZENABLE, &dwOldZEnable);
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			m_pD3Ddev->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			m_pD3Ddev->SetRenderState(D3DRS_ZENABLE, dwOldZEnable);
			m_pD3Ddev->SetTexture(0, pTexRed);
		}
	}
	
	return m_pD3Ddev->DrawIndexedPrimitive(Type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

#pragma region unused

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT PrimitiveCount, CONST void *pIndexData, D3DFORMAT IndexDataFormat, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) 
{	
	return m_pD3Ddev->DrawIndexedPrimitiveUP(PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) 
{
	return m_pD3Ddev->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) 
{
	return m_pD3Ddev->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawRectPatch(UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo) 
{
	return m_pD3Ddev->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT APIENTRY hkIDirect3DDevice9::DrawTriPatch(UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo)
{
	return m_pD3Ddev->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

#pragma endregion

HRESULT APIENTRY hkIDirect3DDevice9::EndScene()
{
	//this belongs in your ENDSCENE
	/*
	Remember our BOOL Generate? this is here so that your chams don't
	flicker, over and over rapidly, cause it's ****ing annoying.
	so, this bool makes sure that they're only generated once when the endscene is first initiated.
	and not over, and over.
	*/

	
	if (Generate)
	{
		GenerateTexture(m_pD3Ddev, &pTexBlue, D3DCOLOR_ARGB(255, 0, 0, 255));
		GenerateTexture(m_pD3Ddev, &pTexRed, D3DCOLOR_ARGB(255, 255, 0, 0));
		GenerateTexture(m_pD3Ddev, &pTexYellow, D3DCOLOR_ARGB(255, 255, 255, 0));
		GenerateTexture(m_pD3Ddev, &pTexGreen, D3DCOLOR_ARGB(255, 0, 255, 0));
		GenerateTexture(m_pD3Ddev, &pTexPurple, D3DCOLOR_ARGB(255, 128, 0, 128));
		GenerateTexture(m_pD3Ddev, &pTexOrange, D3DCOLOR_ARGB(255, 255, 165, 0));
		Generate = false;
	}
	
	return m_pD3Ddev->EndScene();
}

#pragma region unused

HRESULT APIENTRY hkIDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB) 
{
	return m_pD3Ddev->EndStateBlock(ppSB);
}

HRESULT APIENTRY hkIDirect3DDevice9::EvictManagedResources() 
{
	return m_pD3Ddev->EvictManagedResources();
}

UINT APIENTRY hkIDirect3DDevice9::GetAvailableTextureMem() 
{
	return m_pD3Ddev->GetAvailableTextureMem();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer) 
{
	return m_pD3Ddev->GetBackBuffer(iSwapChain,iBackBuffer, Type, ppBackBuffer);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipPlane(DWORD Index, float *pPlane) 
{
	return m_pD3Ddev->GetClipPlane(Index, pPlane);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9 *pClipStatus) 
{
	return m_pD3Ddev->GetClipStatus(pClipStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters) 
{
	return m_pD3Ddev->GetCreationParameters(pParameters);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetCurrentTexturePalette(UINT *pPaletteNumber)
{
	return m_pD3Ddev->GetCurrentTexturePalette(pPaletteNumber);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9 **ppZStencilSurface) 
{
	return m_pD3Ddev->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDeviceCaps(D3DCAPS9 *pCaps) 
{
	return m_pD3Ddev->GetDeviceCaps(pCaps);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDirect3D(IDirect3D9 **ppD3D9) 
{
	HRESULT hRet = m_pD3Ddev->GetDirect3D(ppD3D9);
	if( SUCCEEDED(hRet) )
		*ppD3D9 = m_pD3Dint;
	return hRet;
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode) 
{
	return m_pD3Ddev->GetDisplayMode(iSwapChain,pMode);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface) 
{
	return m_pD3Ddev->GetFrontBufferData(iSwapChain,pDestSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetFVF(DWORD* pFVF) 
{
	return m_pD3Ddev->GetFVF(pFVF);
}

void APIENTRY hkIDirect3DDevice9::GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp) 
{
	m_pD3Ddev->GetGammaRamp(iSwapChain,pRamp);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData) 
{
	return m_pD3Ddev->GetIndices(ppIndexData);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetLight(DWORD Index, D3DLIGHT9 *pLight) 
{
	return m_pD3Ddev->GetLight(Index, pLight);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetLightEnable(DWORD Index, BOOL *pEnable) 
{
	return m_pD3Ddev->GetLightEnable(Index, pEnable);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetMaterial(D3DMATERIAL9 *pMaterial) 
{
	return m_pD3Ddev->GetMaterial(pMaterial);
}

float APIENTRY hkIDirect3DDevice9::GetNPatchMode() 
{
	return m_pD3Ddev->GetNPatchMode();
}

unsigned int APIENTRY hkIDirect3DDevice9::GetNumberOfSwapChains() 
{
	return m_pD3Ddev->GetNumberOfSwapChains();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY *pEntries)
{
	return m_pD3Ddev->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader) 
{
	return m_pD3Ddev->GetPixelShader(ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount) 
{
	return m_pD3Ddev->GetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{
	return m_pD3Ddev->GetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	return m_pD3Ddev->GetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus) 
{
	return m_pD3Ddev->GetRasterStatus(iSwapChain,pRasterStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State, DWORD *pValue) 
{
	return m_pD3Ddev->GetRenderState(State, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget) 
{
	return m_pD3Ddev->GetRenderTarget(RenderTargetIndex,ppRenderTarget);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface) 
{
	return m_pD3Ddev->GetRenderTargetData(pRenderTarget,pDestSurface);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue) 
{
	return m_pD3Ddev->GetSamplerState(Sampler,Type,pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetScissorRect(RECT* pRect) 
{
	return m_pD3Ddev->GetScissorRect(pRect);
}

BOOL APIENTRY hkIDirect3DDevice9::GetSoftwareVertexProcessing() 
{
	return m_pD3Ddev->GetSoftwareVertexProcessing();
}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* OffsetInBytes,UINT* pStride) 
{
	return m_pD3Ddev->GetStreamSource(StreamNumber, ppStreamData,OffsetInBytes, pStride);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber,UINT* Divider) 
{
	return m_pD3Ddev->GetStreamSourceFreq(StreamNumber,Divider);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
	return m_pD3Ddev->GetSwapChain(iSwapChain,pSwapChain);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTexture(DWORD Stage, IDirect3DBaseTexture9 **ppTexture) 
{
	return m_pD3Ddev->GetTexture(Stage, ppTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue) 
{
	return m_pD3Ddev->GetTextureStageState(Stage, Type, pValue);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix) 
{
	return m_pD3Ddev->GetTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) 
{
	return m_pD3Ddev->GetVertexDeclaration(ppDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader) 
{
	return m_pD3Ddev->GetVertexShader(ppShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	return m_pD3Ddev->GetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{
	return m_pD3Ddev->GetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	return m_pD3Ddev->GetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetViewport(D3DVIEWPORT9 *pViewport) 
{
	return m_pD3Ddev->GetViewport(pViewport);
}

HRESULT APIENTRY hkIDirect3DDevice9::LightEnable(DWORD LightIndex, BOOL bEnable) 
{
	return m_pD3Ddev->LightEnable(LightIndex, bEnable);
}

HRESULT APIENTRY hkIDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) 
{
	return m_pD3Ddev->MultiplyTransform(State, pMatrix);
}

#pragma endregion

HRESULT APIENTRY hkIDirect3DDevice9::Present(CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion) 
{	
	int vTemp = vCurNum;
	int pTemp = pCurNum;

	if (vLogEnabled) {
		if (GetAsyncKeyState(VK_NUMPAD6) & 1)
		{
			vCurNum = CalculateDifference(vCurNum, vLastHigh);
			vLastLow = vTemp;
			add_log("* vertexes increased to: %d", vCurNum);
		}
		else if (GetAsyncKeyState(VK_NUMPAD4) & 1)
		{
			vCurNum = CalculateDifference(vCurNum, vLastLow);
			vLastHigh = vTemp;
			add_log("* vertexes decreased to: %d", vCurNum);
		}
		if (GetAsyncKeyState(VK_NUMPAD5) & 1)
		{
			add_log("/*========================");
			add_log("  num vertexes logged: %d", vCurNum);
			add_log("========================*/");
		}
		if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		{
			vCurNum = 5000;
			vLastHigh = 10000;
			vLastLow = 5;
		}
	} else if (pLogEnabled) {
		if (GetAsyncKeyState(VK_NUMPAD6) & 1)
		{
			pCurNum = CalculateDifference(pCurNum, pLastHigh);
			pLastLow = pTemp;
			add_log("* primcount increased to: %d", pCurNum);
		}
		else if (GetAsyncKeyState(VK_NUMPAD4) & 1)
		{
			pCurNum = CalculateDifference(pCurNum, pLastLow);
			pLastHigh = pTemp;
			add_log("* primcount decreased to: %d", pCurNum);
		}
		if (GetAsyncKeyState(VK_NUMPAD5) & 1)
		{
			add_log("/*========================");
			add_log("  primcount logged: %d", pCurNum);
			add_log("========================*/");
		}
		if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		{
			pCurNum = 5000;
			pLastHigh = 10000;
			pLastLow = 5;
		}
	}
	if (GetAsyncKeyState(VK_NUMPAD1) & 1)
	{
		vLogEnabled = !vLogEnabled;
		add_log("vertex logging: %s", vLogEnabled ? "enabled" : "disabled");
	}
	if (GetAsyncKeyState(VK_NUMPAD3) & 1)
	{
		pLogEnabled = !pLogEnabled;
		add_log("primcount logging: %s", pLogEnabled ? "enabled" : "disabled");
	}

	return m_pD3Ddev->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

#pragma region unused

HRESULT APIENTRY hkIDirect3DDevice9::ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags) 
{
	return m_pD3Ddev->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer,pVertexDecl, Flags);
}

ULONG APIENTRY hkIDirect3DDevice9::Release() 
{
	if( --m_refCount == 0 )
		m_pManager->Release();

	return m_pD3Ddev->Release();
}

HRESULT APIENTRY hkIDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS *pPresentationParameters) 
{
	m_pManager->PreReset();

	HRESULT hRet = m_pD3Ddev->Reset(pPresentationParameters);

	if( SUCCEEDED(hRet) )
	{
		m_PresentParam = *pPresentationParameters;
		m_pManager->PostReset();
	}

	return hRet;
}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipPlane(DWORD Index, CONST float *pPlane) 
{
	return m_pD3Ddev->SetClipPlane(Index, pPlane);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9 *pClipStatus) 
{
	return m_pD3Ddev->SetClipStatus(pClipStatus);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber) 
{
	return m_pD3Ddev->SetCurrentTexturePalette(PaletteNumber);
}

void APIENTRY hkIDirect3DDevice9::SetCursorPosition(int X, int Y, DWORD Flags) 
{
	m_pD3Ddev->SetCursorPosition(X, Y, Flags);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 *pCursorBitmap) 
{
	return m_pD3Ddev->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil) 
{
	return m_pD3Ddev->SetDepthStencilSurface(pNewZStencil);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs) 
{
	return m_pD3Ddev->SetDialogBoxMode(bEnableDialogs);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetFVF(DWORD FVF) 
{
	return m_pD3Ddev->SetFVF(FVF);
}

void APIENTRY hkIDirect3DDevice9::SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{
	m_pD3Ddev->SetGammaRamp(iSwapChain,Flags, pRamp);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData) 
{
	return m_pD3Ddev->SetIndices(pIndexData);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetLight(DWORD Index, CONST D3DLIGHT9 *pLight) 
{
	return m_pD3Ddev->SetLight(Index, pLight);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9 *pMaterial) 
{	
	return m_pD3Ddev->SetMaterial(pMaterial);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetNPatchMode(float nSegments) 
{	
	return m_pD3Ddev->SetNPatchMode(nSegments);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY *pEntries) 
{
	return m_pD3Ddev->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader) 
{
	return m_pD3Ddev->SetPixelShader(pShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) 
{
	return m_pD3Ddev->SetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{
	return m_pD3Ddev->SetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{
	return m_pD3Ddev->SetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) 
{
	return m_pD3Ddev->SetRenderState(State, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) 
{
	return m_pD3Ddev->SetRenderTarget(RenderTargetIndex,pRenderTarget);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value) 
{
	return m_pD3Ddev->SetSamplerState(Sampler,Type,Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetScissorRect(CONST RECT* pRect) 
{
	return m_pD3Ddev->SetScissorRect(pRect);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware) 
{
	return m_pD3Ddev->SetSoftwareVertexProcessing(bSoftware);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride) 
{
	return m_pD3Ddev->SetStreamSource(StreamNumber, pStreamData,OffsetInBytes, Stride);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber,UINT Divider)
{	
	return m_pD3Ddev->SetStreamSourceFreq(StreamNumber,Divider);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTexture(DWORD Stage, IDirect3DBaseTexture9 *pTexture) 
{
#ifdef D3DHOOK_TEXTURES
	IDirect3DDevice9 *dev = NULL;
	if(pTexture != NULL && ((hkIDirect3DTexture9*)(pTexture))->GetDevice(&dev) == D3D_OK)
	{
		if(dev == this)
			return m_pD3Ddev->SetTexture(Stage, ((hkIDirect3DTexture9*)(pTexture))->m_D3Dtex);
	}
#endif
	
	return m_pD3Ddev->SetTexture(Stage, pTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) 
{
	return m_pD3Ddev->SetTextureStageState(Stage, Type, Value);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) 
{
	return m_pD3Ddev->SetTransform(State, pMatrix);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) 
{
	return m_pD3Ddev->SetVertexDeclaration(pDecl);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* pShader) 
{
	return m_pD3Ddev->SetVertexShader(pShader);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) 
{
	return m_pD3Ddev->SetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{
	return m_pD3Ddev->SetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{
	return m_pD3Ddev->SetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9 *pViewport) 
{
	return m_pD3Ddev->SetViewport(pViewport);
}

BOOL APIENTRY hkIDirect3DDevice9::ShowCursor(BOOL bShow) 
{
	return m_pD3Ddev->ShowCursor(bShow);
}

HRESULT APIENTRY hkIDirect3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter) 
{
	return m_pD3Ddev->StretchRect(pSourceSurface,pSourceRect,pDestSurface,pDestRect,Filter);
}

HRESULT APIENTRY hkIDirect3DDevice9::TestCooperativeLevel() 
{
	return m_pD3Ddev->TestCooperativeLevel();
}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint) 
{
	return m_pD3Ddev->UpdateSurface(pSourceSurface,pSourceRect,pDestinationSurface,pDestPoint);
}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9 *pSourceTexture, IDirect3DBaseTexture9 *pDestinationTexture) 
{
	return m_pD3Ddev->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT APIENTRY hkIDirect3DDevice9::ValidateDevice(DWORD *pNumPasses) 
{
	return m_pD3Ddev->ValidateDevice(pNumPasses);
}

#pragma endregion
