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
    // 1. 그래픽카드가 여러 개일 경우를 대비하여 가장 좋은 그래픽카드를 찾는다.
    IDXGIFactory* Factory = nullptr; // 팩토리 : 그래픽 카드 정보 열거, 출력 장치 관리

    HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));
    if (nullptr == Factory)
    {
        MSGASSERT("팩토리 객체 생성에 실패했습니다.");
        return nullptr;
    }

    unsigned __int64 MaxVideoMemory = 0; // 최고성능의 기준을 VRAM으로 정했다. 누가? 프로그래머가.
    IDXGIAdapter* ResultAdapter = nullptr;  // 가장 성능 좋은 그래픽카드 정보를 담을 객체

    // 그래픽카드를 열거하여 가장 성능이 좋은 그래픽카드를 선택하겠다.
    for (int Index = 0;; ++Index)
    {
        IDXGIAdapter* CurAdapter = nullptr;
        Factory->EnumAdapters(Index, &CurAdapter); // 그래픽카드를 순차적으로 열거

        if (nullptr == CurAdapter)
        {
            // 더이상 열거할 그래픽카드가 없으면 반복문 종료
            break; // 반복문 탈출
        }

        DXGI_ADAPTER_DESC Desc;
        CurAdapter->GetDesc(&Desc); // 그래픽카드 정보를 가져와 Desc에 저장

        if (MaxVideoMemory <= Desc.DedicatedVideoMemory) // 이전에 최고 성능 기준보다 높다면
        {
            MaxVideoMemory = Desc.DedicatedVideoMemory;  // VRAM 성능 기준치를 갱신하고
            if (nullptr != ResultAdapter) // 기존에 있던 어댑터는 해제하고
            {
                ResultAdapter->Release();
            }

            ResultAdapter = CurAdapter; // 지금 얻어온 그래픽카드 정보를 Result 어댑터에 저장한다.
            continue;
        }
        CurAdapter->Release(); // 최종 어댑터 정보를 가져왔으니 지역변수는 릴리즈해주고
    }
    // 최종 그래픽카드 정보를 가져와 어댑터 객체에 저장했다.

    if (nullptr != Factory)
    {
        Factory->Release(); // 그래픽카드 정보를 어댑터에 저장했으니 팩토리도 역할을 다했다.
    }

    if (nullptr == ResultAdapter)
    {
        MSGASSERT("그래픽카드가 존재하지 않는 PC입니다.");
        return nullptr;
    }

    return ResultAdapter;
}

void UEngineGraphicDevice::CreateDeviceAndContext()
{
    MainAdapter = FindHighPerformanceAdapter(); // 제일 좋은 성능의 그래픽카드 정보를 MainAdapter에 저장한다.

    int iFlag = 0;

#ifdef _DEBUG
    iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL ResultLevel;

    HRESULT Result = D3D11CreateDevice(
        MainAdapter.Get(),                          // 선택된 GPU 사용
        D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,   // 하드웨어 드라이버 : GPU를 지정한다면 UNKNOWN 선택
        nullptr,                                    // 소프트웨어 드라이버 : 사용안함
        iFlag,                                      // 디버깅 또는 기타 옵션 플래그 : 디버그 모드에선 디버그
        nullptr,                                    // DX 3D 기능 수준 배열 : 기본 수준(nullptr)
        0,                                          // 기능 수준 배열 크기 : 0
        D3D11_SDK_VERSION,                          // SDK 버전 : 항상 이 버전을 사용해야 함
        &Device,                                    // 생성된 Device 객체
        &ResultLevel,                               // 디바이스가 지원하는 DirectX 버전 반환
        &Context);                                  // 생성된 DeviceContext 객체

    if (nullptr == Device)
    {
        MSGASSERT("그래픽 디바이스 생성에 실패했습니다.");
        return;
    }

    if (nullptr == Context)
    {
        MSGASSERT("그래픽 디바이스 컨텍스트 생성에 실패했습니다.");
        return;
    }

    if (Result != S_OK)
    {
        MSGASSERT("그래픽 디바이스 및 컨텍스트 생성 작업에 실패했습니다.");
        return;
    }

    if (ResultLevel != D3D_FEATURE_LEVEL_11_0 
        && ResultLevel != D3D_FEATURE_LEVEL_11_1)
    {
        MSGASSERT("해당 그래픽카드는 DirectX 11 버전을 지원하지 않는 장치입니다.");
        return;
    }

    // 멀티스레드를 사용하겠다는 것을 미리 선언
    Result = CoInitializeEx(nullptr, COINIT_MULTITHREADED); // 멀티 스레드 환경

    if (Result != S_OK)
    {
        MSGASSERT("스레드 안정성 적용에 실패했습니다.");
        return;
    }
}

void UEngineGraphicDevice::InitializeSwapChain(const UEngineWindow& _Window)
{
    FVector Size = _Window.GetWindowSize(); // 윈도우 크기에 맞는 백버퍼를 만들기 위해 ContentsCore에서 윈도우 세팅값이 설정된 이후 백버퍼를 만든다.

    // 스왑 체인 : GPU가 렌더링한 결과를 화면에 출력하기 위해 사용되는 백버퍼의 집합
    DXGI_SWAP_CHAIN_DESC ScInfo = {0}; // 스왑 체인 속성을 설정하는 구조체

    ScInfo.BufferCount = 2;                             // 백버퍼 개수 : 2(더블 버퍼링)
    ScInfo.BufferDesc.Width = Size.iX();                // 백버퍼의 가로 크기 
    ScInfo.BufferDesc.Height = Size.iY();               // 백버퍼의 세로 크기
    ScInfo.OutputWindow = _Window.GetWindowHandle();    // 최종 출력 창 핸들 

    ScInfo.Windowed = true; // true : 창모드, false : 전체화면

    // 주사율 설정(Refresh Rate) : 초당 화면이 갱신되는 빈도
    ScInfo.BufferDesc.RefreshRate.Denominator = 1; // 분모
    ScInfo.BufferDesc.RefreshRate.Numerator = 60;  // 분자

    ScInfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32비트 색상값, 양수 정규화 값(0.0 ~ 1.0)

    ScInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;   // 픽셀이 갱신되는 순서 : 제일 빠른 방법으로
    ScInfo.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
 
    // 백버퍼 사용용도            렌더 타겟으로 사용       및   셰이더에서 접근 가능
    ScInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;

    // 샘플링
    ScInfo.SampleDesc.Quality = 0;
    ScInfo.SampleDesc.Count = 1;

    ScInfo.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 이전 프레임 데이터 버리고 새 프레임으로 대체
    ScInfo.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // 전체화면 모드와 창 모드 간 전환 허용


    IDXGIFactory* pF = nullptr;

    // 현재 어댑터 객체를 만들어준 팩토리 정보를 다시 가져와 pF에 저장한다.
    MainAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pF));

    pF->CreateSwapChain(Device.Get(), &ScInfo, &SwapChain); // 스왑 체인 생성

    // 팩토리와 어댑터는 스왑 체인을 생성함으로써 역할을 다 했다.
    pF->Release();
    MainAdapter->Release();

    if (nullptr == SwapChain)
    {
        MSGASSERT("스왑체인 생성에 실패했습니다.");
        return;
    }
}

void UEngineGraphicDevice::InitializeBackBufferRenderTargets()
{
    // DXBackBufferTexture는 BITMAP이다. 2차원 배열에 색상 정보가 들어있는 핸들
    Microsoft::WRL::ComPtr<ID3D11Texture2D> DXBackBufferTexture = nullptr;
    if (S_OK != SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &DXBackBufferTexture))
    {
        MSGASSERT("백버퍼 텍스처를 가져오는 데 실패했습니다.");
        return;
    };

    // RTV : VRam의 메모리 블록의 쓰기 권한
    BackBufferTarget = std::make_shared<UEngineRenderTarget>();
    BackBufferTarget->CreateRenderTargetView(DXBackBufferTexture);
    BackBufferTarget->CreateDepthTexture(); // DSV 생성 
}

void UEngineGraphicDevice::OMSetRenderTargetWithClear()
{
    BackBufferTarget->ClearRenderTargetView();
    BackBufferTarget->OMSetRenderTargets();
}

void UEngineGraphicDevice::Present()
{
    HRESULT Result = SwapChain->Present(0, 0); // 스왑체인이 관리하는 백버퍼와 프론트버퍼를 교환(Swap)
    //             수직동기화 : 0(미사용) / 추가옵션(Flag) : 0(없음)

    //             디바이스가 랜더링 도중 삭제          디바이스가 리셋되었을경우
    if (Result == DXGI_ERROR_DEVICE_REMOVED || Result == DXGI_ERROR_DEVICE_RESET)
    {
        MSGASSERT("해상도 변경이나 디바이스 관련 설정이 런타임 도중 수정되었습니다");
        return;
    }
}