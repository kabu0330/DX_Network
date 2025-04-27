#pragma once
#include <EngineCore/HUD.h>
#include <vector>
#include <EnginePlatform/EngineSound.h>

// Ό³Έν :
class APlayHUD : public AHUD
{
public:
	// constrcuter destructer
	APlayHUD();
	~APlayHUD();

	// delete Function
	APlayHUD(const APlayHUD& _Other) = delete;
	APlayHUD(APlayHUD&& _Other) noexcept = delete;
	APlayHUD& operator=(const APlayHUD& _Other) = delete;
	APlayHUD& operator=(APlayHUD&& _Other) noexcept = delete;

	void FadeIn();
	void FadeOut(float _Time = 0.6f, float _Power = 1.0f);
	void SetActiveBossText(bool _Value);
	void SetActiveClimbText(bool _Value);
	void ActiveFalsePrompt();
	void ActiveDashPrompt();
	void ActiveFireballPrompt();
	void ActiveFocusPrompt();
	void ActiveJumpPrompt();
	void ActiveSlashPrompt();

	static APlayHUD* GetHUD()
	{
		if (nullptr == HUD)
		{
			return nullptr;
		}
		return HUD;
	}


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime)	override;

	void SetNextAnimationForSkillGauge();
	bool bIsNextAnimation = false;

	void NextAnimation(std::string_view _Name);
	std::string NextAnimName = "";

private:
	inline static APlayHUD* HUD = nullptr;

	FVector ScreenSize = FVector::ZERO;
	FVector HalfSize = FVector::ZERO;
	class AKnight* Knight = nullptr;
	UTimeEventComponent* TimeEventer = nullptr;
	
	void InitSkillGaugeFrame();
	void CreateSkillGaugeFrame();
	std::shared_ptr<class UImageWidget> SkillGaugeFrame = nullptr;
	float SkillGaugeFramePosX = 0.35f;
	float SkillGaugeFramePosY = 0.39f;
	bool bIsSkillGaugeFrame = false;

	void InitSkillGauge();
	void CreateSkillGauge();
	void UpdateSkillGauge();
	ESkillGauge SwitchToEnumSkillGauge(int _Value);
	class UImageWidget* SkillGauge = nullptr;
	int PrevKnightMp = 0;
	bool bIsPickupMp = false;

	void CreateSkillGaugeEffect();
	class UImageWidget* SkillGaugeEffect = nullptr;

	void InitHpFrame();
	std::vector<std::shared_ptr<class UImageWidget>> Hps;
	float HpFramePosX = 0.31f;
	float HpFramePosY = 0.39f;
	float HpFramePosXGap = 0.045f;
	bool bIsHpFrame = false;
	int KnightHp = 0;
	int InitKinghtHp = 0;

	void CheckKnightHp();
	int PrevKnightHp = 0;

	void ChangeHpUI();
	void UpdateHpUI();
	bool bIsHpPlus = false;
	bool bIsHpMinus = false;
	

	void CreateGeo();
	//void CreateGeoCount();
	void CreateBossText();
	std::shared_ptr<class UFontWidget> BossText = nullptr;

	void CreateClimbText();
	std::shared_ptr<class UFontWidget> ClimbText = nullptr;
	
	void CreateFleur();
	std::shared_ptr<class UImageWidget> TextTopFleur = nullptr;
	std::shared_ptr<class UImageWidget> TexBotFleur = nullptr;

	void CreatePrompt();
	void ActiveTruePrompt();
	std::shared_ptr<class UImageWidget> ImagePrompt = nullptr;
	std::shared_ptr<class UFontWidget> TextPrompt = nullptr;

	void CreateFade();
	void FadeChange();
	std::shared_ptr<class UImageWidget> Fade;
	float4 FadeValue = FVector::ZERO;
	float4 FadeDir = FVector::UNIT; // {1, 1, 1, 1}

	USoundPlayer Sound;
};

