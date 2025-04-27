#include "PreCompile.h"
#include "PlayHUD.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/ImageWidget.h>
#include <EngineCore/FontWidget.h>
#include <EngineCore/TimeEventComponent.h>
#include "Door.h"

APlayHUD::APlayHUD()
{
	HUD = this;
	ScreenSize = UEngineCore::GetScreenScale();
	HalfSize = ScreenSize * 0.5f;
	Knight = AKnight::GetPawn();

	TimeEventer = CreateDefaultSubObject<UTimeEventComponent>().get();
	CreateFade();
}

APlayHUD::~APlayHUD()
{
}

void APlayHUD::BeginPlay()
{
	AHUD::BeginPlay();

	InitSkillGaugeFrame();
	InitSkillGauge();
	InitKinghtHp = AKnight::GetPawn()->GetStatRef().GetMaxHp();
	CreateSkillGaugeEffect();

	CreateBossText();
	CreateFleur();
	CreateClimbText();
	CreatePrompt();
	//CreateGeo();
}

void APlayHUD::Tick(float _DeltaTime)
{
	AHUD::Tick(_DeltaTime);

	if (true == ADoor::IsDoorEnter())
	{
		FadeIn();
	}
	if (UEngineInput::IsDown('F'))
	{
		//FadeOut();
		FadeIn(); // 페이드 인에 페이드 아웃까지 같이 로직을 넣었다.
	}

	CheckKnightHp();
	InitHpFrame();
	UpdateHpUI();
	CreateSkillGauge();
	UpdateSkillGauge();
	SetNextAnimationForSkillGauge();
	if (true == SkillGaugeEffect->IsCurAnimationEnd())
	{
		SkillGaugeEffect->SetActive(false);
	}
}

void APlayHUD::SetNextAnimationForSkillGauge()
{
	if (false == bIsNextAnimation) // 트리거
	{
		return;
	}
	if ("" == NextAnimName)
	{
		return;
	}

	if (true == SkillGauge->IsCurAnimationEnd())
	{
		SkillGauge->ChangeAnimation(NextAnimName);

		bIsNextAnimation = false;
		NextAnimName = "";
		return;
	}
}

void APlayHUD::NextAnimation(std::string_view _Name)
{
	bIsNextAnimation = true; // Tick에서 현재 재생중인 애니메이션이 종료되었는지 검사해라.
	NextAnimName = _Name; // 현재 재생중인 애니메이션이 끝나면 이 이름의 애니메이션으로 바꿔라.
}

void APlayHUD::InitSkillGaugeFrame()
{
	SkillGaugeFrame = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK), "SkillGaugeFrame");
	std::string HUDFrame = "HUDFrame";
	SkillGaugeFrame->CreateAnimation(HUDFrame, HUDFrame, 0, 5, 0.2f, false);
	SkillGaugeFrame->ChangeAnimation(HUDFrame);

	SkillGaugeFrame->SetWorldLocation({ -ScreenSize.X * SkillGaugeFramePosX,  ScreenSize.Y * SkillGaugeFramePosY });

	SkillGaugeFrame->SetActive(false);
	SkillGaugeFrame->SetAutoScaleRatio(0.9f);


	// 게임 실행 이후 0.5초 뒤에 프레임 생성
	TimeEventer->AddEndEvent(0.5f, std::bind(&APlayHUD::CreateSkillGaugeFrame, this));
}

void APlayHUD::CreateSkillGaugeFrame()
{
	SkillGaugeFrame->SetActive(true);

	//SkillGaugeFrame->SetDownEvent([]()
	//	{
	//		UEngineDebug::OutPutString("Click");
	//	});
}

void APlayHUD::CheckKnightHp()
{
	int& KnightHpRef = Knight->GetStatRef().GetHpRef();
	KnightHp = KnightHpRef;
}

void APlayHUD::InitSkillGauge()
{
	SkillGauge = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK) + 10 , "SkillGauge").get();
	std::string V_Full = "V_Full";
	std::string V_UpTo3Quarter = "V_UpTo3Quarter";
	std::string V_DownTo3Quarter = "V_DownTo3Quarter";
	std::string V_3Quarter = "V_3Quarter";
	std::string V_UpToHalf = "V_UpToHalf";
	std::string V_DownToHalf = "V_DownToHalf";
	std::string V_Half = "V_Half";
	std::string V_UpToQuarter = "V_UpToQuarter";
	std::string V_DownToQuarter = "V_DownToQuarter";
	std::string V_Quarter = "V_Quarter";
	std::string V_DownToEmpty = "V_DownToEmpty";
	std::string V_Empty = "V_Empty";

	float FrameTime = 0.2f;
	float FastTime = 0.1f;

	SkillGauge->CreateAnimation(V_Full, V_Full, 0, 3, FastTime, false);

	SkillGauge->CreateAnimation(V_UpTo3Quarter, V_UpTo3Quarter, 0, 5, FastTime);
	SkillGauge->CreateAnimation(V_DownTo3Quarter, V_DownTo3Quarter, 0, 4, FastTime);
	SkillGauge->CreateAnimation(V_3Quarter, V_3Quarter, 0, 3, FrameTime);

	SkillGauge->CreateAnimation(V_UpToHalf, V_UpToHalf, 0, 5, FastTime);
	SkillGauge->CreateAnimation(V_DownToHalf, V_DownToHalf, 0, 5, FastTime);
	SkillGauge->CreateAnimation(V_Half, V_Half, 0, 3, FrameTime);

	SkillGauge->CreateAnimation(V_UpToQuarter, V_UpToQuarter, 0, 5, FastTime);
	SkillGauge->CreateAnimation(V_DownToQuarter, V_DownToQuarter, 0, 5, FastTime);
	SkillGauge->CreateAnimation(V_Quarter, V_Quarter, 0, 3, FrameTime);

	SkillGauge->CreateAnimation(V_DownToEmpty, V_DownToEmpty, 0, 2, FastTime);
	SkillGauge->CreateAnimation(V_Empty, V_DownToEmpty, 2, 2, FrameTime);

	SkillGauge->ChangeAnimation(V_Empty);

	SkillGauge->SetWorldLocation({ -ScreenSize.X * 0.388f,  ScreenSize.Y * 0.372f });
	SkillGauge->SetAutoScaleRatio(1.0f);
	SkillGauge->SetAutoScaleRatio(1.05f);
	SkillGauge->ColorData.MulColor = { 2.0f, 2.0f, 2.0f };
	SkillGauge->ColorData.MulColor.W = 0.5f;

	SkillGauge->SetActive(false);
}

void APlayHUD::CreateSkillGauge()
{
	if (false == bIsSkillGaugeFrame)
	{
		return;
	}
	if (true == SkillGaugeFrame->IsCurAnimationEnd())
	{
		if (false == SkillGauge->IsActive())
		{
			SkillGauge->SetActive(true);
		}
	}
}

void APlayHUD::UpdateSkillGauge()
{
	int KnightMp = AKnight::GetPawn()->GetStatRef().GetMp();
	if (KnightMp == PrevKnightMp)
	{
		return;
	}

	// 마나량을 5단계로 구분하고
	ESkillGauge CurGauge = SwitchToEnumSkillGauge(KnightMp);
	ESkillGauge PrevGauge = SwitchToEnumSkillGauge(PrevKnightMp);
	if (CurGauge == PrevGauge) // 단계가 같으면 리턴 
	{
		return;
	}

	int CurPhase = static_cast<int>(CurGauge);
	int PrevPhase = static_cast<int>(PrevGauge);
	int Result = CurPhase - PrevPhase;
	//              25          50     = -25 => 마나를 사용한 것 -> 마나 감소
	//              50          25     =  25 => 마나를 획득한 것 -> 마나 증가
	if (0 > Result) // 마나 감소
	{
		bIsPickupMp = false;
	}
	else // 마나 획득
	{
		bIsPickupMp = true;
	}

	PrevKnightMp = KnightMp; 
	
	std::string V_Full = "V_Full";
	std::string V_UpTo3Quarter = "V_UpTo3Quarter";
	std::string V_DownTo3Quarter = "V_DownTo3Quarter";
	std::string V_3Quarter = "V_3Quarter";
	std::string V_UpToHalf = "V_UpToHalf";
	std::string V_DownToHalf = "V_DownToHalf";
	std::string V_Half = "V_Half";
	std::string V_UpToQuarter = "V_UpToQuarter";
	std::string V_DownToQuarter = "V_DownToQuarter";
	std::string V_Quarter = "V_Quarter";
	std::string V_DownToEmpty = "V_DownToEmpty";
	std::string V_Empty = "V_Empty";

	// 1. 마나가 증가한 경우
	if (true == bIsPickupMp)
	{
		// 마나가 증가했는데 Empty인 경우는 없다.

		if (1 == CurPhase) // 마나가 0에서 25가 됐다면 쿼터
		{
			SkillGauge->ChangeAnimation(V_UpToQuarter); // 이 애니메이션 먼저 재생하고
			NextAnimation(V_Quarter); // 다음 애니메이션은 이걸로 재생해라.
			return;
		}
		else if (2 == CurPhase) // 마나가 25에서 50이 됐다면
		{
			SkillGauge->ChangeAnimation(V_UpToHalf);
			NextAnimation(V_Half);
			Sound = UEngineSound::Play("soul_pickup_1.wav");
			Sound.SetVolume(0.3f);
			return;
		}
		else if (3 == CurPhase) // 마나가 50에서 75이 됐다면
		{
			SkillGauge->ChangeAnimation(V_UpTo3Quarter);
			NextAnimation(V_3Quarter);
			return;
		}
		else if (4 == CurPhase)
		{
			SkillGauge->ChangeAnimation(V_Full);
			SkillGaugeEffect->SetActive(true);
			Sound = UEngineSound::Play("soul_pickup_1.wav");
			Sound.SetVolume(0.3f);
			return;
		}
	}
	else // 2. 마나가 감소한 경우
	{
		// 마나를 사용했는데 풀마나일 순 없다.

		if (3 == CurPhase) // 마나를 써서 98 ~ 75 사이가 남았다면
		{
			SkillGauge->ChangeAnimation(V_DownTo3Quarter);
			NextAnimation(V_3Quarter);
			return;
		}
		else if (2 == CurPhase)
		{
			SkillGauge->ChangeAnimation(V_DownToHalf);
			NextAnimation(V_Half);
			return;
		}
		else if (1 == CurPhase)
		{
			SkillGauge->ChangeAnimation(V_DownToQuarter);
			NextAnimation(V_Quarter);
			return;
		}
		else if (0 == CurPhase)
		{
			SkillGauge->ChangeAnimation(V_DownToEmpty);
			NextAnimation(V_Empty);
			return;
		}
	}
}

ESkillGauge APlayHUD::SwitchToEnumSkillGauge(int _Value)
{
	if (22 > _Value)
	{
		return ESkillGauge::EMPTY;
	}
	else if (55 > _Value)
	{
		return ESkillGauge::QUARTER;
	}
	else if (77 > _Value)
	{
		return ESkillGauge::HALF;
	}
	else if (99 > _Value)
	{
		return ESkillGauge::THREE_QUARTER;
	}
	else
	{
		return ESkillGauge::FULL;
	}
}

void APlayHUD::CreateSkillGaugeEffect()
{
	SkillGaugeEffect = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK) + 11, "SkillGaugeEffect").get();
	std::string VSoulBurst = "VSoulBurst";
	SkillGaugeEffect->CreateAnimation(VSoulBurst, VSoulBurst, 0, 5, 0.1f, false);
	SkillGaugeEffect->ChangeAnimation(VSoulBurst);

	SkillGaugeEffect->SetWorldLocation({ -ScreenSize.X * 0.388f,  ScreenSize.Y * 0.372f });
	SkillGaugeEffect->SetActive(false);
	SkillGaugeEffect->SetAutoScaleRatio(1.5f);
}

void APlayHUD::InitHpFrame()
{
	if (true == bIsHpFrame)
	{
		return;
	}
	if (true == SkillGaugeFrame->IsCurAnimationEnd())
	{
		bIsSkillGaugeFrame = true;
	}

	if (false == bIsSkillGaugeFrame)
	{
		return;
	}

	// 초기 이전 Hp 저장
	PrevKnightHp = InitKinghtHp;

	Hps.reserve(InitKinghtHp);
	for (int i = 0; i < InitKinghtHp; i++)
	{
		// 초기 세팅
		std::shared_ptr<UImageWidget> HpUI = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK) + 10, "HpUI");
		std::string HealthRefill = "HealthRefill";
		std::string HealthIdle = "HealthIdle";
		std::string HealthBreak = "HealthBreak";

		HpUI->CreateAnimation(HealthRefill, HealthRefill, 0, 6, 0.07f, false);
		HpUI->CreateAnimation(HealthIdle, HealthIdle, { 0, 1, 2, 3, 4 }, { 3.0f, 0.1f, 0.1f, 0.1f, 0.1f }, true);
		HpUI->CreateAnimation(HealthBreak, HealthBreak, 0, 6, 0.08f, false);

		HpUI->SetWorldLocation({ -ScreenSize.X * (HpFramePosX - (HpFramePosXGap * i)),  ScreenSize.Y * HpFramePosY });
		HpUI->SetAutoScale(true);
		HpUI->SetAutoScaleRatio(0.7f);
		HpUI->ChangeAnimation(HealthRefill);
		Hps.push_back(HpUI);
	}

	bIsHpFrame = true; // HP UI 생성 이후 접근 금지

	// 1초 뒤 체력 UI가 반짝거리도록 애니메이션 변경
	TimeEventer->AddEndEvent(1.0f, std::bind(&APlayHUD::ChangeHpUI, this));
}

void APlayHUD::ChangeHpUI()
{
	if (KnightHp != InitKinghtHp)
	{
		CheckKnightHp();
	}
	if (KnightHp == Hps.size() + 1)
	{
		return;
	}

	for (int i = 0; i < KnightHp; i++)
	{
		Hps[i]->ChangeAnimation("HealthIdle");
	}
}

void APlayHUD::UpdateHpUI()
{
	if (false == bIsHpFrame)
	{
		return;
	}
	if (PrevKnightHp == KnightHp) // 체력이 같으면 굳이 연산 금지
	{
		return;
	}

	// 지금 체력이 이전보다 커? 그러면 체력 회복
	KnightHp > PrevKnightHp ? bIsHpPlus = true : bIsHpMinus = true;

	if (true == bIsHpPlus) // 체력 회복
	{
		//     5      =      4     + 1
		if (KnightHp == Hps.size() + 1) 
		{
			return;
		}
		//           4           5
		for (; PrevKnightHp < KnightHp; ++PrevKnightHp)
		{
			//   vector[4]가 5번째 hp니까.
			Hps[PrevKnightHp]->ChangeAnimation("HealthRefill");
		}

		PrevKnightHp = KnightHp;
		bIsHpPlus = false;
		TimeEventer->AddEndEvent(1.0f, std::bind(&APlayHUD::ChangeHpUI, this));
	}
	else if (true == bIsHpMinus) // 체력 감소
	{
		if (0 > KnightHp)
		{
			return;
		}
		//         5              4
		for (; PrevKnightHp != KnightHp; --PrevKnightHp)
		{
			//  vector[4]가 5번째 hp니까.
			Hps[PrevKnightHp - 1]->ChangeAnimation("HealthBreak");
		}

		bIsHpMinus = false;
		PrevKnightHp = KnightHp;
	}
}

void APlayHUD::CreateGeo()
{
	std::shared_ptr<UImageWidget> GeoUI = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK), "GeoUI");
	float GeoPosX = SkillGaugeFramePosX - 0.04f;
	float GeoPosY = SkillGaugeFramePosY - 0.07f;
	GeoUI->SetWorldLocation({ -ScreenSize.X * GeoPosX,  ScreenSize.Y * GeoPosY });
	GeoUI->SetTexture("012-04-088.png", true, 0.9f);
}
\
void APlayHUD::CreateBossText()
{
	BossText = CreateWidget<UFontWidget>(static_cast<int>(EUIOrder::BACK), "BossText");
	BossText->SetWorldLocation({ ScreenSize.X * 0.17f,  ScreenSize.Y * - 0.35f });
	BossText->SetFont("NotoSerifCJKsc-Regular", 60.0f, UColor::WHITE);
	BossText->SetText("거짓된 기사");
	BossText->SetActive(false);
}

void APlayHUD::SetActiveBossText(bool _Value)
{
	BossText->SetActive(_Value);
}

void APlayHUD::CreateClimbText()
{
	ClimbText = CreateWidget<UFontWidget>(static_cast<int>(EUIOrder::BACK), "ClimbText");
	ClimbText->SetWorldLocation({ -ScreenSize.X * 0.035f,  ScreenSize.Y * 0.09f });
	ClimbText->SetFont("NotoSerifCJKsc-Regular", 23.0f, UColor::WHITE);
	ClimbText->SetText("올라가기");
	ClimbText->SetActive(false);
}

void APlayHUD::CreateFleur()
{
	std::string FleurTop = "fleurTop";
	std::string FleurBot = "fleurBot";
	float FrameTime = 0.05f;
	float PosX = 0.0f;
	float PosY = 0.1f;

	TextTopFleur = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK), "TextTopFleur");
	TextTopFleur->SetWorldLocation({ -ScreenSize.X * PosX,  ScreenSize.Y * PosY });
	TextTopFleur->CreateAnimation(FleurTop, FleurTop, 0, 5, FrameTime * 1.5f, false);
	TextTopFleur->CreateAnimation("NONE", FleurTop, 0, 0, FrameTime * 1.5f, false);
	TextTopFleur->SetAutoScaleRatio(0.2f);
	TextTopFleur->ChangeAnimation(FleurTop);

	TexBotFleur = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK), "TexBotFleur");
	TexBotFleur->SetWorldLocation({ -ScreenSize.X * PosX,  (ScreenSize.Y * PosY)  - 50.0f});
	TexBotFleur->CreateAnimation(FleurBot, FleurBot, 0, 8, FrameTime, false);
	TexBotFleur->CreateAnimation("NONE", FleurBot, 0, 0, FrameTime, false);
	TexBotFleur->SetAutoScaleRatio(0.4f);
	TexBotFleur->ChangeAnimation(FleurBot);

	TextTopFleur->SetActive(false);
	TexBotFleur->SetActive(false);
}

void APlayHUD::SetActiveClimbText(bool _Value)
{
	ClimbText->SetActive(_Value);
	TextTopFleur->SetActive(_Value);
	TexBotFleur->SetActive(_Value);

	if (true == _Value)
	{
		TextTopFleur->ChangeAnimation("FleurTop");
		TexBotFleur->ChangeAnimation("fleurBot");
	}
	if (false == _Value)
	{
		TextTopFleur->ChangeAnimation("NONE");
		TexBotFleur->ChangeAnimation("NONE");
	}
}

void APlayHUD::CreatePrompt()
{
	float PosX = 0.1f;
	float PosY = -0.4f;
	ImagePrompt = CreateWidget<UImageWidget>(static_cast<int>(EUIOrder::BACK), "ImagePrompt");
	ImagePrompt->SetWorldLocation({ -ScreenSize.X * PosX,  ScreenSize.Y * PosY });
	ImagePrompt->SetTexture("Dash_Prompt.png", true, 0.5f);
	ImagePrompt->SetAutoScaleRatio(0.5f);
	ImagePrompt->SetActive(false);

	TextPrompt = CreateWidget<UFontWidget>(static_cast<int>(EUIOrder::BACK), "TextPrompt");
	TextPrompt->SetWorldLocation({ ScreenSize.X * PosX,  ScreenSize.Y * PosY + 15.0f });
	TextPrompt->SetFont("NotoSerifCJKsc-Regular", 40.0f, UColor::WHITE);
	TextPrompt->SetText("C키");
	TextPrompt->SetActive(false);
}

void APlayHUD::ActiveFalsePrompt()
{
	ImagePrompt->SetActive(false);
	TextPrompt->SetActive(false);
}

void APlayHUD::ActiveTruePrompt()
{
	ImagePrompt->SetActive(true);
	TextPrompt->SetActive(true);
}

void APlayHUD::ActiveDashPrompt()
{
	ActiveTruePrompt();
	ImagePrompt->SetTexture("Dash_Prompt.png", true, 0.5f);
	TextPrompt->SetText("C키");
}

void APlayHUD::ActiveFireballPrompt()
{
	ActiveTruePrompt();
	ImagePrompt->SetTexture("Fireball_prompt.png", true, 0.5f);
	TextPrompt->SetText("A키");
}

void APlayHUD::ActiveFocusPrompt()
{
	ActiveTruePrompt();
	ImagePrompt->SetTexture("Focus_prompt_temp.png", true, 0.5f);
	TextPrompt->SetText("A키 꾸욱~");
}

void APlayHUD::ActiveJumpPrompt()
{
	ActiveTruePrompt();
	ImagePrompt->SetTexture("Wall_Jump_Prompt.png", true, 0.5f);
	TextPrompt->SetText("Z키");
}

void APlayHUD::ActiveSlashPrompt()
{
	ActiveTruePrompt();
	ImagePrompt->SetTexture("Slash_Prompt.png", true, 0.4f);
	TextPrompt->SetText("X키");
}

void APlayHUD::CreateFade()
{
	Fade = CreateWidget<UImageWidget>(static_cast<int>(100000000), "Fade");
	Fade->SetTexture("Fade.png", true, 1.0f);
	Fade->SetActive(true);
}

void APlayHUD::FadeOut(float _Time, float _Power)
{
	Fade->SetActive(true);
	FadeValue = FVector::ZERO;
	FadeDir = -FVector::UNIT;
	FadeDir.W = -_Power;

	TimeEventer->AddEvent(_Time, std::bind(&APlayHUD::FadeChange, this), [this]()
		{
			Fade->SetActive(false);
			Fade->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		});
}

void APlayHUD::FadeIn()
{
	Fade->SetActive(true);
	FadeValue = FVector::NONE;
	  FadeDir = FVector::ZERO;
	FadeDir.W = 2.0f;
	Fade->ColorData.MulColor.W = 0.0f;
	TimeEventer->AddEvent(0.6f, std::bind(&APlayHUD::FadeChange, this), [this]()
		{
			Fade->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
			FadeOut();
		});
}

void APlayHUD::FadeChange()
{
	UEngineDebug::OutPutString("Fade Change Mul : " + Fade->ColorData.MulColor.ToString());

	float DeltaTime = UEngineCore::GetDeltaTime();
	float Ratio = 1.5f;
	FVector FadeValueTest = FadeValue;
	FadeValue.W += FadeDir.W * DeltaTime * Ratio;

	Fade->ColorData.MulColor = FadeValue;

	if (0.0f >= Fade->ColorData.MulColor.W)
	{
		Fade->ColorData.MulColor.W = 0.0f;
		return;
	}
}

