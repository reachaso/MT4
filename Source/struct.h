#pragma once
#include <cstdint>
#include <KamataEngine.h>

// クライアント領域のサイズ
const int32_t kClientWidth = 1280;
const int32_t kClientHeight = 720;

// printf関数の表示位置
static const int kRowHeight = 20;
static const int kColumnWidth = 60;

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

struct Transform {
  KamataEngine::Vector3 scale;
  KamataEngine::Vector3 rotation;
  KamataEngine::Vector3 translation;
};

// 回転軸の定義
enum ShaftType { X, Y, Z };

// 球
struct Sphere {
  KamataEngine::Vector3 center = {0.0f, 0.0f, 0.0f};
  float radius = 1.0f;
  uint32_t color = 0x000000FF;
};

// 三角形
struct Triangle {
  KamataEngine::Vector3 vertices[3] = {
      {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};
  uint32_t color = 0x000000FF;
};

// 軸並行境界箱 (Axis-Aligned Bounding Box)
struct AABB {
  KamataEngine::Vector3 min = {-1.0f, -1.0f, -1.0f};
  KamataEngine::Vector3 max = {1.0f, 1.0f, 1.0f};
  uint32_t color = 0x000000FF;
};

// 線
// origin: 始点
// diff: 終点

struct Line {
  KamataEngine::Vector3 origin = {0.0f, 0.0f, 0.0f};
  KamataEngine::Vector3 diff = {1.0f, 1.0f, 1.0f};
  uint32_t color = 0x000000FF;
};

struct Ray {
  KamataEngine::Vector3 origin = {0.0f, 0.0f, 0.0f};
  KamataEngine::Vector3 diff = {1.0f, 1.0f, 1.0f};
  uint32_t color = 0x000000FF;
};

struct Segment {
  KamataEngine::Vector3 origin = {0.0f, 0.0f, 0.0f};
  KamataEngine::Vector3 diff = {1.0f, 1.0f, 1.0f};
  uint32_t color = 0x000000FF;
};

// 平面
struct Plane {
  KamataEngine::Vector3 normal = {0.0f, 1.0f, 0.0f};
  float distance = 1.0f;
  uint32_t color = 0xFFFFFFFF;
};

// バネ
struct Spring {
  KamataEngine::Vector3 anchor = {0.0f, 0.0f, 0.0f};
  float naturalLength = 1.0f;
  float stiffness = 100.0f;
  float dampingCoefficient = 2.0f;
  float deltaTime = 1.0f / 60.0f;
  bool isMove = false;
};

struct Ball {
  KamataEngine::Vector3 position = {0.0f, 0.0f, 0.0f};
  KamataEngine::Vector3 velocity = {0.0f, 0.0f, 0.0f};
  KamataEngine::Vector3 acceleration = {0.0f, 0.0f, 0.0f};
  float deltaTime = 1.0f / 60.0f;
  float mass = 1.0f;
  float radius = 1.0f;
  float angle = 0.0f;
  uint32_t color = 0x000000FF;
};

struct Circular  {
  KamataEngine::Vector3 center = {0.0f, 0.0f, 0.0f};
  float radius = 1.0f;
  float angularVelocity = 3.14f;
  float angle = 0.0f;
  float deltaTime = 1.0f / 60.0f;
  bool isMove = false;
};

//振り子
struct Pendulum {
  KamataEngine::Vector3 anchor = {0.0f, 1.0f, 0.0f};
  float length = 0.8f;
  float angle = 0.7f;
  float angularVelocity = 0.0f;
  float angularAcceleration = 0.0f;
  float deltaTime = 1.0f / 60.0f;
  bool isMove = false;
};

//円錐振り子
struct ConicalPendulum {
  KamataEngine::Vector3 anchor = {0.0f, 1.0f, 0.0f};
  float length = 0.8f;
  float halfApexAngle = 0.7f;
  float angle = 0.0f;
  float angularVelocity = 0.0f;
  float deltaTime = 1.0f / 60.0f;
  bool isMove = false;
};

struct Capsule {
  Segment segment;
  float radius;
};
