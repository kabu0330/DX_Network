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
    AllRenderTargetTextures.clear();
    AllRTVs.clear();
    AllSRVs.clear();
}

void UEngineRenderTarget::CreateRenderTargetView(float4 _Scale, float4 _ClearColor /*= float4::NONE*/, DXGI_FORMAT _Format /*= DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT*/)
{
    ClearColor = _ClearColor; // 처음에 화면을 지울 색상

    std::shared_ptr<class UEngineTexture> NewTexture = std::make_shared<UEngineTexture>();

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

    NewTexture->CreateTextureWithView(Desc);
    NewTexture->Size = _Scale;

    AllRTVs.push_back(NewTexture->GetRTV()); // 렌더타겟뷰를 관리할 자료구조에 넣는다.
    AllRenderTargetTextures.push_back(NewTexture);
}

void UEngineRenderTarget::CreateRenderTargetView(Microsoft::WRL::ComPtr<ID3D11Texture2D> _Texture2D)
{
	std::shared_ptr<class UEngineTexture> NewTexture = std::make_shared<UEngineTexture>();
	NewTexture->CreateRenderTargetView(_Texture2D);

    if (nullptr == NewTexture->GetRTV())
    {
        MSGASSERT("백버퍼의 렌더타겟 뷰가 존재하지 않습니다.");
        return;
    }

    AllRTVs.push_back(NewTexture->GetRTV());
    AllRenderTargetTextures.push_back(NewTexture);
}

// 몇번째 타겟의 사이즈로 깊이버퍼를 만들거냐
void UEngineRenderTarget::CreateDepthTexture(int _Index)
{
    if (AllRenderTargetTextures.size() <= _Index)
    {
        MSGASSERT("깊이버퍼를 생성할 텍스처 인덱스가 올바르지 않습니다.");
        return;
    }

    FVector Size = AllRenderTargetTextures[_Index]->GetTextureSize();
 
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
    DepthTexture->CreateTextureWithView(Desc);
}

void UEngineRenderTarget::ClearRenderTargets()
{
    // Clear RTV
    for (size_t i = 0; i < AllRTVs.size(); i++)
    {
        UEngineCore::GetDevice().GetContext()->ClearRenderTargetView(AllRTVs[i], ClearColor.Arr1D);
    }

    // Clear DSV
    if (nullptr == DepthTexture)
    {
        return;
    }  
    if (nullptr != DepthTexture->GetDSV())
    {
        UEngineCore::GetDevice().GetContext()->ClearDepthStencilView(DepthTexture->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }
}

void UEngineRenderTarget::OMSetRenderTargets()
{
    UEngineCore::GetDevice().GetContext()->OMSetRenderTargets(1, &AllRTVs[0], DepthTexture->GetDSV());
}

void UEngineRenderTarget::CopyTo(std::shared_ptr<UEngineRenderTarget> _Target)
{
    // 복사하기 전에는 지우고 병합한다.
    _Target->ClearRenderTargets(); 
    MergeTo(_Target); 
}

void UEngineRenderTarget::MergeTo(std::shared_ptr<UEngineRenderTarget> _Target)
{
    _Target->OMSetRenderTargets(); // 출력병합
    TargetUnit.BindTextureToShaderSlot("MergeTex", AllRenderTargetTextures[0]);  // 셰이더 슬롯에 연결
    TargetUnit.SetRenderingPipelineAndDraw(nullptr, 0.0f);
    TargetUnit.Reset();
}

void UEngineRenderTarget::ShowPostEffect(UEngineCamera* _Camera, float _DeltaTime)
{
    for (std::shared_ptr<UPostEffect>& Effect : PostEffects)
    {
        if (false == Effect->IsActive)
        {
            continue;
        }

        Effect->Effect(_Camera, _DeltaTime);
    }
}
