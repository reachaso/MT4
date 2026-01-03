#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

class Quaternion {

public:
	float x;
	float y;
	float z;
	float w;

	Quaternion();
	Quaternion(float x, float y, float z, float w);

	// Quaternionの積
	static Quaternion Muyltiply(const Quaternion& lhs, const Quaternion& rhs);

	// 単位Quaternionを返す
	static Quaternion IdentityQuaternion();

	// 共役Quaternionを返す
	static Quaternion Conjugate(const Quaternion& quaternion);

	// Quaternionのnormを返す
	static float Norm(const Quaternion& quaternion);

	// 正規化されたQuaternionを返す
	static Quaternion Normalize(const Quaternion& quaternion);

	// Quaternionの逆数を返す
	static Quaternion Inverse(const Quaternion& quaternion);

	// 任意回転軸を表すQuaternionの生成
	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

	// ベクトルをQuaternionで回転させた結果を返す
	static Vector3 RottateVector(const Vector3& vector, const Quaternion& quaternion);

	// Quaternionから回転行列を求める
	static Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);
};
