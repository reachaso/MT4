#include "Quaternion.h"
#include <cmath>
#include <algorithm>

// デフォルトは単位Quaternion（回転なし）
Quaternion::Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

Quaternion::Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

// Quaternionの積
Quaternion Quaternion::Muyltiply(const Quaternion& lhs, const Quaternion& rhs) {
	const float outW = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	const float outX = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
	const float outY = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
	const float outZ = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;

	return Quaternion(outX, outY, outZ, outW); 
}

// 単位Quaternionを返す
Quaternion Quaternion::IdentityQuaternion() {
	return Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
}

// 共役Quaternionを返す
Quaternion Quaternion::Conjugate(const Quaternion& q) {
	return Quaternion(-q.x, -q.y, -q.z, q.w);
}

// Quaternionのnormを返す
float Quaternion::Norm(const Quaternion& q) { return std::sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z); }

// 正規化されたQuaternionを返す
Quaternion Quaternion::Normalize(const Quaternion& q) {
	constexpr float kEps = 1.0e-6f;
	const float n = Norm(q);
	if (n < kEps)
		return IdentityQuaternion();

	const float inv = 1.0f / n;
	return Quaternion(q.x * inv, q.y * inv, q.z * inv, q.w * inv);
}

// Quaternionの逆数を返す
Quaternion Quaternion::Inverse(const Quaternion& q) {
	constexpr float kEps = 1.0e-6f;
	const float lenSq = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
	if (lenSq < kEps)
		return IdentityQuaternion();

	const Quaternion c = Conjugate(q);
	const float inv = 1.0f / lenSq;
	return Quaternion(c.x * inv, c.y * inv, c.z * inv, c.w * inv);
}


// 任意回転軸を表すQuaternionの生成（axisは内部で正規化）
Quaternion Quaternion::MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	constexpr float kEps = 1.0e-6f;

	const float ax = axis.x;
	const float ay = axis.y;
	const float az = axis.z;

	const float len = std::sqrt(ax * ax + ay * ay + az * az);
	if (len < kEps) {
		return IdentityQuaternion();
	}

	const float invLen = 1.0f / len;
	const float nx = ax * invLen;
	const float ny = ay * invLen;
	const float nz = az * invLen;

	const float half = angle * 0.5f;
	const float s = std::sin(half);
	const float c = std::cos(half);

	// x,y,z,w
	return Quaternion(nx * s, ny * s, nz * s, c);
}

// ベクトルをQuaternionで回転（q * v * q^-1）
Vector3 Quaternion::RottateVector(const Vector3& vector, const Quaternion& quaternion) {
	// 安全のため正規化（回転として使うなら本来は単位Quaternion想定）
	const Quaternion q = Normalize(quaternion);

	// ベクトルを純Quaternion化（w=0）
	const Quaternion p(vector.x, vector.y, vector.z, 0.0f);

	// r = q * p * q^{-1}
	const Quaternion qInv = Inverse(q);
	const Quaternion qp = Muyltiply(q, p);
	const Quaternion r = Muyltiply(qp, qInv);

	return Vector3(r.x, r.y, r.z);
}

Matrix4x4 Quaternion::MakeRotateMatrix(const Quaternion& quaternion) {
	const Quaternion q = Normalize(quaternion);

	const float x = q.x;
	const float y = q.y;
	const float z = q.z;
	const float w = q.w;

	const float xx = x * x;
	const float yy = y * y;
	const float zz = z * z;
	const float xy = x * y;
	const float xz = x * z;
	const float yz = y * z;
	const float wx = w * x;
	const float wy = w * y;
	const float wz = w * z;

	Matrix4x4 m{};

	m.m[0][0] = 1.0f - 2.0f * (yy + zz);
	m.m[0][1] = 2.0f * (xy + wz);
	m.m[0][2] = 2.0f * (xz - wy);
	m.m[0][3] = 0.0f;

	m.m[1][0] = 2.0f * (xy - wz);
	m.m[1][1] = 1.0f - 2.0f * (xx + zz);
	m.m[1][2] = 2.0f * (yz + wx);
	m.m[1][3] = 0.0f;

	m.m[2][0] = 2.0f * (xz + wy);
	m.m[2][1] = 2.0f * (yz - wx);
	m.m[2][2] = 1.0f - 2.0f * (xx + yy);
	m.m[2][3] = 0.0f;

	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 0.0f;
	m.m[3][3] = 1.0f;

	return m;
}

Quaternion Quaternion::Slerp(const Quaternion& q0In, const Quaternion& q1In, float t) {
	// q0,q1 は単位Quaternion
	Quaternion q0 = q0In;
	Quaternion q1 = q1In;

	// ------------------------------
	// 内積（cosθ）
	// ------------------------------
	float dot = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;

	// ------------------------------
	// dot < 0 なら q0 を反転（資料通り）
	// （-q は同じ回転を表すので、短い経路になるように調整）
	// ------------------------------
	if (dot < 0.0f) {
		q0 = Quaternion(-q0.x, -q0.y, -q0.z, -q0.w);
		dot = -dot;
	}

	// acos の安全策（誤差で1をちょい超えるとNaNになる）
	if (dot > 1.0f)
		dot = 1.0f;
	if (dot < -1.0f)
		dot = -1.0f;

	// ------------------------------
	// θ と補間係数
	// ------------------------------
	const float theta = std::acos(dot);
	const float sinTheta = std::sin(theta);

	// sinθ が 0 に近い（ほぼ同じ向き）ときは、0除算を避けてLerp
	constexpr float kEps = 1.0e-6f;
	if (std::fabs(sinTheta) < kEps) {
		const float invT = 1.0f - t;
		return Quaternion(invT * q0.x + t * q1.x, invT * q0.y + t * q1.y, invT * q0.z + t * q1.z, invT * q0.w + t * q1.w);
	}

	const float scale0 = std::sin((1.0f - t) * theta) / sinTheta;
	const float scale1 = std::sin(t * theta) / sinTheta;

	// ------------------------------
	// 補間結果（Normalizeしない）
	// ------------------------------
	return Quaternion(scale0 * q0.x + scale1 * q1.x, scale0 * q0.y + scale1 * q1.y, scale0 * q0.z + scale1 * q1.z, scale0 * q0.w + scale1 * q1.w);
}
