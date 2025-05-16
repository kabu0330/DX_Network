#include "PreCompile.h"
#include "ContentsResource.h"
#include <EnginePlatform/EngineSound.h>
#include <EngineCore/EngineFont.h>
#include <EngineCore/EngineTexture.h>

void UContentsResource::LoadResource()
{
	{	// 1. 이미지 파일 로드
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}

		Dir.Append("Image");

		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineTexture::LoadTexture(FilePath);
		}
	}

	//{
	//	// 맵 리소스
	//	UEngineDirectory Dir;
	//	Dir.MoveParentToDirectory("ContentsResources");
	//	Dir.Append("MapObjectResources");

	//	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
	//	for (size_t i = 0; i < ImageFiles.size(); i++)
	//	{
	//		std::string FilePath = ImageFiles[i].GetPathToString();
	//		UEngineTexture::LoadTexture(FilePath);
	//	}
	//}

	{
		// 맵 리소스
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("MapData");

		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineTexture::LoadTexture(FilePath);
		}
	}	

	{	// 사운드 로드
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Sound");


		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".wav", ".mp3" });

		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineSound::LoadSound(FilePath);
		}
	}
}

void UContentsResource::LoadFont()
{
	{
		// 폰트
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("Font/TrajanPro-Regular.otf");
		std::string FilePath = Dir.GetPathToString();
		UEngineFont::LoadFont("TrajanPro-Regular", FilePath);
	}
	{
		// 폰트
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("Font/NotoSerifCJKsc-Regular.otf");
		std::string FilePath = Dir.GetPathToString();
		UEngineFont::LoadFont("NotoSerifCJKsc-Regular", FilePath);
	}
	{
		// 폰트
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("Font/Perpetua.ttf");
		std::string FilePath = Dir.GetPathToString();
		UEngineFont::LoadFont("Perpetua", FilePath);
	}
	{
		// 폰트
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("Font/TrajanPro-Bold.otf");
		std::string FilePath = Dir.GetPathToString();
		UEngineFont::LoadFont("TrajanPro-Bold", FilePath);
	}
}

void UContentsResource::LoadContentsResource(std::string_view _Path)
{
	std::string Path = _Path.data();
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory("ContentsResources"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}
	Dir.Append(Path);
	UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
}

void UContentsResource::LoadResourceDirectory()
{
	LoadResource(); // 최초 1회 리소스 폴더를 로드해야 한다.
	LoadFont();

	// Knight
	LoadContentsResource("Image/Knight/Idle");
	LoadContentsResource("Image/Knight/Run");
	LoadContentsResource("Image/Knight/Jump");
	LoadContentsResource("Image/Knight/RunToIdle");
	LoadContentsResource("Image/Knight/IdleToRun");
	LoadContentsResource("Image/Knight/Slash");
	LoadContentsResource("Image/Knight/SlashAlt");
	LoadContentsResource("Image/Knight/UpSlash");
	LoadContentsResource("Image/Knight/DownSlash");
	LoadContentsResource("Image/Knight/Stun");
	LoadContentsResource("Image/Knight/Death");
	LoadContentsResource("Image/Knight/DeathDamage");
	LoadContentsResource("Image/Knight/DeathHead");
	LoadContentsResource("Image/Knight/Airborn");
	LoadContentsResource("Image/Knight/Land");
	LoadContentsResource("Image/Knight/HardLand");
	LoadContentsResource("Image/Knight/LookDown");
	LoadContentsResource("Image/Knight/LookDownLoop");
	LoadContentsResource("Image/Knight/LookUp");
	LoadContentsResource("Image/Knight/LookUpLoop");
	LoadContentsResource("Image/Knight/Dash");
	LoadContentsResource("Image/Knight/Focus");
	LoadContentsResource("Image/Knight/FocusGet");
	LoadContentsResource("Image/Knight/FocusEnd");
	LoadContentsResource("Image/Knight/FireballAntic");
	LoadContentsResource("Image/Knight/FireballCast");

	// Knight Effect
	LoadContentsResource("Image/Effect/Knight/SlashEffect");
	LoadContentsResource("Image/Effect/Knight/UpSlashEffect");
	LoadContentsResource("Image/Effect/Knight/DownSlashEffect");
	LoadContentsResource("Image/Effect/Knight/NailHitEffect");
	LoadContentsResource("Image/Effect/Knight/FocusEffect");
	//LoadContentsResource("Image/Effect/Knight/FocusEffectEnd");
	LoadContentsResource("Image/Effect/Knight/BurstEffect");
	//LoadContentsResource("Image/Effect/Knight/Dash_effect");
	LoadContentsResource("Image/Effect/Knight/Dash_effect2");
	LoadContentsResource("Image/Effect/Knight/Fireball");
	LoadContentsResource("Image/Effect/Knight/FireballImpact");
	LoadContentsResource("Image/Effect/Knight/StunEffect01");
	LoadContentsResource("Image/Effect/Knight/StunEffect02");
	LoadContentsResource("Image/Effect/Knight/StunEffect03");
	LoadContentsResource("Image/Effect/Knight/Blast");
	LoadContentsResource("Image/Effect/Knight/SoulBurst");
	LoadContentsResource("Image/Effect/Knight/FireballWallImpact");

	// Monster Effect
	LoadContentsResource("Image/Effect/Monster/BossGroundWave");
	LoadContentsResource("Image/Effect/Monster/BossDropObject");
	LoadContentsResource("Image/Effect/Monster/ShotImpact");

	// UI
	LoadContentsResource("Image/UI/HealthIdle");
	LoadContentsResource("Image/UI/HealthRefill");
	LoadContentsResource("Image/UI/HealthBreak");
	LoadContentsResource("Image/UI/HUDFrame");
	LoadContentsResource("Image/UI/VSoulBurst");
	LoadContentsResource("Image/UI/V_3Quarter");
	LoadContentsResource("Image/UI/V_DownTo3Quarter");
	LoadContentsResource("Image/UI/V_DownToEmpty");
	LoadContentsResource("Image/UI/V_DownToHalf");
	LoadContentsResource("Image/UI/V_DownToQuarter");
	LoadContentsResource("Image/UI/V_Full");
	LoadContentsResource("Image/UI/V_Half");
	LoadContentsResource("Image/UI/V_Quarter");
	LoadContentsResource("Image/UI/V_UpTo3Quarter");
	LoadContentsResource("Image/UI/V_UpToHalf");
	LoadContentsResource("Image/UI/V_UpToQuarter");
	LoadContentsResource("Image/UI/fleurBot");
	LoadContentsResource("Image/UI/fleurTop");


}

void UContentsResource::LoadFolder()
{
	//UEngineDirectory TitleMain;
	//TitleMain.MoveParentToDirectory("ContentsResources//Image//Title");
	//TitleMain.Append("TitleBackGround");
}

void UContentsResource::LoadSprite()
{
	UEngineSprite::CreateSpriteToMeta("WanderingHusk.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("LeapingHusk.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("HuskHornhead.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("Crawlid.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("Vengefly.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("Gruzzer.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("AspidMother.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("AspidHatchling.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("AspidHunter.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("FalseKnight.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("FalseKnightHead.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("FalseKnightDeath.png", ".smeta");

	// Effect
	UEngineSprite::CreateSpriteToMeta("Explode.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("Puff.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("HitOrange.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("WhiteHit.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("BlackParticle.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("DefaultHitParticle.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("OrangeParticle.png", ".smeta");

	UEngineSprite::CreateSpriteToMeta("GroundImapctEffect.png", ".smeta");
	UEngineSprite::CreateSpriteToMeta("BulletFire.png", ".smeta");


	UEngineSprite::CreateSpriteToMeta("Barrier.png", ".smeta");

}

void UContentsResource::LoadShaderResource()
{
	// Shader
	{
		UEngineDirectory CurDir;
		CurDir.MoveParentToDirectory("ContentsShader");

		std::vector<UEngineFile> ShaderFiles = CurDir.GetAllFile(true, { ".fx", ".hlsl" });

		for (size_t i = 0; i < ShaderFiles.size(); i++)
		{
			UEngineShader::AutoCompileShaderByNaming(ShaderFiles[i]);
		}
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("MyMaterial");
		Mat->SetVertexShader("ContentsShader.fx");
		Mat->SetPixelShader("ContentsShader.fx");
	}

	{
		std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("MyCollisionDebugMaterial");
		Mat->SetVertexShader("EngineDebugCollisionShader.fx");
		Mat->SetPixelShader("EngineDebugCollisionShader.fx");
		// 언제나 화면에 나오는 누구도 이녀석의 앞을 가릴수 없어.
		Mat->SetDepthStencilState("CollisionDebugDepth");
		Mat->SetRasterizerState("CollisionDebugRas");
	}
}

UContentsResource::UContentsResource()
{
}

UContentsResource::~UContentsResource()
{
}