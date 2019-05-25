//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include "DXSample.h"
#include "nv_helpers_dx12/TopLevelASGenerator.h"
#include <vector>
#include "dxc/dxcapi.h"

using namespace DirectX;

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

class D3D12HelloTriangle : public DXSample
{
public:
	D3D12HelloTriangle(UINT width, UINT height, std::wstring name);

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void OnDestroy() override;
	virtual void OnKeyUp(UINT8 key) override;

private:
	static const UINT FrameCount = 2;

	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	struct AccelerationStructureBuffers
	{
		ComPtr<ID3D12Resource> scratch;
		ComPtr<ID3D12Resource> result;
		ComPtr<ID3D12Resource> descriptors;
	};

	nv_helpers_dx12::TopLevelASGenerator m_topLevelASGenerator;
	AccelerationStructureBuffers m_topLevelASBuffers;

	struct BottomLevelASInstance
	{
		ComPtr<ID3D12Resource> bottomLevelAS;
		DirectX::XMMATRIX transformation;
	};
	std::vector<BottomLevelASInstance> m_bottomLevelASInstances;
	ComPtr<ID3D12Resource> m_bottomLevelAS;

	/// Create the acceleration structure of an instance
	///
	/// \param     vVertexBuffers : pair of buffer and vertex count
	/// \return    AccelerationStructureBuffers for TLAS
	AccelerationStructureBuffers CreateBottomLevelAS(std::vector<std::pair<ComPtr<ID3D12Resource>, uint32_t>> vVertexBuffers);

	/// Create the main acceleration structure that holds
	/// all instances of the scene
	/// \param     instances : pair of BLAS and transform
	void CreateTopLevelAS(const std::vector<BottomLevelASInstance>& bottomLevelASInstances);

	/// Create all acceleration structures, bottom and top
	void CreateAccelerationStructures();


	// Pipeline objects.
	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Device5> m_device;
	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12GraphicsCommandList4> m_commandList;
	UINT m_rtvDescriptorSize;

	// App resources.
	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	// Synchronization objects.
	UINT m_frameIndex;
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValue;

	bool m_raster = true;

	void LoadPipeline();
	void LoadAssets();
	void PopulateCommandList();
	void WaitForPreviousFrame();




	ComPtr<ID3D12RootSignature> CreateRayGenSignature();
	ComPtr<ID3D12RootSignature> CreateMissSignature();
	ComPtr<ID3D12RootSignature> CreateHitSignature();

	void CreateRaytracingPipeline();

	ComPtr<IDxcBlob> m_rayGenLibrary;
	ComPtr<IDxcBlob> m_hitLibrary;
	ComPtr<IDxcBlob> m_missLibrary;
	ComPtr<IDxcBlob> m_shadowLibrary;


	ComPtr<ID3D12RootSignature> m_rayGenSignature;
	ComPtr<ID3D12RootSignature> m_hitSignature;
	ComPtr<ID3D12RootSignature> m_missSignature;
	ComPtr<ID3D12RootSignature> m_shadowSignature;

	ComPtr<ID3D12StateObject> m_rtStateObject;
	ComPtr<ID3D12StateObjectProperties> m_rtStateObjectProps;
};
