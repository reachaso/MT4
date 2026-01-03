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

	Quaternion rotation0 = Quaternion::MakeRotateAxisAngleQuaternion({0.71f, 0.71f, 0.0f}, 0.3f);
	Quaternion rotation1 = Quaternion::MakeRotateAxisAngleQuaternion({0.71f, 0.0f, 0.71f}, 3.141592f);

	Quaternion interpolate0 = Quaternion::Slerp(rotation0, rotation1, 0.0f);
	Quaternion interpolate1 = Quaternion::Slerp(rotation0, rotation1, 0.3f);
	Quaternion interpolate2 = Quaternion::Slerp(rotation0, rotation1, 0.5f);
	Quaternion interpolate3 = Quaternion::Slerp(rotation0, rotation1, 0.7f);
	Quaternion interpolate4 = Quaternion::Slerp(rotation0, rotation1, 1.0f);

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
		
		ImGui::Begin("Quaternion Test");

		auto RowQuatSlerp = [](const char* name, const Quaternion& q, float t) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%5.2f  %5.2f  %5.2f  %5.2f", q.x, q.y, q.z, q.w);
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(": %s, Slerp(q0, q1, %.1ff)", name, t);
		};

		ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg;

		if (ImGui::BeginTable("SlerpTable", 2, flags)) {
			// 左列幅固定：コロン位置が綺麗に揃う
			ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, 280.0f);
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);

			// 例：t を変えたSlerp結果を表示
			struct Item {
				const char* name;
				float t;
			};
			const Item items[] = {
			    {"interpolate1", 0.0f},
                {"interpolate1", 0.3f},
                {"interpolate2", 0.5f},
                {"interpolate3", 0.7f},
                {"interpolate4", 1.0f},
			};

			for (const auto& it : items) {
				Quaternion qi = Quaternion::Slerp(rotation0, rotation1, it.t); // ←Slerp実装済み前提
				RowQuatSlerp(it.name, qi, it.t);
			}

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
