#include "Math/Math3D.h"
#include "struct.h"
#include <KamataEngine.h>
#include <Windows.h>

using namespace KamataEngine;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	// KamataEngineの初期化
	KamataEngine::Initialize(L"LE2B_03_オオシマ_タイガ");
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommonInstance = DirectXCommon::GetInstance();

	// ImGuiインスタンスの取得
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();

#ifdef _DEBUG

	// ImGuiのフォント設定
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF("Resources/font/Huninn-Regular.ttf", 15.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	io.FontDefault = font;

#endif

	// ==============================
	// 初期化処理ここから
	// ==============================

	Vector3 axis = Normalize({1.0f, 1.0f, 1.0f});
	float angle = 0.44f;

	Matrix4x4 rotateMatrix = MakeRotateAxisAngle(axis, angle);

	//==============================
	// ゲームループ
	//==============================
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break; // ゲームループを抜ける
		}

		// ImGuiの開始
		imguiManager->Begin();

		//==============================
		// 更新処理開始
		//==============================

#ifdef _DEBUG

		ImGui::Begin("rotateMatrix");

		for (int i = 0; i < 4; i++) {
			ImGui::Text("%.3f %.3f %.3f %.3f", rotateMatrix.m[i][0], rotateMatrix.m[i][1], rotateMatrix.m[i][2], rotateMatrix.m[i][3]);
		}

		ImGui::End();

#endif // DEBUG

		//==============================
		// 更新処理終了
		//==============================

		// ImGuiの終了
		imguiManager->End();

		//==============================
		// 描画処理開始
		//==============================
		dxCommonInstance->PreDraw();

		// ImGuiの描画
		imguiManager->Draw();

		dxCommonInstance->PostDraw();
		//==============================
		// 描画処理終了
		//==============================
	}

	// KamataEngineの終了処理
	KamataEngine::Finalize();
	return 0;
}
