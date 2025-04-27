#pragma once
#include <Windows.h>
#include <string>
#include <functional>
#include <format> 
#include <locale>

#include <DirectXMath.h>
#include <DirectXCollision.h>

#include "EngineDefine.h"

class ENGINEAPI  UEngineMath
{
public:
	// 상수 정의
	static inline const double DPI = 3.14159265358979323846264338327950288419716939937510;
	static inline const double DPI2 = DPI * 2.0;

	static inline const float PI = 3.14159265358979323846264f;
	static inline const float PI2 = PI * 2.0f;

	static inline const float D2R = UEngineMath::PI / 180.0f;
	static inline const float R2D = 180.0f / UEngineMath::PI;

	static float Sqrt(float _Value)
	{
		return ::sqrtf(_Value);
	}

	template <typename DataType>
	static DataType ClampMax(DataType value, DataType maxValue)
	{
		return (value > maxValue) ? maxValue : value;
	}

	template <typename DataType>
	static DataType ClampMin(DataType value, DataType minValue)
	{
		return (value < minValue) ? minValue : value;
	}

	template <typename DataType>
	static DataType Clamp(DataType value, DataType minValue, DataType maxValue)
	{
		if (value < minValue)
			return minValue;
		else if (value > maxValue)
			return maxValue;
		else
			return value;
	}

	template <typename DataType>
	static DataType Lerp(DataType A, DataType B, DataType Alpha)
	{
		return A * (1 - Alpha) + B * Alpha;
	}

	template <typename DataType>
	static DataType Lerp(DataType A, DataType B, float Alpha)
	{
		return A * (1.0f - Alpha) + B * Alpha;
	}

	static float Lerp(float A, float B, float Alpha)
	{
		return A * (1.0f - Alpha) + B * Alpha;
	}
};

template<typename ValueType>
class TVector
{
public:
	static const TVector NONE;
	static const TVector ZERO;
	static const TVector LEFT;
	static const TVector RIGHT;
	static const TVector UP;
	static const TVector DOWN;
	static const TVector FORWARD;
	static const TVector BACK;
	static const TVector UNIT;

public:
	union
	{
		struct
		{
			ValueType X;
			ValueType Y;
			ValueType Z;
			ValueType W;
		};

		ValueType Arr2D[1][4];
		ValueType Arr1D[4];
		DirectX::XMFLOAT3 DirectFloat3;
		DirectX::XMFLOAT4 DirectFloat4;
		// 다이렉트 simd 연산 전용 벡터
		DirectX::XMVECTOR DirectVector;
	};

	ENGINEAPI TVector()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)
	{
	}

	ENGINEAPI TVector(DirectX::XMVECTOR _DirectVector) : DirectVector(_DirectVector)
	{
	}

	ENGINEAPI TVector(float _X, float _Y) : X(_X), Y(_Y), Z(0.0f), W(1.0f)
	{
	}
	ENGINEAPI TVector(float _X, float _Y, float _Z) : X(_X), Y(_Y), Z(_Z), W(1.0f)
	{
	}
	TVector(float _X, float _Y, float _Z, float _W) : X(_X), Y(_Y), Z(_Z), W(_W)
	{
	}

	TVector(int _X, int _Y) : X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(0.0f), W(1.0f)
	{
	}
	TVector(long _X, long _Y) : X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(0.0f), W(1.0f)
	{
	}

	static float GetVectorAngleDeg(const TVector& _Left, const TVector& _Right)
	{
		return GetVectorAngleRad(_Left, _Right) * UEngineMath::R2D;
	}

	static float GetVectorAngleRad(const TVector& _Left, const TVector& _Right)
	{
		TVector LCopy = _Left;
		TVector RCopy = _Right;
		LCopy.Normalize();
		RCopy.Normalize();

		float CosRad = Dot(LCopy, RCopy);

		// cos 의 역함수 
		// cos(각도) => 결과
		// acos(결과) => 각도
		// cos함수의 역함수
		return acos(CosRad);
	}

	void RoundVector()
	{
		X = ::roundf(X);
		Y = ::roundf(Y);
		Z = ::roundf(Z);
	}

	static TVector Cross(const TVector& _Left, const TVector& _Right)
	{
		TVector Result;
		Result.X = _Left.Y * _Right.Z - _Left.Z * _Right.Y;
		Result.Y = _Left.Z * _Right.X - _Left.X * _Right.Z;
		Result.Z = _Left.X * _Right.Y - _Left.Y * _Right.X;
		return Result;
	}

	static float Dot(const TVector& _Left, const TVector& _Right)
	{
		float LeftLen = _Left.Length();
		float RightLen = _Right.Length();

		// LeftLen* RightLen* cosf(angle);

		return _Left.X * _Right.X + _Left.Y * _Right.Y + _Left.Z * _Right.Z;
	}

	static TVector Normalize(TVector _Value)
	{
		_Value.Normalize();
		return _Value;
	}

	// 360도 개념으로 넣어줘라.
	static TVector AngleToVectorDeg(float _Angle)
	{
		// 360분법을 => 라디안으로 바꾸는 값을 만들어야 한다.
		// 360 => 6.28
		// 근본함수는 라디안 각도를 기반으로 
		return AngleToVectorRad(_Angle * UEngineMath::D2R);
	}

	static TVector Lerp(TVector _A, TVector _B, float _Alpha)
	{
		TVector Result;
		_Alpha = UEngineMath::Clamp(_Alpha, 0.0f, 1.0f);
		Result.X = UEngineMath::Lerp(_A.X, _B.X, _Alpha);
		Result.Y = UEngineMath::Lerp(_A.Y, _B.Y, _Alpha);
		return Result;
	}

	static TVector AngleToVectorRad(float _Angle)
	{
		// 특정 각도를 가리키는 벡터
		// 방향은 정해졌는데 길이는 1인 벡터
		// cosf(_Angle) = 밑변
		return { cosf(_Angle), sinf(_Angle) };
	}

	// 벡터와 행렬이 곱해지는 것을 트랜스폼
	static TVector Transform(const TVector& _Vector, const class FMatrix& _Matrix);

	// 이동 적용할게
	static TVector TransformCoord(const TVector& _Vector, const class FMatrix& _Matrix);

	// 이동 적용하지 않을게
	static TVector TransformNormal(const TVector& _Vector, const class FMatrix& _Matrix);

	int iX() const
	{
		return static_cast<int>(X);
	}

	int iY() const
	{
		return static_cast<int>(Y);
	}

	float hX() const
	{
		return X * 0.5f;
	}

	float hY() const
	{
		return Y * 0.5f;
	}

	// X든 Y든 0이있으면 터트리는 함수.
	bool IsZeroed() const
	{
		return X == 0.0f || Y == 0.0f;
	}

	TVector Half() const
	{
		return { X * 0.5f, Y * 0.5f };
	}

	float Length() const
	{
		return UEngineMath::Sqrt(X * X + Y * Y + Z * Z);
	}

	POINT ConvertWindowPOINT()
	{
		return { iX(), iY() };
	}

	ENGINEAPI class FIntPoint ConvertToPoint() const;

	void Normalize()
	{
		float Len = Length();
		if (0.0f < Len && false == isnan(Len))
		{
			X = X / Len;
			Y = Y / Len;
			Z = Z / Len;
		}
		return;
	}

	TVector NormalizeReturn() const
	{
		TVector Result = *this;
		Result.Normalize();
		return Result;
	}

	void RotationXDeg(float _Angle)
	{
		RotationXRad(_Angle * UEngineMath::D2R);
	}

	void RotationXRad(float _Angle)
	{
		TVector Copy = *this;
		Z = (Copy.Z * cosf(_Angle)) - (Copy.Y * sinf(_Angle));
		Y = (Copy.Z * sinf(_Angle)) + (Copy.Y * cosf(_Angle));
	}

	TVector RotationXDegReturn(float _Angle)
	{
		return RotationXRadReturn(_Angle * UEngineMath::D2R);
	}

	TVector RotationXRadReturn(float _Angle)
	{
		TVector Result = *this;
		Result.Z = (Z * cosf(_Angle)) - (Y * sinf(_Angle));
		Result.Y = (Z * sinf(_Angle)) + (Y * cosf(_Angle));
		return Result;
	}

	void RotationYDeg(float _Angle)
	{
		RotationYRad(_Angle * UEngineMath::D2R);
	}

	void RotationYRad(float _Angle)
	{
		TVector Copy = *this;
		X = (Copy.X * cosf(_Angle)) - (Copy.Z * sinf(_Angle));
		Z = (Copy.X * sinf(_Angle)) + (Copy.Z * cosf(_Angle));
	}

	TVector RotationYDegReturn(float _Angle)
	{
		return RotationYRadReturn(_Angle * UEngineMath::D2R);
	}

	TVector RotationYRadReturn(float _Angle)
	{
		TVector Result = *this;
		Result.X = (X * cosf(_Angle)) - (Z * sinf(_Angle));
		Result.Z = (X * sinf(_Angle)) + (Z * cosf(_Angle));
		return Result;
	}

	TVector ABSVectorReturn()
	{
		return DirectX::XMVectorAbs(DirectVector);
	}

	void RotationZDeg(float _Angle)
	{
		RotationZRad(_Angle * UEngineMath::D2R);
	}

	void RotationZRad(float _Angle)
	{
		TVector Copy = *this;
		X = (Copy.X * cosf(_Angle)) - (Copy.Y * sinf(_Angle));
		Y = (Copy.X * sinf(_Angle)) + (Copy.Y * cosf(_Angle));
	}

	TVector RotationZDegReturn(float _Angle)
	{
		return RotationZRadReturn(_Angle * UEngineMath::D2R);
	}

	TVector RotationZRadReturn(float _Angle)
	{
		TVector Result = *this;
		Result.X = (X * cosf(_Angle)) - (Y * sinf(_Angle));
		Result.Y = (X * sinf(_Angle)) + (Y * cosf(_Angle));
		return Result;
	}

	float Dot(const TVector& other) const
	{
		return X * other.X + Y * other.Y;
	}

	TVector operator*(float _Value) const
	{
		TVector Result;
		Result.X = X * _Value;
		Result.Y = Y * _Value;
		Result.Z = Z * _Value;
		return Result;
	}

	TVector operator+(const TVector& _Other) const
	{
		TVector Result;
		Result.X = X + _Other.X;
		Result.Y = Y + _Other.Y;
		return Result;
	}

	ENGINEAPI TVector operator*(const class FMatrix& _Matrix) const;
	ENGINEAPI TVector& operator*=(const class FMatrix& _Matrix);

	TVector& operator-=(const TVector& _Other)
	{
		X -= _Other.X;
		Y -= _Other.Y;
		Z -= _Other.Z;
		return *this;
	}

	TVector operator-(const TVector& _Other) const
	{
		TVector Result;
		Result.X = X - _Other.X;
		Result.Y = Y - _Other.Y;
		Result.Z = Z - _Other.Z;
		return Result;
	}

	TVector operator-() const
	{
		TVector Result;
		Result.X = -X;
		Result.Y = -Y;
		Result.Z = -Z;
		return Result;
	}

	TVector operator/(int _Value) const
	{
		TVector Result;
		Result.X = X / _Value;
		Result.Y = Y / _Value;
		Result.Z = Z / _Value;
		return Result;
	}

	TVector operator/(const TVector& Other) const
	{
		TVector Result;
		Result.X = X / Other.X;
		Result.Y = Y / Other.Y;
		Result.Z = Z / Other.Z;
		return Result;
	}

	bool operator==(const TVector& _Other) const
	{
		return X == _Other.X && Y == _Other.Y && Z == _Other.Z;
	}

	TVector& operator+=(const TVector& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		Z += _Other.Z;
		return *this;
	}

	TVector& operator*=(const TVector& _Other)
	{
		X *= _Other.X;
		Y *= _Other.Y;
		Z *= _Other.Z;
		return *this;
	}

	TVector& operator*=(float _Other)
	{
		X *= _Other;
		Y *= _Other;
		Z *= _Other;
		return *this;
	}

	std::string ToString()
	{
		std::string Stream;
		std::string XValue = std::format("{:08.2f}", X);
		std::string YValue = std::format("{:08.2f}", Y);
		std::string ZValue = std::format("{:05.0f}", Z);
		std::string WValue = std::format("{:03.2f}", W);

		Stream += "X : [";
		Stream += XValue;
		Stream += "] Y : [";
		Stream += YValue;
		Stream += "] Z : [";
		Stream += ZValue;
		Stream += "] W : [";
		Stream += WValue;
		Stream += "]";
		return Stream;
	}

	// const가 붙은 함수에서는 const가 붙은 함수 호출할수 없다.
	bool EqualToInt(TVector _Other) const
	{
		return iX() == _Other.iX() && iY() == _Other.iY();
	}

	class FQuat DegAngleToQuaternion();
};

template<>
const TVector<float> TVector<float>::NONE = TVector<float>(0.0f, 0.0f, 0.0f, 0.0f);

template<>
const TVector<float> TVector<float>::ZERO = TVector<float>(0.0f, 0.0f, 0.0f, 1.0f);

template<>
const TVector<float> TVector<float>::LEFT = TVector<float>(-1.0f, 0.0f, 0.0f, 0.0f);

template<>
const TVector<float> TVector<float>::RIGHT = TVector<float>(1.0f, 0.0f, 0.0f, 0.0f);

template<>
const TVector<float> TVector<float>::UP = TVector<float>(0.0f, 1.0f, 0.0f, 0.0f);

template<>
const TVector<float> TVector<float>::DOWN = TVector<float>(0.0f, -1.0f, 0.0f, 0.0f);

template<>
const TVector<float> TVector<float>::FORWARD = TVector<float>(0.0f, 0.0f, 1.0f, 0.0f);

template<>
const TVector<float> TVector<float>::BACK = TVector<float>(0.0f, 0.0f, -1.0f, 0.0f);

template<>
const TVector<float> TVector<float>::UNIT = TVector<float>(0.0f, 0.0f, -1.0f, 0.0f);

class FQuat
{
public:
	union
	{
		struct
		{
			float X;
			float Y;
			float Z;
			float W;
		};

		float Arr2D[1][4] = { 0.0f, };
		float Arr1D[4];
		// 다이렉트 simd 연산 전용 벡터.
		DirectX::XMFLOAT4 DirectFloat4;
		DirectX::XMVECTOR DirectVector;
	};

	FQuat()
		: X(0.0f), Y(0.0f), Z(0.0f), W(0.0f)
	{
		DirectFloat4 = DirectX::XMFLOAT4();
		DirectVector = DirectX::XMVECTOR();
	}

	TVector<float> QuaternionToEulerDeg() const;
	TVector<float> QuaternionToEulerRad() const;
};

using FVector = TVector<float>;
using float4 = TVector<float>;


class FMatrix
{
public:
	union
	{
		float Arr2D[4][4] = { 0, };
		FVector ArrVector[4];
		float Arr1D[16];
		DirectX::XMMATRIX DirectMatrix;

		struct
		{
			float _00;
			float _01;
			float _02;
			float _03;
			float _10;
			float _11;
			float _12;
			float _13;
			float _20;
			float _21;
			float _22;
			float _23;
			float _30;
			float _31;
			float _32;
			float _33;
		};
	};

	FMatrix()
	{
		Identity(); // 생성하면 무조건 항등행렬로
	}

	void Identity()
	{
		DirectMatrix = DirectX::XMMatrixIdentity();
	}

	FVector GetFoward()
	{
		FVector Dir = ArrVector[2];
		Dir.Normalize();
		return Dir;
	}

	FVector GetRight()
	{
		FVector Dir = ArrVector[0];
		Dir.Normalize();
		return Dir;
	}

	FVector GetUp()
	{
		FVector Dir = ArrVector[1];
		Dir.Normalize();
		return Dir;
	}

	ENGINEAPI FMatrix operator*(const FMatrix& _Value);

	void Scale(const FVector& _Value)
	{
		DirectMatrix = DirectX::XMMatrixScalingFromVector(_Value.DirectVector);
	}

	void Position(const FVector& _Value)
	{
		DirectMatrix = DirectX::XMMatrixTranslationFromVector(_Value.DirectVector);
	}

	void RotationDeg(const FVector& _Angle)
	{
		RotationRad(_Angle * UEngineMath::D2R);
	}

	void RotationRad(const FVector& _Angle)
	{
		DirectMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(_Angle.DirectVector);
	}

	void Transpose()
	{
		for (size_t y = 0; y < 4; y++)
		{
			for (size_t x = y; x < 4; x++)
			{
				float Swap = Arr2D[y][x];
				Arr2D[y][x] = Arr2D[x][y];
				Arr2D[x][y] = Swap;
			}
		}
	}

	void View(const FVector& _Pos, const FVector& _Dir, const FVector& _Up)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixLookToLH(_Pos.DirectVector, _Dir.DirectVector, _Up.DirectVector);
		return;
	}

	void OrthographicLH(float _Width, float _Height, float _Near, float _Far)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far);
	}

	void PerspectiveFovDeg(float _FovAngle, float _Width, float _Height, float _Near, float _Far)
	{
		PerspectiveFovRad(_FovAngle * UEngineMath::D2R, _Width, _Height, _Near, _Far);
	}

	void PerspectiveFovRad(float _FovAngle, float _Width, float _Height, float _Near, float _Far)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixPerspectiveFovLH(_FovAngle, _Width / _Height, _Near, _Far);
	}
	
	// 뷰포트에서 y축을 반전시켜 윈도우 좌표계 => 데카르트 좌표계로 바꾼다.
	//                 1280          720        640           360            누가 앞에 나오고 누가 뒤에 나올거냐
	void ViewPort(float _Width, float _Height, float _Left, float _Top, float _ZMin, float _ZMax)
	{
		Identity();
		Arr2D[0][0] = _Width * 0.5f;
		// Y축 반전
		Arr2D[1][1] = -_Height * 0.5f;

		// 화면 2~3뿌릴건데 그중에서 누가 앞에오고 뒤에오고를 결정하려면 
		Arr2D[2][2] = _ZMax != 0.0f ? 1.0f : _ZMin / _ZMax;

		Arr2D[3][0] = Arr2D[0][0] + _Left;
		Arr2D[3][1] = -Arr2D[1][1] + _Top;
		Arr2D[3][2] = _ZMax != 0.0f ? 1.0f : _ZMin / _ZMax;
	}

	void RotationXDeg(float _Angle)
	{
		RotationXRad(_Angle * UEngineMath::D2R);
	}

	void RotationXRad(float _Angle)
	{
		Identity();
		Arr2D[1][1] = cosf(_Angle);
		Arr2D[1][2] = -sinf(_Angle);
		Arr2D[2][1] = sinf(_Angle);
		Arr2D[2][2] = cosf(_Angle);
	}

	void RotationYDeg(float _Angle)
	{
		RotationYRad(_Angle * UEngineMath::D2R);
	}

	void RotationYRad(float _Angle)
	{
		Identity();
		Arr2D[0][0] = cosf(_Angle);
		Arr2D[0][2] = sinf(_Angle);
		Arr2D[2][0] = -sinf(_Angle);
		Arr2D[2][2] = cosf(_Angle);
	}

	FMatrix InverseReturn()
	{
		FMatrix Result;
		Result.DirectMatrix = DirectX::XMMatrixInverse(nullptr, DirectMatrix);

		return Result;
	}

	void Decompose(FVector& _Scale, FQuat& _RotQuaternion, FVector& _Pos)
	{
		DirectX::XMMatrixDecompose(&_Scale.DirectVector, &_RotQuaternion.DirectVector, &_Pos.DirectVector, DirectMatrix);
	}

	void RotationZDeg(float _Angle)
	{
		RotationZRad(_Angle * UEngineMath::D2R);
	}

	void RotationZRad(float _Angle)
	{
		Identity();
		Arr2D[0][0] = cosf(_Angle);
		Arr2D[0][1] = -sinf(_Angle);
		Arr2D[1][0] = sinf(_Angle);
		Arr2D[1][1] = cosf(_Angle);
	}
};

using float4x4 = FMatrix;


enum class ECollisionType
{
	Point,
	Rect,
	CirCle, // 타원이 아닌 정방원 
	OBB2D,
	Sphere,
	AABB, 	// 회전하지 않은 박스
	OBB, 	// 회전한 박스
	Max_CollisionType

};

struct FCollisionData
{
	union 
	{
		// 정방원
		DirectX::BoundingSphere Sphere; 
		DirectX::BoundingBox AABB;
		DirectX::BoundingOrientedBox OBB;
	};

	FCollisionData()
	{
		Sphere = DirectX::BoundingSphere();
		AABB = DirectX::BoundingBox();
		OBB = DirectX::BoundingOrientedBox();
	}
};

// 크기와 회전, 위치
struct FTransform
{
	float4 Scale;
	float4 Rotation;
	FQuat Quat;
	float4 Location;

	// 로컬
	float4 RelativeScale;
	float4 RelativeRotation;
	FQuat RelativeQuat;
	float4 RelativeLocation;

	// 월드
	float4 WorldScale;
	float4 WorldRotation;
	FQuat WorldQuat;
	float4 WorldLocation;

	float4x4 ScaleMat;
	float4x4 RotationMat;
	float4x4 LocationMat;
	float4x4 RevolveMat;
	float4x4 ParentMat;
	float4x4 LocalWorld;
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
	float4x4 WVP;

	FTransform()
		: Scale(FVector(1.0f, 1.0f, 1.0f, 1.0f ))
	{

	}

public:
	ENGINEAPI void TransformUpdate(bool _IsAbsolut = false);

	ENGINEAPI void Decompose();


	FVector GetWorldFoward()
	{
		return World.GetFoward();;
	}

	FVector GetWorldRight()
	{
		return World.GetRight();
	}

	FVector GetWorldUp()
	{
		return World.GetUp();
	}

	FVector GetLocalFoward()
	{
		// 부모행렬이 곱해지지 않은 월드
		return LocalWorld.GetFoward();
	}

	FVector GetLocalRight()
	{
		return LocalWorld.GetRight();
	}

	FVector GetLocalUp()
	{
		return LocalWorld.GetUp();
	}

private:
	friend class CollisionFunctionInit;

	static std::function<bool(const FTransform&, const FTransform&)> AllCollisionFunction[static_cast<int>(ECollisionType::Max_CollisionType)][static_cast<int>(ECollisionType::Max_CollisionType)];

public:
	ENGINEAPI static bool Collision(ECollisionType _LeftType, const FTransform& _Left, ECollisionType _RightType, const FTransform& _Right);

	ENGINEAPI static bool PointToCirCle(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool PointToRect(const FTransform& _Left, const FTransform& _Right);

	ENGINEAPI static bool RectToRect(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool RectToCirCle(const FTransform& _Left, const FTransform& _Right);

	ENGINEAPI static bool CirCleToCirCle(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool CirCleToRect(const FTransform& _Left, const FTransform& _Right);

	// 연산량이 크다.
	ENGINEAPI static bool OBB2DToOBB2D(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool OBB2DToRect(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool OBB2DToSphere(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool OBB2DToPoint(const FTransform& _Left, const FTransform& _Right);

	FCollisionData GetCollisionData() const
	{
		FCollisionData Result;
		// OBB를 세팅해준거 같지만 모든 애들을 다 세팅해준 것
		// Sphere와 AABB전체를 다 세팅한 것
		Result.OBB.Center = WorldLocation.DirectFloat3;
		Result.OBB.Extents = (WorldScale * 0.5f).ABSVectorReturn().DirectFloat3;
		Result.OBB.Orientation = WorldQuat.DirectFloat4;
		return Result;
	}

	FVector ZAxisCenterLeftTop() const
	{
		return FVector(Location.X - Scale.Half().X, Location.Y + Scale.Half().Y);
	}

	FVector ZAxisCenterLeftBottom() const
	{
		FVector Result;
		Result.X = Location.X - Scale.hX();
		Result.Y = Location.Y + Scale.hY();
		return Result;
	}

	float ZAxisCenterLeft() const
	{
		return Location.X - Scale.hX();
	}

	float ZAxisCenterTop() const
	{
		return Location.Y + Scale.hY();
	}

	FVector ZAxisCenterRightTop() const
	{
		FVector Result;
		Result.X = Location.X + Scale.hX();
		Result.Y = Location.Y + Scale.hY();
		return Result;
	}

	FVector ZAxisCenterRightBottom() const
	{
		return FVector(Location.X + Scale.Half().X, Location.Y - Scale.Half().Y);
	}

	float ZAxisCenterRight() const
	{
		return Location.X + Scale.hX();
	}

	float ZAxisCenterBottom() const
	{
		return Location.Y - Scale.hY();
	}
};

class FIntPoint
{
public:
	int X = 0;
	int Y = 0;

	static const FIntPoint LEFT;
	static const FIntPoint RIGHT;
	static const FIntPoint UP;
	static const FIntPoint DOWN;

	ENGINEAPI FIntPoint()
	{

	}

	FIntPoint(int _X, int _Y) : X(_X), Y(_Y)
	{

	}

	FIntPoint operator+(FIntPoint _Other) const
	{
		FIntPoint Result;
		Result.X = X + _Other.X;
		Result.Y = Y + _Other.Y;
		return Result;
	}

	FIntPoint operator/(int _Value) const
	{
		FIntPoint Result;
		Result.X = X / _Value;
		Result.Y = Y / _Value;
		return Result;
	}


	bool operator==(FIntPoint _Other) const
	{
		return X == _Other.X && Y == _Other.Y;
	}

	FIntPoint& operator+=(FIntPoint _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		return *this;
	}
};


template<typename ValueType>
class TColor
{
public:
	static const TColor WHITE;
	static const TColor BLACK;
	static const TColor YELLOW;
	static const TColor RED;
	static const TColor BLUE;
	static const TColor GREEN;

	union
	{
		unsigned int Color;
		struct
		{
			unsigned char R;
			unsigned char G;
			unsigned char B;
			unsigned char A;
		};
	};

	TColor(unsigned long _Value)
		:Color(_Value)
	{

	}

	bool operator==(const TColor& _Other) const
	{
		return R == _Other.R && G == _Other.G && B == _Other.B;
	}


	TColor(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A)
		:R(_R), G(_G), B(_B), A(_A)
	{

	}
};

using UColor = TColor<unsigned char>;

template<>
const TColor<unsigned char> TColor<unsigned char>::WHITE = TColor<unsigned char>(255, 255, 255, 255);

template<>
const TColor<unsigned char> TColor<unsigned char>::BLACK = TColor<unsigned char>(0, 0, 0, 255);

template<>
const TColor<unsigned char> TColor<unsigned char>::YELLOW = TColor<unsigned char>(255, 255, 0, 255);

template<>
const TColor<unsigned char> TColor<unsigned char>::RED = TColor<unsigned char>(255, 0, 0, 255);

template<>
const TColor<unsigned char> TColor<unsigned char>::BLUE = TColor<unsigned char>(0, 0, 255, 255);

template<>
const TColor<unsigned char> TColor<unsigned char>::GREEN = TColor<unsigned char>(0, 255, 0, 255);