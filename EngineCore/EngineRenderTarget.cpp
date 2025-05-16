#include "PreCompile.h"
#include "EngineRenderTarget.h"

UEngineRenderTarget::UEngineRenderTarget()
{
    // ����Ʈ ���μ��̿�
    TargetUnit.SetMesh("FullRect"); // ȭ�� ��ü�� ������ �޽ÿ�
    TargetUnit.SetMaterial("TargetMerge"); // ������ ȭ�� ���� �տ� ������ ��Ƽ����
}

UEngineRenderTarget::~UEngineRenderTarget()
{
    AllRenderTargetTextures.clear();
    AllRTVs.clear();
    AllSRVs.clear();
}

void UEngineRenderTarget::CreateRenderTargetView(float4 _Scale, float4 _ClearColor /*= float4::NONE*/, DXGI_FORMAT _Format /*= DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT*/)
{
    ClearColor = _ClearColor; // ó���� ȭ���� ���� ����

    std::shared_ptr<class UEngineTexture> NewTexture = std::make_shared<UEngineTexture>();

    D3D11_TEXTURE2D_DESC Desc = { 0 };
    Desc.ArraySize = 1;
    Desc.Width = _Scale.iX();
    Desc.Height = _Scale.iY();
    Desc.Format = _Format;

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    Desc.MipLevels = 1;
    Desc.Usage = D3D11_USAGE_DEFAULT; // �׷���ī�尡 ����Ұž�
    Desc.CPUAccessFlags = 0;          // CPU�� �������� ��.
    
    //                                   ���� Ÿ���̸鼭                        ���̴��� ����Ѵ�.
    Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

    NewTexture->CreateTextureWithView(Desc);
    NewTexture->Size = _Scale;

    AllRTVs.push_back(NewTexture->GetRTV()); // ����Ÿ�ٺ並 ������ �ڷᱸ���� �ִ´�.
    AllRenderTargetTextures.push_back(NewTexture);
}

void UEngineRenderTarget::CreateRenderTargetView(Microsoft::WRL::ComPtr<ID3D11Texture2D> _Texture2D)
{
	std::shared_ptr<class UEngineTexture> NewTexture = std::make_shared<UEngineTexture>();
	NewTexture->CreateRenderTargetView(_Texture2D);

    if (nullptr == NewTexture->GetRTV())
    {
        MSGASSERT("������� ����Ÿ�� �䰡 �������� �ʽ��ϴ�.");
        return;
    }

    AllRTVs.push_back(NewTexture->GetRTV());
    AllRenderTargetTextures.push_back(NewTexture);
}

// ���° Ÿ���� ������� ���̹��۸� ����ų�
void UEngineRenderTarget::CreateDepthTexture(int _Index)
{
    if (AllRenderTargetTextures.size() <= _Index)
    {
        MSGASSERT("���̹��۸� ������ �ؽ�ó �ε����� �ùٸ��� �ʽ��ϴ�.");
        return;
    }

    FVector Size = AllRenderTargetTextures[_Index]->GetTextureSize();
 
    D3D11_TEXTURE2D_DESC Desc = { 0 };
    Desc.ArraySize = 1;
    Desc.Width = Size.iX();
    Desc.Height = Size.iY();
    Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 3����Ʈ �Ǽ�(����), 1����Ʈ ����(���ٽ�)

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    Desc.MipLevels = 1;
    Desc.Usage = D3D11_USAGE_DEFAULT; // �׷���ī�常 ����Ұž�
    Desc.CPUAccessFlags = 0;          // CPU�� �������� ��.
    Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL; // �� ����ü�� �뵵�� ���� ���ٽ�

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
    // �����ϱ� ������ ����� �����Ѵ�.
    _Target->ClearRenderTargets(); 
    MergeTo(_Target); 
}

void UEngineRenderTarget::MergeTo(std::shared_ptr<UEngineRenderTarget> _Target)
{
    _Target->OMSetRenderTargets(); // ��º���
    TargetUnit.BindTextureToShaderSlot("MergeTex", AllRenderTargetTextures[0]);  // ���̴� ���Կ� ����
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
