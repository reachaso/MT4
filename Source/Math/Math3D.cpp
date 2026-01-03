#include "Math3D.h"
#include <algorithm>
#include <assert.h>
#include <cmath>
#include <math.h>

//==================================
// Vector3 関連関数
//==================================

Vector3 Add(const Vector3 &v1, const Vector3 &v2) {
  Vector3 result;

  result.x = v1.x + v2.x;
  result.y = v1.y + v2.y;
  result.z = v1.z + v2.z;

  return result;
}

Vector3 Subtract(const Vector3 &v1, const Vector3 &v2) {
  Vector3 result;

  result.x = v1.x - v2.x;
  result.y = v1.y - v2.y;
  result.z = v1.z - v2.z;

  return result;
}

Vector3 Multiply(const Vector3 &v1, float scalar) {
  Vector3 result;

  result.x = v1.x * scalar;
  result.y = v1.y * scalar;
  result.z = v1.z * scalar;

  return result;
}

float Dot(const Vector3 &v1, const Vector3 &v2) {
  Vector3 result;

  result.x = v1.x * v2.x;
  result.y = v1.y * v2.y;
  result.z = v1.z * v2.z;

  return result.x + result.y + result.z;
}

float Length(const Vector3 &v) {
  float result;

  result = v.x * v.x + v.y * v.y + v.z * v.z;

  result = sqrtf(result);

  return result;
}

Vector3 Normalize(const Vector3 &v) {
  Vector3 result;
  float length = Length(v);
  if (length != 0) {
    result.x = v.x / length;
    result.y = v.y / length;
    result.z = v.z / length;
  } else {
    result.x = 0;
    result.y = 0;
    result.z = 0;
  }
  return result;
}

Vector3 Vector3Transform(const Vector3 &vector, const Matrix4x4 &matrix) {
  Vector3 result;

  result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] +
             vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];

  result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] +
             vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];

  result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] +
             vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];

  float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] +
            vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

  if (w == 0) {
    w = 1.0f;
  }

  result.x /= w;
  result.y /= w;
  result.z /= w;

  return result;
}

Vector3 project(const Vector3 &v1, const Vector3 &v2) {
  float dot = Dot(v1, v2);
  float lengthSq = Dot(v2, v2); // v2の長さの2乗

  if (lengthSq == 0) {
    return {0, 0, 0};
  }

  float scalar = dot / lengthSq;
  Vector3 result = Multiply(v2, scalar);

  return result;
}

Vector3 closestPoint(const Vector3 &point, const Segment &segment) {

  // 線分の始点
  const Vector3 &a = segment.origin;
  // 線分の終点
  Vector3 b = Add(segment.origin, segment.diff);

  // abベクトル
  Vector3 ab = Subtract(b, a);
  // apベクトル
  Vector3 ap = Subtract(point, a);

  float abLenSq = Dot(ab, ab);
  if (abLenSq == 0.0f) {
    // 線分の長さが0の場合、始点を返す
    return a;
  }

  // 最近接点のパラメータtを計算（0 <= t <= 1にクランプ）
  float t = Dot(ap, ab) / abLenSq;
  if (t < 0.0f)
    t = 0.0f;
  if (t > 1.0f)
    t = 1.0f;

  // 最近接点を計算
  Vector3 result = Add(a, Multiply(ab, t));
  return result;
}

Vector3 Cross(const Vector3 &v1, const Vector3 &v2) {
  Vector3 result;

  result.x = v1.y * v2.z - v1.z * v2.y;
  result.y = v1.z * v2.x - v1.x * v2.z;
  result.z = v1.x * v2.y - v1.y * v2.x;

  return result;
}

Vector3 Project(const Vector3 &v, const Vector3 &axis) {
  float k = Dot(v, axis) / Dot(axis, axis); // v·axis / |axis|²
  return axis * k;                          // k * axis
}

//==================================
// Matrix4x4 関連関数
//==================================

Matrix4x4 Add(const Matrix4x4 &m1, const Matrix4x4 &m2) {
  Matrix4x4 result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.m[i][j] = m1.m[i][j] + m2.m[i][j];
    }
  }
  return result;
}

Matrix4x4 Subtract(const Matrix4x4 &m1, const Matrix4x4 &m2) {
  Matrix4x4 result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.m[i][j] = m1.m[i][j] - m2.m[i][j];
    }
  }
  return result;
}

Matrix4x4 Multiply(const Matrix4x4 &m1, const Matrix4x4 &m2) {
  Matrix4x4 result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.m[i][j] = 0;
      for (int k = 0; k < 4; ++k) {
        result.m[i][j] += m1.m[i][k] * m2.m[k][j];
      }
    }
  }
  return result;
}

Matrix4x4 Inverse(const Matrix4x4 &m) {
  //|A|用の変数
  float det = 0;

  Matrix4x4 result;

  // 行列式の計算
  det = (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]) +
        (m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]) +
        (m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2])

        - (m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]) -
        (m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]) -
        (m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2])

        - (m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]) -
        (m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]) -
        (m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2])

        + (m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]) +
        (m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]) +
        (m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2])

        + (m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]) +
        (m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]) +
        (m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2])

        - (m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]) -
        (m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]) -
        (m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2])

        - (m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]) -
        (m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]) -
        (m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0])

        + (m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]) +
        (m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]) +
        (m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

  result.m[0][0] = 1 / det *
                   ((m.m[1][1] * m.m[2][2] * m.m[3][3]) +
                    (m.m[1][2] * m.m[2][3] * m.m[3][1]) +
                    (m.m[1][3] * m.m[2][1] * m.m[3][2]) -
                    (m.m[1][3] * m.m[2][2] * m.m[3][1]) -
                    (m.m[1][2] * m.m[2][1] * m.m[3][3]) -
                    (m.m[1][1] * m.m[2][3] * m.m[3][2]));

  result.m[0][1] = 1 / det *
                   ((-1 * m.m[0][1] * m.m[2][2] * m.m[3][3]) -
                    (m.m[0][2] * m.m[2][3] * m.m[3][1]) -
                    (m.m[0][3] * m.m[2][1] * m.m[3][2]) +
                    (m.m[0][3] * m.m[2][2] * m.m[3][1]) +
                    (m.m[0][2] * m.m[2][1] * m.m[3][3]) +
                    (m.m[0][1] * m.m[2][3] * m.m[3][2]));

  result.m[0][2] = 1 / det *
                   ((m.m[0][1] * m.m[1][2] * m.m[3][3]) +
                    (m.m[0][2] * m.m[1][3] * m.m[3][1]) +
                    (m.m[0][3] * m.m[1][1] * m.m[3][2]) -
                    (m.m[0][3] * m.m[1][2] * m.m[3][1]) -
                    (m.m[0][2] * m.m[1][1] * m.m[3][3]) -
                    (m.m[0][1] * m.m[1][3] * m.m[3][2]));

  result.m[0][3] = 1 / det *
                   ((-1 * m.m[0][1] * m.m[1][2] * m.m[2][3]) -
                    (m.m[0][2] * m.m[1][3] * m.m[2][1]) -
                    (m.m[0][3] * m.m[1][1] * m.m[2][2]) +
                    (m.m[0][3] * m.m[1][2] * m.m[2][1]) +
                    (m.m[0][2] * m.m[1][1] * m.m[2][3]) +
                    (m.m[0][1] * m.m[1][3] * m.m[2][2]));

  result.m[1][0] = 1 / det *
                   ((-1 * m.m[1][0] * m.m[2][2] * m.m[3][3]) -
                    (m.m[1][2] * m.m[2][3] * m.m[3][0]) -
                    (m.m[1][3] * m.m[2][0] * m.m[3][2]) +
                    (m.m[1][3] * m.m[2][2] * m.m[3][0]) +
                    (m.m[1][2] * m.m[2][0] * m.m[3][3]) +
                    (m.m[1][0] * m.m[2][3] * m.m[3][2]));

  result.m[1][1] = 1 / det *
                   ((m.m[0][0] * m.m[2][2] * m.m[3][3]) +
                    (m.m[0][2] * m.m[2][3] * m.m[3][0]) +
                    (m.m[0][3] * m.m[2][0] * m.m[3][2]) -
                    (m.m[0][3] * m.m[2][2] * m.m[3][0]) -
                    (m.m[0][2] * m.m[2][0] * m.m[3][3]) -
                    (m.m[0][0] * m.m[2][3] * m.m[3][2]));

  result.m[1][2] = 1 / det *
                   ((-1 * m.m[0][0] * m.m[1][2] * m.m[3][3]) -
                    (m.m[0][2] * m.m[1][3] * m.m[3][0]) -
                    (m.m[0][3] * m.m[1][0] * m.m[3][2]) +
                    (m.m[0][3] * m.m[1][2] * m.m[3][0]) +
                    (m.m[0][2] * m.m[1][0] * m.m[3][3]) +
                    (m.m[0][0] * m.m[1][3] * m.m[3][2]));

  result.m[1][3] = 1 / det *
                   ((m.m[0][0] * m.m[1][2] * m.m[2][3]) +
                    (m.m[0][2] * m.m[1][3] * m.m[2][0]) +
                    (m.m[0][3] * m.m[1][0] * m.m[2][2]) -
                    (m.m[0][3] * m.m[1][2] * m.m[2][0]) -
                    (m.m[0][2] * m.m[1][0] * m.m[2][3]) -
                    (m.m[0][0] * m.m[1][3] * m.m[2][2]));

  result.m[2][0] = 1 / det *
                   ((m.m[1][0] * m.m[2][1] * m.m[3][3]) +
                    (m.m[1][1] * m.m[2][3] * m.m[3][0]) +
                    (m.m[1][3] * m.m[2][0] * m.m[3][1]) -
                    (m.m[1][3] * m.m[2][1] * m.m[3][0]) -
                    (m.m[1][1] * m.m[2][0] * m.m[3][3]) -
                    (m.m[1][0] * m.m[2][3] * m.m[3][1]));

  result.m[2][1] = 1 / det *
                   ((-1 * m.m[0][0] * m.m[2][1] * m.m[3][3]) -
                    (m.m[0][1] * m.m[2][3] * m.m[3][0]) -
                    (m.m[0][3] * m.m[2][0] * m.m[3][1]) +
                    (m.m[0][3] * m.m[2][1] * m.m[3][0]) +
                    (m.m[0][1] * m.m[2][0] * m.m[3][3]) +
                    (m.m[0][0] * m.m[2][3] * m.m[3][1]));

  result.m[2][2] = 1 / det *
                   ((m.m[0][0] * m.m[1][1] * m.m[3][3]) +
                    (m.m[0][1] * m.m[1][3] * m.m[3][0]) +
                    (m.m[0][3] * m.m[1][0] * m.m[3][1]) -
                    (m.m[0][3] * m.m[1][1] * m.m[3][0]) -
                    (m.m[0][1] * m.m[1][0] * m.m[3][3]) -
                    (m.m[0][0] * m.m[1][3] * m.m[3][1]));

  result.m[2][3] = 1 / det *
                   ((-1 * m.m[0][0] * m.m[1][1] * m.m[2][3]) -
                    (m.m[0][1] * m.m[1][3] * m.m[2][0]) -
                    (m.m[0][3] * m.m[1][0] * m.m[2][1]) +
                    (m.m[0][3] * m.m[1][1] * m.m[2][0]) +
                    (m.m[0][1] * m.m[1][0] * m.m[2][3]) +
                    (m.m[0][0] * m.m[1][3] * m.m[2][1]));

  result.m[3][0] = 1 / det *
                   ((-1 * m.m[1][0] * m.m[2][1] * m.m[3][2]) -
                    (m.m[1][1] * m.m[2][2] * m.m[3][0]) -
                    (m.m[1][2] * m.m[2][0] * m.m[3][1]) +
                    (m.m[1][2] * m.m[2][1] * m.m[3][0]) +
                    (m.m[1][1] * m.m[2][0] * m.m[3][2]) +
                    (m.m[1][0] * m.m[2][2] * m.m[3][1]));

  result.m[3][1] = 1 / det *
                   ((m.m[0][0] * m.m[2][1] * m.m[3][2]) +
                    (m.m[0][1] * m.m[2][2] * m.m[3][0]) +
                    (m.m[0][2] * m.m[2][0] * m.m[3][1]) -
                    (m.m[0][2] * m.m[2][1] * m.m[3][0]) -
                    (m.m[0][1] * m.m[2][0] * m.m[3][2]) -
                    (m.m[0][0] * m.m[2][2] * m.m[3][1]));

  result.m[3][2] = 1 / det *
                   ((-1 * m.m[0][0] * m.m[1][1] * m.m[3][2]) -
                    (m.m[0][1] * m.m[1][2] * m.m[3][0]) -
                    (m.m[0][2] * m.m[1][0] * m.m[3][1]) +
                    (m.m[0][2] * m.m[1][1] * m.m[3][0]) +
                    (m.m[0][1] * m.m[1][0] * m.m[3][2]) +
                    (m.m[0][0] * m.m[1][2] * m.m[3][1]));

  result.m[3][3] = 1 / det *
                   ((m.m[0][0] * m.m[1][1] * m.m[2][2]) +
                    (m.m[0][1] * m.m[1][2] * m.m[2][0]) +
                    (m.m[0][2] * m.m[1][0] * m.m[2][1]) -
                    (m.m[0][2] * m.m[1][1] * m.m[2][0]) -
                    (m.m[0][1] * m.m[1][0] * m.m[2][2]) -
                    (m.m[0][0] * m.m[1][2] * m.m[2][1]));

  return result;
}

Matrix4x4 Transpose(const Matrix4x4 &m) {
  Matrix4x4 result;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.m[i][j] = m.m[j][i];
    }
  }

  return result;
}

Matrix4x4 MakeIdentity4x4() {
  Matrix4x4 result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (i == j) {
        result.m[i][j] = 1.0f;
      } else {
        result.m[i][j] = 0.0f;
      }
    }
  }
  return result;
}

//==================================
// 回転行列
//==================================

Matrix4x4 MakeRotateMatrix(ShaftType shaft, float radian) {
  Matrix4x4 result;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.m[i][j] = 0.0f;
    }
  }

  switch (shaft) {
  case X:

    result.m[0][0] = 1.0f;
    result.m[1][1] = std::cosf(radian);
    result.m[1][2] = std::sinf(radian);
    result.m[2][1] = -std::sinf(radian);
    result.m[2][2] = std::cosf(radian);
    result.m[3][3] = 1.0f;

    break;
  case Y:

    result.m[0][0] = std::cosf(radian);
    result.m[0][2] = -std::sinf(radian);
    result.m[1][1] = 1.0f;
    result.m[2][0] = std::sinf(radian);
    result.m[2][2] = std::cosf(radian);
    result.m[3][3] = 1.0f;

    break;
  case Z:

    result.m[0][0] = std::cosf(radian);
    result.m[0][1] = std::sinf(radian);
    result.m[1][0] = -std::sinf(radian);
    result.m[1][1] = std::cosf(radian);
    result.m[2][2] = 1.0f;
    result.m[3][3] = 1.0f;

    break;
  }

  return result;
}

//==================================
// 平行移動行列
//==================================

Matrix4x4 MakeTranslateMatrix(const Vector3 &translate) {

  Matrix4x4 result;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (i == j) {
        result.m[i][j] = 1.0f;
      } else {
        result.m[i][j] = 0.0f;
      }
    }
  }

  result.m[3][0] = translate.x;
  result.m[3][1] = translate.y;
  result.m[3][2] = translate.z;

  return result;
}

//==================================
// 拡大縮小行列
//==================================

Matrix4x4 MakeScaleMatrix(const Vector3 &scale) {
  Matrix4x4 result;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (i == j) {
        result.m[i][j] = (i == 0)   ? scale.x
                         : (i == 1) ? scale.y
                         : (i == 2) ? scale.z
                                    : 1.0f;
      } else {
        result.m[i][j] = 0.0f;
      }
    }
  }

  return result;
}

//==================================
// Affine関数
//==================================
Matrix4x4 MakeAffineMatrix(const Vector3 &scale, const Vector3 &rotate,
                           const Vector3 &translate) {

  // スケーリング行列 S
  Matrix4x4 resultScaleMatrix = MakeScaleMatrix(scale);

  // 回転行列 R
  Matrix4x4 resultRotateXMatrix = MakeRotateMatrix(X, rotate.x);
  Matrix4x4 resultRotateYMatrix = MakeRotateMatrix(Y, rotate.y);
  Matrix4x4 resultRotateZMatrix = MakeRotateMatrix(Z, rotate.z);
  Matrix4x4 resultRotateMatrix = Multiply(
      resultRotateXMatrix, Multiply(resultRotateYMatrix, resultRotateZMatrix));

  // 平行移動行列 T
  Matrix4x4 resultTranslateMatrix = MakeTranslateMatrix(translate);

  // アフィン変換行列 W = S * R * T
  Matrix4x4 result = Multiply(
      resultScaleMatrix, Multiply(resultRotateMatrix, resultTranslateMatrix));

  return result;
}

//==================================
// 透視投影行列
//==================================

Matrix4x4 MakePerspectiveFovMatrix(float fov, float aspectRatio, float nearClip,
                                   float farClip) {
  Matrix4x4 result = {};
  float cot = 1.0f / std::tanf(fov / 2.0f);

  result.m[0][0] = cot / aspectRatio;
  result.m[1][1] = cot;
  result.m[2][2] = farClip / (farClip - nearClip);
  result.m[2][3] = 1.0f;
  result.m[3][2] = -(farClip * nearClip) / (farClip - nearClip);

  return result;
}

//==================================
// 正射影行列
//==================================

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right,
                                 float bottom, float nearClip, float farClip) {

  Matrix4x4 result = {};

  result.m[0][0] = 2.0f / (right - left);
  result.m[1][1] = 2.0f / (top - bottom);
  result.m[2][2] = 1.0f / (farClip - nearClip);
  result.m[3][0] = (left + right) / (left - right);
  result.m[3][1] = (top + bottom) / (bottom - top);
  result.m[3][2] = nearClip / (nearClip - farClip);
  result.m[3][3] = 1.0f;

  return result;
}

//==================================
// ビューポート変換行列
//==================================

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height,
                             float minDepth, float maxDepth) {
  Matrix4x4 result = {};

  result.m[0][0] = width / 2.0f;
  result.m[1][1] = -height / 2.0f;
  result.m[2][2] = maxDepth - minDepth;
  result.m[3][0] = left + (width / 2.0f);
  result.m[3][1] = top + (height / 2.0f);
  result.m[3][2] = minDepth;
  result.m[3][3] = 1.0f;

  return result;
}

void keepMinMax(AABB &aabb) {
  // AABBの最小値と最大値を正しく保つための処理
  aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
  aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
  aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
  aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);
  aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);
  aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);
}

Vector3 Lerp(const Vector3 &start, const Vector3 &end, float t) {
  // 線形補間
  if (t < 0.0f) {
    t = 0.0f;
  }
  if (t > 1.0f) {
    t = 1.0f;
  }
  Vector3 result;
  result.x = t * start.x + (1.0f - t) * end.x;
  result.y = t * start.y + (1.0f - t) * end.y;
  result.z = t * start.z + (1.0f - t) * end.z;
  return result;
}

Vector3 Bezier(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2,
               float t) {

  Vector3 p0p1 = Lerp(p0, p1, t);
  Vector3 p1p2 = Lerp(p1, p2, t);
  Vector3 p = Lerp(p0p1, p1p2, t);

  return p;
}

Vector3 operator*(Vector3 &v1, float s) {
  Vector3 result;
  result.x = v1.x * s;
  result.y = v1.y * s;
  result.z = v1.z * s;

  return result;
}

Vector3 operator*(const Vector3 &v1, const Vector3 &v2) {

  Vector3 result;

  result.x = v1.x * v2.x;
  result.y = v1.y * v2.y;
  result.z = v1.z * v2.z;

  return result;
}

Vector3 operator+(const Vector3 &v1, const Vector3 &v2) {
  Vector3 result;
  result.x = v1.x + v2.x;
  result.y = v1.y + v2.y;
  result.z = v1.z + v2.z;

  return result;
}

Vector3 operator-(const Vector3 &v1, const Vector3 &v2) {
  Vector3 result;
  result.x = v1.x - v2.x;
  result.y = v1.y - v2.y;
  result.z = v1.z - v2.z;

  return result;
}

Vector3 operator/(Vector3 &v1, float s) {
  Vector3 result;
  if (s != 0) {
    result.x = v1.x / s;
    result.y = v1.y / s;
    result.z = v1.z / s;
  } else {
    result.x = 0;
    result.y = 0;
    result.z = 0;
  }
  return result;
}

Matrix4x4 operator*(const Matrix4x4 &m1, const Matrix4x4 &m2) {
  Matrix4x4 result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.m[i][j] = 0.0f;
      for (int k = 0; k < 4; ++k) {
        result.m[i][j] += m1.m[i][k] * m2.m[k][j];
      }
    }
  }
  return result;
}

Vector3 &operator+=(Vector3 &v1, const Vector3 &v2) {
  v1.x += v2.x;
  v1.y += v2.y;
  v1.z += v2.z;
  return v1;
}

Vector3 &operator*=(Vector3 &v, float scalar) {
  v.x *= scalar;
  v.y *= scalar;
  v.z *= scalar;
  return v;
}

Vector3 &operator-=(Vector3 &v1, const Vector3 &v2) {
  v1.x -= v2.x;
  v1.y -= v2.y;
  v1.z -= v2.z;
  return v1;
}

void CircularMotion(Ball &ball, Circular &circular) {
  ball.angle += circular.angularVelocity * circular.deltaTime;

  if (ball.angle >= 360.0f) {
    ball.angle -= 360.0f;
  }

  ball.position.x = circular.center.x + circular.radius * std::cos(ball.angle);
  ball.position.y = circular.center.y + circular.radius * std::sin(ball.angle);
  ball.position.z = circular.center.z; // Z座標は固定
}

void UpdateSpring(Ball &ball, Spring &spring) {
  Vector3 diff = ball.position - spring.anchor;

  float length = Length(diff);
  if (length != 0.0f) {
    Vector3 direction = Normalize(diff);
    Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
    Vector3 displacement = (ball.position - restPosition) * length;
    Vector3 restoringForce = -spring.stiffness * displacement;
    Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;
    Vector3 force = restoringForce + dampingForce;
    ball.acceleration = force / ball.mass;
  }

  ball.velocity += ball.acceleration * spring.deltaTime;
  ball.position += ball.velocity * spring.deltaTime;
}

void UpdatePendulum(Ball &ball, Pendulum &pendulum) {
  pendulum.angularAcceleration =
      -(9.8f / pendulum.length) * std::sin(pendulum.angle);
  pendulum.angularVelocity += pendulum.angularAcceleration * pendulum.deltaTime;
  pendulum.angle += pendulum.angularVelocity * pendulum.deltaTime;

  ball.position.x =
      pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
  ball.position.y =
      pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
  ball.position.z = pendulum.anchor.z;
}

void InitializeConicalPendulum(ConicalPendulum &conicalPendulum, Ball &ball) {

  float radius =
      std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
  float height =
      std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;

  ball.position.x = conicalPendulum.anchor.x +
                    radius * std::cos(conicalPendulum.angle) * radius;
  ball.position.y = conicalPendulum.anchor.y - height;
  ball.position.z =
      conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * radius;
}

void UpdateConicalPendulum(Ball &ball, ConicalPendulum &conicalPendulum) {

  conicalPendulum.angularVelocity =
      std::sqrt(9.8f / (conicalPendulum.length *
                        std::cos(conicalPendulum.halfApexAngle)));
  conicalPendulum.angle +=
      conicalPendulum.angularVelocity * conicalPendulum.deltaTime;

  float radius =
      std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
  float height =
      std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;

  ball.position.x = conicalPendulum.anchor.x +
                    radius * std::cos(conicalPendulum.angle) * radius;
  ball.position.y = conicalPendulum.anchor.y - height;
  ball.position.z =
      conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * radius;
}

Vector3 Reflect(const Vector3 &input, const Vector3 &normal) {
    Vector3 result;

    // 入力ベクトルを法線ベクトルに対して反射
    float dotProduct = Dot(input, normal);
    result.x = input.x - 2.0f * dotProduct * normal.x;
    result.y = input.y - 2.0f * dotProduct * normal.y;
    result.z = input.z - 2.0f * dotProduct * normal.z;

    // 反射ベクトルを返す
    return result;
}
