#include "PreCompile.h"
#include "TitleHUD.h"
#include <EngineCore/ImageWidget.h>
#include <EngineCore/FontWidget.h>
#include <EnginePlatform/EngineInput.h>

ATitleHUD::ATitleHUD()
{
	CreateFade();
	CreateCreditsLogo();

	TimeEventer = CreateDefaultSubObject<UTimeEventComponent>().get();
}

ATitleHUD::~ATitleHUD()
{
}

void ATitleHUD::BeginPlay()
{
	AHUD::BeginPlay();

}

void ATitleHUD::Tick(float _DeltaTime)
{
	AHUD::Tick(_DeltaTime);
	if (UEngineInput::IsDown('F'))
	{
		CreditsLogo->ColorData.MulColor.W -= 0.1f;
		//CreditsFadeOut();
		int a = 0;
	}
}

void ATitleHUD::CreateFade()
{
	Fade = CreateWidget<UImageWidget>(static_cast<int>(100000000), "Fade");
	Fade->SetTexture("Fade.png", true, 1.0f);
	Fade->SetActive(true);
}

void ATitleHUD::CreateCreditsLogo()
{
	CreditsLogo = CreateWidget<UImageWidget>(static_cast<int>(110000000), "CreditLogo");
	CreditsLogo->SetTexture("creditslogo.png", true, 1.0f);
	CreditsLogo->SetActive(false);
	//CreditsLogo->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	//CreditsLogo->SetRelativeLocation({ ScreenSize.X * 0.3f, ScreenSize.Y * -0.27f , 999.0f });
}

void ATitleHUD::CreditsFadeIn()
{
	CreditsLogo->SetActive(true);
	CreditsFadeValue = FVector::NONE;
	CreditsFadeDir = FVector::ZERO;
	CreditsFadeDir.W = 0.5f;
	//CreditsLogo->ColorData.MulColor.W = 0.0f;
	TimeEventer->AddEvent(5.0f, std::bind(&ATitleHUD::CreditsFadeChange, this), [this]()
		{
			CreditsLogo->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		});
}

void ATitleHUD::CreditsFadeOut()
{
	CreditsLogo->SetActive(true);
	CreditsFadeValue = FVector::ZERO;
	CreditsFadeDir = -FVector::UNIT;
	CreditsFadeDir.W = -0.4f;
	//CreditsLogo->ColorData.MulColor.W = 0.0f;
	TimeEventer->AddEvent(5.0f, std::bind(&ATitleHUD::CreditsFadeChange, this), [this]()
		{
			CreditsLogo->SetActive(false);
			CreditsLogo->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		});
}

void ATitleHUD::CreditsFadeChange()
{
	UEngineDebug::OutPutString("Fade Change Mul : " + CreditsLogo->ColorData.MulColor.ToString());
	float DeltaTime = UEngineCore::GetDeltaTime();
	float Ratio = 1.0f;
	CreditsFadeValue.W += CreditsFadeDir.W * DeltaTime * Ratio;

	CreditsLogo->ColorData.MulColor.W = CreditsFadeValue.W;

	if (0.0f >= CreditsLogo->ColorData.MulColor.W)
	{
		CreditsLogo->ColorData.MulColor.W = 0.0f;
		return;
	}
	if (1.0f <= CreditsLogo->ColorData.MulColor.W)
	{
		CreditsLogo->ColorData.MulColor.W = 1.0f;
		return;
	}

}

void ATitleHUD::FadeIn()
{
	Fade->SetActive(true);
	FadeValue = FVector::NONE;
	FadeDir = FVector::ZERO;
	FadeDir.W = 0.5f;
	Fade->ColorData.MulColor.W = 0.0f;
	TimeEventer->AddEvent(6.0f, std::bind(&ATitleHUD::FadeChange, this), [this]()
		{
			Fade->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		});
}

void ATitleHUD::FadeInAndOut()
{
	Fade->SetActive(true);
	FadeValue = FVector::NONE;
	FadeDir = FVector::ZERO;
	FadeDir.W = 1.0f;
	Fade->ColorData.MulColor.W = 0.0f;
	TimeEventer->AddEvent(3.0f, std::bind(&ATitleHUD::FadeChange, this), [this]()
		{
			Fade->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			FadeOut();
		});
}

void ATitleHUD::FadeOut()
{
	Fade->SetActive(true);
	FadeValue = FVector::ZERO;
	FadeDir = -FVector::UNIT;
	FadeDir.W = -1.0f;

	// 2초간 FadeChange 함수 호출하고, 끝나면 Fade Active 끄고 MulColor도 원상복구한다.
	// UI가 다 MulColor 값을 공유하는듯 하다. -2.0f 넘어가면 다른 UI도 지워진다.
	TimeEventer->AddEvent(2.0f, std::bind(&ATitleHUD::FadeChange, this), [this]()
		{
			Fade->SetActive(false);
			Fade->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		});
}

void ATitleHUD::FadeChange()
{
	UEngineDebug::OutPutString("Fade Change Mul : " + Fade->ColorData.MulColor.ToString());

	float DeltaTime = UEngineCore::GetDeltaTime();
	float Ratio = 1.0f;
	FVector FadeValueTest = FadeValue;
	FadeValue.W += FadeDir.W * DeltaTime * Ratio;

	Fade->ColorData.MulColor = FadeValue;

	if (0.0f >= Fade->ColorData.MulColor.W)
	{
		Fade->ColorData.MulColor.W = 0.0f;
		return;
	}
	if (1.0f <= Fade->ColorData.MulColor.W)
	{
		Fade->ColorData.MulColor.W = 1.0f;
		return;
	}
}

