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
}

void UEngineRenderTarget::CreateTarget(float4 _Scale, float4 _ClearColor /*= float4::NONE*/, DXGI_FORMAT _Format /*= DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT*/)
{
    ClearColor = _ClearColor; // ó���� ȭ���� ���� ����

    std::shared_ptr<class UEngineTexture> NewTarget = std::make_shared<UEngineTexture>();

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

    NewTarget->CreateViewObject(Desc); // �ؽ�ó����� DSV�� �����
    NewTarget->Size = _Scale;

    ArrRTVs.push_back(NewTarget->GetRTV()); // ����Ÿ�ٺ並 ������ �ڷᱸ���� �ִ´�.
    ArrTexture.push_back(NewTarget);
}

// ����ۿ� ����Ÿ�ٺ� ���� �Լ�
void UEngineRenderTarget::CreateTarget(Microsoft::WRL::ComPtr<ID3D11Texture2D> _Texture2D)
{
	std::shared_ptr<class UEngineTexture> NewTarget = std::make_shared<UEngineTexture>();
	NewTarget->CreateViewObject(_Texture2D);

    if (nullptr == NewTarget->GetRTV())
    {
        MSGASSERT("������� ����Ÿ�� �䰡 �������� �ʽ��ϴ�.");
        return;
    }

    ArrRTVs.push_back(NewTarget->GetRTV());

    ArrTexture.push_back(NewTarget);
}

// ���° Ÿ���� ������� ���̹��۸� ����ų�
void UEngineRenderTarget::CreateDepth(int _Index)
{
    if (ArrTexture.size() <= _Index)
    {
        MSGASSERT("���̹��۸� ������ �ؽ�ó �ε����� �ùٸ��� �ʽ��ϴ�.");
        return;
    }

    FVector Size = ArrTexture[_Index]->GetTextureSize();
 
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
    DepthTexture->CreateViewObject(Desc);
}

// ���� ȭ���� ����� �� �ڿ� �׸���.
void UEngineRenderTarget::Clear()
{
    for (size_t i = 0; i < ArrRTVs.size(); i++)
    {
        // ���� ���� ClearColor�� ��� RTV�� �����.
        UEngineCore::GetDevice().GetContext()->ClearRenderTargetView(ArrRTVs[i], ClearColor.Arr1D);
    }
    UEngineCore::GetDevice().GetContext()->ClearDepthStencilView(DepthTexture->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void UEngineRenderTarget::Setting()
{
    // ���� Ÿ���� �����Ѵ�. �迭�� ���� Ÿ�ٺ䰡 ���� �� ������ ��Ƽ����Ÿ��, �ϳ��� ������ �̱� ���� Ÿ��
    UEngineCore::GetDevice().GetContext()->OMSetRenderTargets(1, &ArrRTVs[0], DepthTexture->GetDSV());
}

// ���� ����Ÿ�ٿ��Ա��� ����Ǿ�� �Ѵ�.
void UEngineRenderTarget::CopyTo(std::shared_ptr<UEngineRenderTarget> _Target)
{
    _Target->Clear(); // �����ϱ� ������ �����
    MergeTo(_Target); // �����Ѵ�.
}

void UEngineRenderTarget::MergeTo(std::shared_ptr<UEngineRenderTarget> _Target)
{
    _Target->Setting(); // ��º��� OMSetRenderTargets
    TargetUnit.SetTexture("MergeTex", ArrTexture[0]); // ������� ����
    TargetUnit.Render(nullptr, 0.0f); // ������ ��������������
    TargetUnit.Reset();
}