#include "../../Snct_Windows.h"
#include "../../Snct_Utility.h"
#include "../Snct_DXConstantParameter.h"
#include "Snct_DX12.h"

// �����X���b�h�킯�̐�
constexpr unsigned int NumContexts		= 3; 

// �����t���[�����\�[�X���Ŏ��s����R�}���h���X�g�̐�
// �T���v�����̐��l�� 3 �ƂȂ��Ă��邪
// ����͕K�v�Ȃ�����
constexpr unsigned int CommandListCount	= 1;

//�������Render�ł��g��


class SnctDX12FrameResource 
{
public:
	 SnctDX12FrameResource(ID3D12Device* pDevice, ID3D12PipelineState* pPso, UINT frameResourceIndex, ID3D12DescriptorHeap* pCbvSrvHeap);
	 ~SnctDX12FrameResource() {};

	// ComanndList���܂Ƃ߂Ď擾����p�̃|�C���^�z��@�����̃R�}���h���X�g�������Ă���
	ID3D12CommandList* m_pBatchSubmit[NumContexts * 2 + CommandListCount];

	// �X���b�h���s���Ȃ��R�}���h���X�g
	ComPtr<ID3D12CommandAllocator>		m_pCommandAllocators[CommandListCount];
	ComPtr<ID3D12GraphicsCommandList>	m_pCommandLists[CommandListCount];

	// ���ꂪ�X���b�h�����p�ӂ���Ă���R�}���h���X�g
	ComPtr<ID3D12CommandAllocator>		m_pObjectCommandAllocators[NumContexts];
	ComPtr<ID3D12GraphicsCommandList>	m_pObjectCommandLists[NumContexts];

	UINT m_fenceValue;

protected:

private:
	// Render����|�C���^�Ŏ擾
	ComPtr<ID3D12PipelineState> m_pPipelineState;

	// ���ۂ̃o�b�t�@�@�����������炢��񂩂�
	ComPtr<ID3D12Resource> m_pCameraConstantBuffer;
	ComPtr<ID3D12Resource> m_pObjectConstantBuffer;

	// �����Ă����f�[�^���ꎞ�ۑ�����ǂݎ��p�o�b�t�@
	XMConstantCamera* m_pConstantCamera;
	XMConstantObject* m_pConstantObject;

	D3D12_GPU_DESCRIPTOR_HANDLE m_cameraCbvHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_objectCbvHandle;

public:
	// RTV �� DSV ���o�C���h
	void Bind(ID3D12GraphicsCommandList* pCommandList, D3D12_CPU_DESCRIPTOR_HANDLE* pRtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE* pDsvHandle);
	
	// �R�}���h���X�g�̃��Z�b�g
	void CmdReset();

	// �R���X�^���g�o�b�t�@�̃A�b�v�f�[�g
	void UpdateConstantBuffers(XMConstantCamera* cameraConstant, XMConstantObject* objectConstant);
};