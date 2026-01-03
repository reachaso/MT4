#include "Math/Math3D.h"
#include "Quaternion/Quaternion.h"
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

	Quaternion q1, q2;

	q1 = Quaternion(2.0f, 3.0f, 4.0f, 1.0f);
	q2 = Quaternion(1.0f, 3.0f, 5.0f, 2.0f);

	Quaternion identitiy = Quaternion::IdentityQuaternion();

	Quaternion conj = Quaternion::Conjugate(q1);

	Quaternion inv = Quaternion::Inverse(q1);

	Quaternion normal = Quaternion::Normalize(q1);

	Quaternion mult1 = Quaternion::Muyltiply(q1, q2);

	Quaternion mult2 = Quaternion::Muyltiply(q2, q1);

	float norm = Quaternion::Norm(q1);

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
		// 1行出力ヘルパー（x y z w : Label）
		auto DrawQuatRow = [](const char* label, const Quaternion& q) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%6.2f %6.2f %6.2f %6.2f", q.x, q.y, q.z, q.w);
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(" %s", label);
		};

		auto DrawFloatRow = [](const char* label, float v) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%6.2f", v);
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(" %s", label);
		};

		ImGui::Begin("Quaternion Test");

		ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerV;

		if (ImGui::BeginTable("QuatTable", 2, flags)) {
			// 左列(数値)の幅を固定すると、コロン位置がキレイに揃う
			ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, 260.0f);
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);

			DrawQuatRow("Identity", identitiy);
			DrawQuatRow("Conjugate", conj);
			DrawQuatRow("Inverse", inv);
			DrawQuatRow("Normalize", normal);
			DrawQuatRow("Multipy(q1, q2)", mult1);
			DrawQuatRow("Multipy(q2, q1)", mult2);
			DrawFloatRow("Norm", norm);

			ImGui::EndTable();
		}

		ImGui::End();
#endif

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
