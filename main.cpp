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

	Quaternion rotation = Quaternion::MakeRotateAxisAngleQuaternion(Normalize(Vector3{1.0f, 0.4f, -0.2f}), 0.45f);
	Vector3 pointY = {2.1f, -0.9f, 1.3f};
	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(rotation);
	Vector3 rotatetByQuaternion = Quaternion::RottateVector(pointY, rotation);
	Vector3 rotatetByMatrix = Vector3Transform(pointY, rotateMatrix);

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

		auto RowQuat = [](const char* label, const Quaternion& q) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%6.2f %6.2f %6.2f %6.2f", q.x, q.y, q.z, q.w);
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(": %s", label);
		};

		auto RowVec3 = [](const char* label, const Vector3& v) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%6.2f %6.2f %6.2f", v.x, v.y, v.z);
			ImGui::TableSetColumnIndex(1);
			ImGui::Text(": %s", label);
		};

		auto RowHeader = [](const char* title) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted(title);
			ImGui::TableSetColumnIndex(1);
			ImGui::TextUnformatted("");
		};

		auto RowMat4Line = [](const float a, const float b, const float c, const float d) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::Text("%7.3f %7.3f %7.3f %7.3f", a, b, c, d);
			ImGui::TableSetColumnIndex(1);
			ImGui::TextUnformatted("");
		};

		ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV;

		if (ImGui::BeginTable("ResultTable", 2, flags)) {
			// 左列の幅を固定すると、右の「: label」が綺麗に揃う
			ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, 320.0f);
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);

			// ---- ここから表示したいものを順番に並べる ----
			RowQuat("rotation", rotation); // Quaternion

			RowHeader("rotateMatrix");
			RowMat4Line(rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3]);
			RowMat4Line(rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3]);
			RowMat4Line(rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3]);
			RowMat4Line(rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);

			RowVec3("rotateByQuaternion", rotatetByQuaternion);
			RowVec3("rotateByMatrix", rotatetByMatrix);

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
