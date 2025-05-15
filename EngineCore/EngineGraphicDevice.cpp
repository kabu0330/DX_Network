#include "PreCompile.h"
#include "EngineGraphicDevice.h"
#include "EngineTexture.h"
#include "EngineDepthStencilState.h"
#include "EngineRenderTarget.h"

UEngineGraphicDevice::UEngineGraphicDevice()
{
}

UEngineGraphicDevice::~UEngineGraphicDevice()
{
    Release();
}

void UEngineGraphicDevice::Release()
{
    MainAdapter = nullptr;
    SwapChain = nullptr;
    Context = nullptr;
    Device = nullptr;
}

IDXGIAdapter* UEngineGraphicDevice::FindHighPerformanceAdapter()
{
    // 1. �׷���ī�尡 ���� ���� ��츦 ����Ͽ� ���� ���� �׷���ī�带 ã�´�.
    IDXGIFactory* Factory = nullptr; // ���丮 : �׷��� ī�� ���� ����, ��� ��ġ ����

    HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));
    if (nullptr == Factory)
    {
        MSGASSERT("���丮 ��ü ������ �����߽��ϴ�.");
        return nullptr;
    }

    unsigned __int64 MaxVideoMemory = 0; // �ְ����� ������ VRAM���� ���ߴ�. ����? ���α׷��Ӱ�.
    IDXGIAdapter* ResultAdapter = nullptr;  // ���� ���� ���� �׷���ī�� ������ ���� ��ü

    // �׷���ī�带 �����Ͽ� ���� ������ ���� �׷���ī�带 �����ϰڴ�.
    for (int Index = 0;; ++Index)
    {
        IDXGIAdapter* CurAdapter = nullptr;
        Factory->EnumAdapters(Index, &CurAdapter); // �׷���ī�带 ���������� ����

        if (nullptr == CurAdapter)
        {
            // ���̻� ������ �׷���ī�尡 ������ �ݺ��� ����
            break; // �ݺ��� Ż��
        }

        DXGI_ADAPTER_DESC Desc;
        CurAdapter->GetDesc(&Desc); // �׷���ī�� ������ ������ Desc�� ����

        if (MaxVideoMemory <= Desc.DedicatedVideoMemory) // ������ �ְ� ���� ���غ��� ���ٸ�
        {
            MaxVideoMemory = Desc.DedicatedVideoMemory;  // VRAM ���� ����ġ�� �����ϰ�
            if (nullptr != ResultAdapter) // ������ �ִ� ����ʹ� �����ϰ�
            {
                ResultAdapter->Release();
            }

            ResultAdapter = CurAdapter; // ���� ���� �׷���ī�� ������ Result ����Ϳ� �����Ѵ�.
            continue;
        }
        CurAdapter->Release(); // ���� ����� ������ ���������� ���������� ���������ְ�
    }
    // ���� �׷���ī�� ������ ������ ����� ��ü�� �����ߴ�.

    if (nullptr != Factory)
    {
        Factory->Release(); // �׷���ī�� ������ ����Ϳ� ���������� ���丮�� ������ ���ߴ�.
    }

    if (nullptr == ResultAdapter)
    {
        MSGASSERT("�׷���ī�尡 �������� �ʴ� PC�Դϴ�.");
        return nullptr;
    }

    return ResultAdapter;
}

void UEngineGraphicDevice::CreateDeviceAndContext()
{
    MainAdapter = FindHighPerformanceAdapter(); // ���� ���� ������ �׷���ī�� ������ MainAdapter�� �����Ѵ�.

    int iFlag = 0;

#ifdef _DEBUG
    iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL ResultLevel;

    HRESULT Result = D3D11CreateDevice(
        MainAdapter.Get(),                          // ���õ� GPU ���
        D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,   // �ϵ���� ����̹� : GPU�� �����Ѵٸ� UNKNOWN ����
        nullptr,                                    // ����Ʈ���� ����̹� : ������
        iFlag,                                      // ����� �Ǵ� ��Ÿ �ɼ� �÷��� : ����� ��忡�� �����
        nullptr,                                    // DX 3D ��� ���� �迭 : �⺻ ����(nullptr)
        0,                                          // ��� ���� �迭 ũ�� : 0
        D3D11_SDK_VERSION,                          // SDK ���� : �׻� �� ������ ����ؾ� ��
        &Device,                                    // ������ Device ��ü
        &ResultLevel,                               // ����̽��� �����ϴ� DirectX ���� ��ȯ
        &Context);                                  // ������ DeviceContext ��ü

    if (nullptr == Device)
    {
        MSGASSERT("�׷��� ����̽� ������ �����߽��ϴ�.");
        return;
    }

    if (nullptr == Context)
    {
        MSGASSERT("�׷��� ����̽� ���ؽ�Ʈ ������ �����߽��ϴ�.");
        return;
    }

    if (Result != S_OK)
    {
        MSGASSERT("�׷��� ����̽� �� ���ؽ�Ʈ ���� �۾��� �����߽��ϴ�.");
        return;
    }

    if (ResultLevel != D3D_FEATURE_LEVEL_11_0 
        && ResultLevel != D3D_FEATURE_LEVEL_11_1)
    {
        MSGASSERT("�ش� �׷���ī��� DirectX 11 ������ �������� �ʴ� ��ġ�Դϴ�.");
        return;
    }

    // ��Ƽ�����带 ����ϰڴٴ� ���� �̸� ����
    Result = CoInitializeEx(nullptr, COINIT_MULTITHREADED); // ��Ƽ ������ ȯ��

    if (Result != S_OK)
    {
        MSGASSERT("������ ������ ���뿡 �����߽��ϴ�.");
        return;
    }
}

void UEngineGraphicDevice::InitializeSwapChain(const UEngineWindow& _Window)
{
    FVector Size = _Window.GetWindowSize(); // ������ ũ�⿡ �´� ����۸� ����� ���� ContentsCore���� ������ ���ð��� ������ ���� ����۸� �����.

    // ���� ü�� : GPU�� �������� ����� ȭ�鿡 ����ϱ� ���� ���Ǵ� ������� ����
    DXGI_SWAP_CHAIN_DESC ScInfo = {0}; // ���� ü�� �Ӽ��� �����ϴ� ����ü

    ScInfo.BufferCount = 2;                             // ����� ���� : 2(���� ���۸�)
    ScInfo.BufferDesc.Width = Size.iX();                // ������� ���� ũ�� 
    ScInfo.BufferDesc.Height = Size.iY();               // ������� ���� ũ��
    ScInfo.OutputWindow = _Window.GetWindowHandle();    // ���� ��� â �ڵ� 

    ScInfo.Windowed = true; // true : â���, false : ��üȭ��

    // �ֻ��� ����(Refresh Rate) : �ʴ� ȭ���� ���ŵǴ� ��
    ScInfo.BufferDesc.RefreshRate.Denominator = 1; // �и�
    ScInfo.BufferDesc.RefreshRate.Numerator = 60;  // ����

    ScInfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32��Ʈ ����, ��� ����ȭ ��(0.0 ~ 1.0)

    ScInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;   // �ȼ��� ���ŵǴ� ���� : ���� ���� �������
    ScInfo.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
 
    // ����� ���뵵            ���� Ÿ������ ���       ��   ���̴����� ���� ����
    ScInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;

    // ���ø�
    ScInfo.SampleDesc.Quality = 0;
    ScInfo.SampleDesc.Count = 1;

    ScInfo.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� ������ ������ ������ �� ���������� ��ü
    ScInfo.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // ��üȭ�� ���� â ��� �� ��ȯ ���


    IDXGIFactory* pF = nullptr;

    // ���� ����� ��ü�� ������� ���丮 ������ �ٽ� ������ pF�� �����Ѵ�.
    MainAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pF));

    pF->CreateSwapChain(Device.Get(), &ScInfo, &SwapChain); // ���� ü�� ����

    // ���丮�� ����ʹ� ���� ü���� ���������ν� ������ �� �ߴ�.
    pF->Release();
    MainAdapter->Release();

    if (nullptr == SwapChain)
    {
        MSGASSERT("����ü�� ������ �����߽��ϴ�.");
        return;
    }
}

void UEngineGraphicDevice::InitializeBackBufferRenderTargets()
{
    // DXBackBufferTexture�� BITMAP�̴�. 2���� �迭�� ���� ������ ����ִ� �ڵ�
    Microsoft::WRL::ComPtr<ID3D11Texture2D> DXBackBufferTexture = nullptr;
    if (S_OK != SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &DXBackBufferTexture))
    {
        MSGASSERT("����� �ؽ�ó�� �������� �� �����߽��ϴ�.");
        return;
    };

    // RTV : VRam�� �޸� ����� ���� ����
    BackBufferTarget = std::make_shared<UEngineRenderTarget>();
    BackBufferTarget->CreateRenderTargetView(DXBackBufferTexture);
    BackBufferTarget->CreateDepthTexture(); // DSV ���� 
}

void UEngineGraphicDevice::OMSetRenderTargetWithClear()
{
    BackBufferTarget->ClearRenderTargetView();
    BackBufferTarget->OMSetRenderTargets();
}

void UEngineGraphicDevice::Present()
{
    HRESULT Result = SwapChain->Present(0, 0); // ����ü���� �����ϴ� ����ۿ� ����Ʈ���۸� ��ȯ(Swap)
    //             ��������ȭ : 0(�̻��) / �߰��ɼ�(Flag) : 0(����)

    //             ����̽��� ������ ���� ����          ����̽��� ���µǾ������
    if (Result == DXGI_ERROR_DEVICE_REMOVED || Result == DXGI_ERROR_DEVICE_RESET)
    {
        MSGASSERT("�ػ� �����̳� ����̽� ���� ������ ��Ÿ�� ���� �����Ǿ����ϴ�");
        return;
    }
}