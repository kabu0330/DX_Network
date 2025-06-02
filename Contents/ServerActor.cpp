#include "PreCompile.h"
#include "ServerActor.h"

AServerActor::AServerActor()
{
	InitActorData(this);
	
	std::shared_ptr<UDefaultSceneComponent> SceneComponent = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = SceneComponent;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>().get();
	BodyRenderer->SetupAttachment(RootComponent);

	CurFramePacketTime = 20.0f * 30.0f;
}

AServerActor::~AServerActor()
{
}

void AServerActor::BeginPlay()
{
	Super::BeginPlay();
}

void AServerActor::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

