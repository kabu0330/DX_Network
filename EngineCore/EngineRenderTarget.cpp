#include "PreCompile.h"
#include "EngineRenderTarget.h"

UEngineRenderTarget::UEngineRenderTarget()
{
    // 포스트 프로세싱용
    TargetUnit.SetMesh("FullRect"); // 화면 전체를 가리는 메시와
    TargetUnit.SetMaterial("TargetMerge"); // 언제나 화면 제일 앞에 나오는 머티리얼
}

UEngineRenderTarget::~UEngineRenderTarget()
{
}

void UEngineRenderTarget::CreateTarget(float4 _Scale, float4 _ClearColor /*= float4::NONE*/, DXGI_FORMAT _Format /*= DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT*/)
{
    ClearColor = _ClearColor; // 처음에 화면을 지울 색상

    std::shared_ptr<class UEngineTexture> NewTarget = std::make_shared<UEngineTexture>();

    D3D11_TEXTURE2D_DESC Desc = { 0 };
    Desc.ArraySize = 1;
    Desc.Width = _Scale.iX();
    Desc.Height = _Scale.iY();
    Desc.Format = _Format;

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    Desc.MipLevels = 1;
    Desc.Usage = D3D11_USAGE_DEFAULT; // 그래픽카드가 사용할거야
    Desc.CPUAccessFlags = 0;          // CPU는 접근하지 마.
    
    //                                   렌더 타겟이면서                        셰이더도 사용한다.
    Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

    NewTarget->CreateViewObject(Desc); // 텍스처만들고 DSV도 만들고
    NewTarget->Size = _Scale;

    ArrRTVs.push_back(NewTarget->GetRTV()); // 렌더타겟뷰를 관리할 자료구조에 넣는다.
    ArrTexture.push_back(NewTarget);
}

// 백버퍼용 렌더타겟뷰 생성 함수
void UEngineRenderTarget::CreateTarget(Microsoft::WRL::ComPtr<ID3D11Texture2D> _Texture2D)
{
	std::shared_ptr<class UEngineTexture> NewTarget = std::make_shared<UEngineTexture>();
	NewTarget->CreateViewObject(_Texture2D);

    if (nullptr == NewTarget->GetRTV())
    {
        MSGASSERT("백버퍼의 렌더타겟 뷰가 존재하지 않습니다.");
        return;
    }

    ArrRTVs.push_back(NewTarget->GetRTV());

    ArrTexture.push_back(NewTarget);
}

// 몇번째 타겟의 사이즈로 깊이버퍼를 만들거냐
void UEngineRenderTarget::CreateDepth(int _Index)
{
    if (ArrTexture.size() <= _Index)
    {
        MSGASSERT("깊이버퍼를 생성할 텍스처 인덱스가 올바르지 않습니다.");
        return;
    }

    FVector Size = ArrTexture[_Index]->GetTextureSize();
 
    D3D11_TEXTURE2D_DESC Desc = { 0 };
    Desc.ArraySize = 1;
    Desc.Width = Size.iX();
    Desc.Height = Size.iY();
    Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 3바이트 실수(깊이), 1바이트 정수(스텐실)

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    Desc.MipLevels = 1;
    Desc.Usage = D3D11_USAGE_DEFAULT; // 그래픽카드만 사용할거야
    Desc.CPUAccessFlags = 0;          // CPU는 접근하지 마.
    Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL; // 이 구조체의 용도는 뎁스 스텐실

    DepthTexture = std::make_shared<UEngineTexture>();
    DepthTexture->CreateViewObject(Desc);
}

// 먼저 화면을 지우고 난 뒤에 그린다.
void UEngineRenderTarget::Clear()
{
    for (size_t i = 0; i < ArrRTVs.size(); i++)
    {
        // 내가 정한 ClearColor로 모든 RTV를 지운다.
        UEngineCore::GetDevice().GetContext()->ClearRenderTargetView(ArrRTVs[i], ClearColor.Arr1D);
    }
    UEngineCore::GetDevice().GetContext()->ClearDepthStencilView(DepthTexture->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void UEngineRenderTarget::Setting()
{
    // 렌더 타겟을 세팅한다. 배열에 렌더 타겟뷰가 여러 개 있으면 멀티렌더타겟, 하나만 있으면 싱글 렌더 타겟
    UEngineCore::GetDevice().GetContext()->OMSetRenderTargets(1, &ArrRTVs[0], DepthTexture->GetDSV());
}

// 최종 렌더타겟에게까지 복사되어야 한다.
void UEngineRenderTarget::CopyTo(std::shared_ptr<UEngineRenderTarget> _Target)
{
    _Target->Clear(); // 복사하기 전에는 지우고
    MergeTo(_Target); // 병합한다.
}

void UEngineRenderTarget::MergeTo(std::shared_ptr<UEngineRenderTarget> _Target)
{
    _Target->Setting(); // 출력병합 OMSetRenderTargets
    TargetUnit.SetTexture("MergeTex", ArrTexture[0]); // 상수버퍼 세팅
    TargetUnit.Render(nullptr, 0.0f); // 렌더링 파이프라인으로
    TargetUnit.Reset();
}