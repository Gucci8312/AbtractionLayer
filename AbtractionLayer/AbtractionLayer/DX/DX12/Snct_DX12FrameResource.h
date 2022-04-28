#include "../../Snct_Windows.h"
#include "../../Snct_Utility.h"
#include "../Snct_DXConstantParameter.h"
#include "Snct_DX12.h"

// 多分スレッドわけの数
constexpr unsigned int NumContexts		= 3; 

// 多分フレームリソース内で実行するコマンドリストの数
// サンプル内の数値は 3 となっているが
// 今回は必要なさそう
constexpr unsigned int CommandListCount	= 1;

//ここ二つはRenderでも使う


class SnctDX12FrameResource 
{
public:
	 SnctDX12FrameResource(ID3D12Device* pDevice, ID3D12PipelineState* pPso, UINT frameResourceIndex, ID3D12DescriptorHeap* pCbvSrvHeap);
	 ~SnctDX12FrameResource() {};

	// ComanndListをまとめて取得する用のポインタ配列　直下のコマンドリストが入っている
	ID3D12CommandList* m_pBatchSubmit[NumContexts * 2 + CommandListCount];

	// スレッド実行しないコマンドリスト
	ComPtr<ID3D12CommandAllocator>		m_pCommandAllocators[CommandListCount];
	ComPtr<ID3D12GraphicsCommandList>	m_pCommandLists[CommandListCount];

	// これがスレッド数分用意されているコマンドリスト
	ComPtr<ID3D12CommandAllocator>		m_pObjectCommandAllocators[NumContexts];
	ComPtr<ID3D12GraphicsCommandList>	m_pObjectCommandLists[NumContexts];

	UINT m_fenceValue;

protected:

private:
	// Renderからポインタで取得
	ComPtr<ID3D12PipelineState> m_pPipelineState;

	// 実際のバッファ　もしかしたらいらんかも
	ComPtr<ID3D12Resource> m_pCameraConstantBuffer;
	ComPtr<ID3D12Resource> m_pObjectConstantBuffer;

	// 持ってきたデータを一時保存する読み取り用バッファ
	XMConstantCamera* m_pConstantCamera;
	XMConstantObject* m_pConstantObject;

	D3D12_GPU_DESCRIPTOR_HANDLE m_cameraCbvHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_objectCbvHandle;

public:
	// RTV と DSV をバインド
	void Bind(ID3D12GraphicsCommandList* pCommandList, D3D12_CPU_DESCRIPTOR_HANDLE* pRtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE* pDsvHandle);
	
	// コマンドリストのリセット
	void CmdReset();

	// コンスタントバッファのアップデート
	void UpdateConstantBuffers(XMConstantCamera* cameraConstant, XMConstantObject* objectConstant);
};