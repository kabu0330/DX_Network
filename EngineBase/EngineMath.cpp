#include "PreCompile.h"
#include "EngineMath.h"

//DirectX::BoundingSphere Sphere; // 구
//XMFLOAT3 Center;            // Center of the sphere.
//float Radius;               // Radius of the sphere.

//DirectX::BoundingBox AABB;
//XMFLOAT3 Center;            // Center of the box.
//XMFLOAT3 Extents;           // 센터기준 절반크기

//DirectX::BoundingOrientedBox OBB; // 
//XMFLOAT3 Center;            // Center of the box.
//XMFLOAT3 Extents;           // Distance from the center to each side.
//XMFLOAT4 Orientation;       // Unit quaternion representing rotation (box -> world).

// DirectX::BoundingFrustum Cam; // 카메라 

template<>
FQuat TVector<float>::DegAngleToQuaternion()
{
	FQuat Result;
	Result.DirectVector = DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectVector);
	return Result;
}

const FIntPoint FIntPoint::LEFT = { -1, 0 };
const FIntPoint FIntPoint::RIGHT = { 1, 0 };
const FIntPoint FIntPoint::UP = { 0, -1 };
const FIntPoint FIntPoint::DOWN = { 0, 1 };

FIntPoint FVector::ConvertToPoint() const
{
	return { iX(), iY() };
}

std::function<bool(const FTransform&, const FTransform&)> FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Max_CollisionType)][static_cast<int>(ECollisionType::Max_CollisionType)];

class CollisionFunctionInit
{
public:
	CollisionFunctionInit()
	{
		// 데이터 영역이 초기화 될때 초기화하는 일을 자동으로 수행할수 있다.
		// 데이터 영역이 만들어질때 이 작업은 자동으로 실행된다.
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Rect)][static_cast<int>(ECollisionType::Rect)] = FTransform::RectToRect;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::CirCle)][static_cast<int>(ECollisionType::CirCle)] = FTransform::CirCleToCirCle;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Rect)][static_cast<int>(ECollisionType::CirCle)] = FTransform::RectToCirCle;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::CirCle)][static_cast<int>(ECollisionType::Rect)] = FTransform::CirCleToRect;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB2D)][static_cast<int>(ECollisionType::OBB2D)] = FTransform::OBB2DToOBB2D;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB2D)][static_cast<int>(ECollisionType::Rect)] = FTransform::OBB2DToRect;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB2D)][static_cast<int>(ECollisionType::CirCle)] = FTransform::OBB2DToSphere;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::OBB2D)][static_cast<int>(ECollisionType::Point)] = FTransform::OBB2DToPoint;

	}
};

// 데이터 영역
CollisionFunctionInit Inst = CollisionFunctionInit();


FVector FQuat::QuaternionToEulerDeg() const
{
	return QuaternionToEulerRad() * UEngineMath::R2D;
}

FVector FQuat::QuaternionToEulerRad() const
{
	FVector result;

	float sinrCosp = 2.0f * (W * Z + X * Y);
	float cosrCosp = 1.0f - 2.0f * (Z * Z + X * X);
	result.Z = atan2f(sinrCosp, cosrCosp);

	float pitchTest = W * X - Y * Z;
	float asinThreshold = 0.4999995f;
	float sinp = 2.0f * pitchTest;

	if (pitchTest < -asinThreshold)
	{
		result.X = -(0.5f * UEngineMath::PI);
	}
	else if (pitchTest > asinThreshold)
	{
		result.X = (0.5f * UEngineMath::PI);
	}
	else
	{
		result.X = asinf(sinp);
	}

	// 이걸 사용했을대 
	float sinyCosp = 2.0f * (W * Y + X * Z);
	float cosyCosp = 1.0f - 2.0f * (X * X + Y * Y);
	result.Y = atan2f(sinyCosp, cosyCosp);

	// 0, 45 0 => 쿼터니온으로 바꾼다.
	// SetActorRoation(쿼터니온);
	// FQuat GetActorRoation();
	// FQuat => 각도 -180 , 275, - 180

	return result;
}

bool FTransform::Collision(ECollisionType _LeftType, const FTransform& _Left, ECollisionType _RightType, const FTransform& _Right)
{
	if (nullptr == FTransform::AllCollisionFunction[static_cast<int>(_LeftType)][static_cast<int>(_RightType)])
	{
		MSGASSERT("아직 구현하지 않은 충돌 타입간의 충돌 체크를 하려고 했습니다.");
		return false;
	}

	return FTransform::AllCollisionFunction[static_cast<int>(_LeftType)][static_cast<int>(_RightType)](_Left, _Right);
}

bool FTransform::PointToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	FTransform LeftTrans = _Left;
	LeftTrans.Scale = FVector::ZERO;
	return CirCleToCirCle(LeftTrans, _Right);
}

// 점 vs 사각형
bool FTransform::PointToRect(const FTransform& _Left, const FTransform& _Right)
{
	FTransform LeftTrans = _Left;
	LeftTrans.Scale = FVector::ZERO;
	return RectToRect(LeftTrans, _Right);
}

bool FTransform::CirCleToCirCle(const FTransform& _Left, const FTransform& _Right)
{

	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	return LeftCol.Sphere.Intersects(RightCol.Sphere);


	//FVector Len = _Left.Location - _Right.Location;

	//// 트랜스폼을 원으로 봤을때 반지름은 x의 절반크기를 반지름으로 보겠습니다.
	//// 두원의 반지름의 합이 벡터의 길이보다 크다면 
	//if (Len.Length() < _Left.Scale.hX() + _Right.Scale.hX())
	//{
	//	return true;
	//}

	//return false;
}

bool FTransform::RectToRect(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	return LeftCol.AABB.Intersects(RightCol.AABB);


	//if (_Left.ZAxisCenterLeft() > _Right.ZAxisCenterRight())
	//{
	//	return false;
	//}

	//if (_Left.ZAxisCenterRight() < _Right.ZAxisCenterLeft())
	//{
	//	return false;
	//}

	//if (_Left.ZAxisCenterTop() > _Right.ZAxisCenterBottom())
	//{
	//	return false;
	//}

	//if (_Left.ZAxisCenterBottom() < _Right.ZAxisCenterTop())
	//{
	//	return false;
	//}
	// 공식 만들면 된다.
	// return true;
}

bool FTransform::RectToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	return CirCleToRect(_Right, _Left);
}



bool FTransform::CirCleToRect(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	return LeftCol.Sphere.Intersects(RightCol.AABB);


	//// 좌우로 반지름 확장한 트랜스폼
	//FTransform WTransform = _Right;
	//WTransform.Scale.X += _Left.Scale.X;

	//// 위아래로 반지름 만큼 확장한 트랜스폼
	//FTransform HTransform = _Right;
	//HTransform.Scale.Y += _Left.Scale.X;

	//if (true == PointToRect(_Left, WTransform) || true == PointToRect(_Left, HTransform))
	//{
	//	return true;
	//}

	//// 비용 절약을 위해서 static으로 만드는 방법도 있는데.
	//// static FVector ArrPoint[4];
	//// 쓰레드에서는 못쓴다.
	//FVector ArrPoint[4];

	//ArrPoint[0] = _Right.ZAxisCenterLeftTop();
	//ArrPoint[1] = _Right.ZAxisCenterLeftBottom();
	//ArrPoint[2] = _Right.ZAxisCenterRightTop();
	//ArrPoint[3] = _Right.ZAxisCenterRightBottom();

	//FTransform PointCirCle;
	//PointCirCle.Scale = _Left.Scale;
	//for (size_t i = 0; i < 4; i++)
	//{
	//	PointCirCle.Location = ArrPoint[i];
	//	if (true == PointToCirCle(_Left, PointCirCle))
	//	{
	//		return true;
	//	}
	//}

	//return false;
}

bool FTransform::OBB2DToOBB2D(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	return LeftCol.OBB.Intersects(RightCol.OBB);
}

bool FTransform::OBB2DToRect(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	return LeftCol.OBB.Intersects(RightCol.AABB);
}

bool FTransform::OBB2DToSphere(const FTransform& _Left, const FTransform& _Right)
{
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	return LeftCol.OBB.Intersects(RightCol.Sphere);
}

bool FTransform::OBB2DToPoint(const FTransform& _Left, const FTransform& _Right)
{
	// ??
	FCollisionData LeftCol = _Left.GetCollisionData();
	FCollisionData RightCol = _Right.GetCollisionData();
	LeftCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Center.z = 0.0f;
	RightCol.OBB.Extents = {0.0f, 0.0f, 0.0f};
	return LeftCol.OBB.Intersects(RightCol.AABB);
}

FVector FVector::Transform(const FVector& _Vector, const class FMatrix& _Matrix)
{
	return _Vector * _Matrix;
}

FVector FVector::TransformCoord(const FVector& _Vector, const class FMatrix& _Matrix)
{
	FVector Copy = _Vector;
	Copy.W = 1.0f;
	return Copy * _Matrix;
}

FVector FVector::TransformNormal(const FVector& _Vector, const class FMatrix& _Matrix)
{
	FVector Copy = _Vector;
	Copy.W = 0.0f;
	return Copy * _Matrix;
}

template<>
FVector FVector::operator*(const class FMatrix& _Matrix) const
{
	FVector Result;
	// 나머지 완성하고 곱해서 결과 확인해보세요.

	// x y z w가 다 곱해져야 한다.
	Result.X = Arr2D[0][0] * _Matrix.Arr2D[0][0] + Arr2D[0][1] * _Matrix.Arr2D[1][0] + Arr2D[0][2] * _Matrix.Arr2D[2][0] + Arr2D[0][3] * _Matrix.Arr2D[3][0];
	Result.Y = Arr2D[0][0] * _Matrix.Arr2D[0][1] + Arr2D[0][1] * _Matrix.Arr2D[1][1] + Arr2D[0][2] * _Matrix.Arr2D[2][1] + Arr2D[0][3] * _Matrix.Arr2D[3][1];
	Result.Z = Arr2D[0][0] * _Matrix.Arr2D[0][2] + Arr2D[0][1] * _Matrix.Arr2D[1][2] + Arr2D[0][2] * _Matrix.Arr2D[2][2] + Arr2D[0][3] * _Matrix.Arr2D[3][2];
	Result.W = Arr2D[0][0] * _Matrix.Arr2D[0][3] + Arr2D[0][1] * _Matrix.Arr2D[1][3] + Arr2D[0][2] * _Matrix.Arr2D[2][3] + Arr2D[0][3] * _Matrix.Arr2D[3][3];


	//std::cout << "X : " << Arr2D[0][0] << "*" << _Matrix.Arr2D[0][0] << "+" << Arr2D[0][1] << "*" << _Matrix.Arr2D[1][0] << "+" << Arr2D[0][2] << "*" << _Matrix.Arr2D[2][0] << "+" << Arr2D[0][3] << "*" << _Matrix.Arr2D[3][0] << std::endl;
	//std::cout << "Y : " << Arr2D[0][0] << "*" << _Matrix.Arr2D[0][1] << "+" << Arr2D[0][1] << "*" << _Matrix.Arr2D[1][1] << "+" << Arr2D[0][2] << "*" << _Matrix.Arr2D[2][1] << "+" << Arr2D[0][3] << "*" << _Matrix.Arr2D[3][1] << std::endl;
	//std::cout << "Z : " << Arr2D[0][0] << "*" << _Matrix.Arr2D[0][2] << "+" << Arr2D[0][1] << "*" << _Matrix.Arr2D[1][2] << "+" << Arr2D[0][2] << "*" << _Matrix.Arr2D[2][2] << "+" << Arr2D[0][3] << "*" << _Matrix.Arr2D[3][2] << std::endl;
	//std::cout << "W : " << Arr2D[0][0] << "*" << _Matrix.Arr2D[0][3] << "+" << Arr2D[0][1] << "*" << _Matrix.Arr2D[1][3] << "+" << Arr2D[0][2] << "*" << _Matrix.Arr2D[2][3] << "+" << Arr2D[0][3] << "*" << _Matrix.Arr2D[3][3] << std::endl;

	//std::cout << Result.ToString() << std::endl;


	return Result;
}

FVector& FVector::operator*=(const FMatrix& _Matrix)
{
	DirectVector = DirectX::XMVector4Transform(DirectVector, _Matrix.DirectMatrix);
	return *this;
}

FMatrix FMatrix::operator*(const FMatrix& _Matrix)
{
	FMatrix Result;
	Result.DirectMatrix = DirectX::XMMatrixMultiply(DirectMatrix, _Matrix.DirectMatrix);
	return Result;

}

ENGINEAPI void FTransform::Decompose()
{
	World.Decompose(WorldScale, WorldQuat, WorldLocation);

	LocalWorld.Decompose(RelativeScale, RelativeQuat, RelativeLocation);

	//Scale = RelativeScale;
	//Quat = RelativeQuat;
	//Rotation = RelativeQuat.QuaternionToEulerDeg();
	//Location = RelativeLocation;
}

void FTransform::TransformUpdate(bool _IsAbsolut /*= false*/)
{
	// 10000 10000 10000

	// 절대로
	// 100 100 100
	
	
	// world인지 local
	ScaleMat.Scale(Scale);
	RotationMat.RotationDeg(Rotation);
	LocationMat.Position(Location);

	FMatrix CheckWorld = ScaleMat * RotationMat * LocationMat;

	// 절대로 지켜져야하는 최종 값이 되어야 한다.
	if (true == _IsAbsolut)
	{
		World = CheckWorld;
		LocalWorld = CheckWorld * ParentMat.InverseReturn();
		// LocalWorld 나의 로컬값이라는 것.
	}
	else 
	{
		//      크         자             이            공           부
		LocalWorld = CheckWorld;
		World = CheckWorld * RevolveMat * ParentMat;
		// 나의 로컬은 알지만 부모가 아직 안곱해져서 부모를 굽해서 나의 월드 값을 찾아낸다.

		// World.ArrVector[3]; => 
	}

	// 크자이 값을 역으로 다시 꺼내려고 하는 것.
	Decompose();



}

