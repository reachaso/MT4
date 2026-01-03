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

	Vector3 from0 = Normalize(Vector3{1.0f, 0.7f, 0.5f});
	Vector3 to0 = Multiply(from0, -1.0f);

	Vector3 from1 = Normalize(Vector3{-0.6f, 0.9f, 0.2f});
	Vector3 to1 = Normalize(Vector3{0.4f, 0.7f, -0.5f});

	Matrix4x4 rotateMatrix0 = DirectionToDirection(Normalize(Vector3{1.0f, 0.0f, 0.0f}), Normalize(Vector3{-1.0f, 0.0f, 0.0f}));

	Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);
	Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);

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

		ImGui::Text("rotateMatrix0");

		for (int i = 0; i < 4; i++) {
			ImGui::Text("%.3f %.3f %.3f %.3f", rotateMatrix0.m[i][0], rotateMatrix0.m[i][1], rotateMatrix0.m[i][2], rotateMatrix0.m[i][3]);
		}

		ImGui::Text("rotateMatrix1");

		for (int i = 0; i < 4; i++) {
			ImGui::Text("%.3f %.3f %.3f %.3f", rotateMatrix1.m[i][0], rotateMatrix1.m[i][1], rotateMatrix1.m[i][2], rotateMatrix1.m[i][3]);
		}

		ImGui::Text("rotateMatrix2");

		for (int i = 0; i < 4; i++) {
			ImGui::Text("%.3f %.3f %.3f %.3f", rotateMatrix2.m[i][0], rotateMatrix2.m[i][1], rotateMatrix2.m[i][2], rotateMatrix2.m[i][3]);
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
