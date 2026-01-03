#pragma once
#include "struct.h"

//==================================
// Vector3 関連関数
//==================================

// 加算
Vector3 Add(const Vector3 &v1, const Vector3 &v2);
// 減算
Vector3 Subtract(const Vector3 &v1, const Vector3 &v2);
// スカラー倍
Vector3 Multiply(const Vector3 &v1, float scalar);
// 内積
float Dot(const Vector3 &v1, const Vector3 &v2);
// 長さ
float Length(const Vector3 &v);
// 正規化
Vector3 Normalize(const Vector3 &v);
// 座標変換
Vector3 Vector3Transform(const Vector3 &vector, const Matrix4x4 &matrix);
// 正射影ベクトル
Vector3 project(const Vector3 &v1, const Vector3 &v2);
// 最近接点
Vector3 closestPoint(const Vector3 &point, const Segment& segment);
// 直線の交点
Vector3 Cross(const Vector3 &v1, const Vector3 &v2);

Vector3 Project(const Vector3 &v, const Vector3 &axis);

//==================================
// Matrix4x4 関連関数
//==================================

// 行列の加法
Matrix4x4 Add(const Matrix4x4 &m1, const Matrix4x4 &m2);
// 行列の減法
Matrix4x4 Subtract(const Matrix4x4 &m1, const Matrix4x4 &m2);
// 行列の積
Matrix4x4 Multiply(const Matrix4x4 &m1, const Matrix4x4 &m2);
// 逆行列
Matrix4x4 Inverse(const Matrix4x4 &m);
// 転置行列
Matrix4x4 Transpose(const Matrix4x4 &m);
// 単位行列の生成
Matrix4x4 MakeIdentity4x4();

//==================================
// 回転行列
//==================================

Matrix4x4 MakeRotateMatrix(ShaftType shaft, float radian);

//==================================
// 平行移動行列
//==================================

Matrix4x4 MakeTranslateMatrix(const Vector3 &translate);

//==================================
// 拡大縮小行列
//==================================

Matrix4x4 MakeScaleMatrix(const Vector3 &scale);

//==================================
// Affine関数
//==================================
Matrix4x4 MakeAffineMatrix(const Vector3 &scale, const Vector3 &rotate,
                           const Vector3 &translate);

//==================================
// 透視投影行列
//==================================

Matrix4x4 MakePerspectiveFovMatrix(float fov, float aspectRatio, float nearClip,
                                   float farClip);

//==================================
// 正射影行列
//==================================

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right,
                                 float bottom, float nearClip, float farClip);

//==================================
// ビューポート変換行列
//==================================

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height,
                             float minDepth, float maxDepth);

//==================================
// min/max 関数
//==================================

void keepMinMax(AABB& aabb);

//==================================
// Lerp 関数
//==================================

Vector3 Lerp(const Vector3 &start, const Vector3 &end, float t);

//==================================
// Bezier
//==================================

Vector3 Bezier(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2,
               float t);

//==================================
// 演算子オーバーロード
//==================================

Vector3 operator*(Vector3& v1,float s);

Vector3 operator*(const Vector3 &v1, const Vector3 &v2);

Vector3 operator+(const Vector3 &v1, const Vector3 &v2);

Vector3 operator-(const Vector3 &v1, const Vector3 &v2);

Vector3 operator/(Vector3 &v1, float s);

Matrix4x4 operator*(const Matrix4x4 &m1, const Matrix4x4 &m2);

inline Vector3 operator*(const Vector3 &vector, float scalar) {
  return {vector.x * scalar, vector.y * scalar, vector.z * scalar};
}

inline Vector3 operator*(float scalar, const Vector3 &vector) {
  return vector * scalar;
}

Vector3 &operator+=(Vector3 &v1, const Vector3 &v2);
Vector3 &operator*=(Vector3 &v, float scalar);

Vector3 &operator-=(Vector3 &v1, const Vector3 &v2);

//==================================
// 円運動
//==================================

void CircularMotion(Ball &ball, Circular &circular);

//==================================
// バネ
//==================================

void UpdateSpring(Ball &ball, Spring &spring);

//==================================
// 振り子
//==================================

void UpdatePendulum(Ball &ball, Pendulum &pendulum);

//==================================
// 円錐振り子
//==================================

void InitializeConicalPendulum(ConicalPendulum &conicalPendulum,
                               Ball &ball);
void UpdateConicalPendulum(Ball &ball, ConicalPendulum &conicalPendulum);

//==================================
// 反射ベクトル
//==================================

Vector3 Reflect(const Vector3 &input, const Vector3 &normal);
