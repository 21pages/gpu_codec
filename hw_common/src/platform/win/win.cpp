#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <cstdio>
#include <list>

#include <dxgi.h>
#include <d3d11.h>

#include "win.h"

bool NativeDevice::Init(AdapterVendor vendor, ID3D11Device *device)
{
	if (device) {
		if (!Init(device)) return false;
	} else {
		if (!Init(vendor)) return false;
	}
	if (vendor == AdapterVendor::ADAPTER_VENDOR_INTEL) {
		if (!SetMultithreadProtected()) return false;
	}
	return true;
}

bool NativeDevice::Init(AdapterVendor vendor)
{
    HRESULT hr = S_OK;

	HRB(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)factory1_.ReleaseAndGetAddressOf()));

	if (vendor == ADAPTER_VENDOR_ANY) {
		adapter1_.Reset();
	} else {
		ComPtr<IDXGIAdapter1> tmpAdapter = nullptr;
		for (int i = 0; !FAILED(factory1_->EnumAdapters1(i, tmpAdapter.ReleaseAndGetAddressOf())); i++) {
			if (vendor == ADAPTER_VENDOR_FIRST) {
				adapter1_.Swap(tmpAdapter);
				break;
			} else {
				DXGI_ADAPTER_DESC1 desc = DXGI_ADAPTER_DESC1();
				tmpAdapter->GetDesc1(&desc);
				if (desc.VendorId == static_cast<int>(vendor)) {
					adapter1_.Swap(tmpAdapter);
					break;
				}
			}
		}
		if (!adapter1_) {
			return false;
		}
	}

	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL featureLevel;
	D3D_DRIVER_TYPE d3dDriverType = adapter1_ ? D3D_DRIVER_TYPE_UNKNOWN: D3D_DRIVER_TYPE_HARDWARE;
	hr = D3D11CreateDevice(adapter1_.Get(), d3dDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, device_.ReleaseAndGetAddressOf(), &featureLevel, context_.ReleaseAndGetAddressOf());

	if (FAILED(hr))
	{
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		std::cerr << "Direct3D Feature Level 11 unsupported." << std::endl;
		return false;
	}
	return true;
}

bool NativeDevice::Init(ID3D11Device *device)
{
	device_ = device;
	device_->GetImmediateContext(context_.ReleaseAndGetAddressOf());
	
	return true;
}

bool NativeDevice::SetMultithreadProtected()
{
	ComPtr<ID3D10Multithread> hmt = nullptr;
	HRB(context_.As(&hmt));
    if (!hmt->SetMultithreadProtected(TRUE)) {
		if (!hmt->GetMultithreadProtected()) {
			std::cerr << "Failed to SetMultithreadProtected" << std::endl;
			return false;
		}
    }
	return true;
}