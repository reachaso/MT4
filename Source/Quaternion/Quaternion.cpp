#include "Quaternion.h"
#include <cmath>

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
