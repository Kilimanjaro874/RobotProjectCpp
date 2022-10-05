#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "dxlib_ext_texture.h"
#include "dxlib_ext_render_param.h"

namespace dxe {

	class Camera;
	class Mesh final {
	public :
		enum class eMeshFormat {
			MESH_FMT_PG,	// プログラマブルメッシュ
			MESH_FMT_MV		// DxLib 用 MV メッシュ
		};
		enum class fDebugLine {
			TNL_ENUM_BIT_FLAG(FLG_NONE),
			TNL_ENUM_BIT_FLAG(FLG_AXIS),
			TNL_ENUM_BIT_FLAG(FLG_OBB),			
			TNL_ENUM_BIT_FLAG(FLG_AABB),
			TNL_ENUM_BIT_FLAG(FLG_BD_SPHERE),
			TNL_ENUM_BIT_FLAG(FLG_VTX_NML) // pg mesh only
		};
	public :
		~Mesh() {
			if (vb_hdl_) DeleteVertexBuffer(vb_hdl_);
			if (ib_hdl_) DeleteIndexBuffer(ib_hdl_);
			if (mv_hdl_) MV1DeleteModel(mv_hdl_);
		}

		fDebugLine		flg_dbg_line_ = fDebugLine::FLG_NONE;
		tnl::Vector3	pos_;
		tnl::Vector3	scl_ = { 1,1,1 };
		tnl::Quaternion rot_q_;


		//==========================================================================================================================
		// 
		//
		// function
		//
		//

		//-----------------------------------------------------------------------------------------------------
		// Create*** で生成したプログラマブルメッシュを Xファイルフォーマットでリソースとして保存する
		// arg1... 保存先のパス
		// ret.... [ true : 保存成功 ] [ false : 保存失敗 ]
		// tips... この機能が必要になった経緯
		//         DxLib の機能として MV*** 関数の中には DxLib が対応しているフォーマットの
		//         モデルデータをロードして使用しないと機能しない物があるため 例.シャドウマップ, サンプラステート　ブレンドステート等
		bool exportForFileFormatX(const std::string& file_path);

		//-----------------------------------------------------------------------------------------------------
		// Create*** で生成したプログラマブルメッシュを MVファイルフォーマットでリソースとして保存する
		// arg1... 保存先のパス
		// ret.... [ true : 保存成功 ] [ false : 保存失敗 ]
		// tips... MV はバイナリ形式なのでロードの高速化措置として用意
		bool exportForFileFormatMV(const std::string& file_path);

		//-----------------------------------------------------------------------------------------------------
		// 描画
		// arg1... カメラ
		void render(const Camera* camera);


		//==========================================================================================================================
		// 
		//
		// inline function
		//
		//

		// テクスチャの設定
		inline void setTexture( std::shared_ptr<Texture> texture ) {
			tex_diffuse_ = texture;
			if (eMeshFormat::MESH_FMT_MV == mesh_format_) MV1SetTextureGraphHandle(mv_hdl_, 0, tex_diffuse_->getGraphHandle(), true);
		}
		const std::shared_ptr<Texture> getTexture() {
			return tex_diffuse_;
		}

		inline float& alpha() { return render_param_.alpha_; }

		// カリングモード設定
		// arg1... DX_CULLING_***
		inline void setCullingMode(const int mode) noexcept { render_param_.setCullingMode( mode ); }

		// ブレンドモード設定
		// arg1... DX_BLENDMODE_***
		// tips... MV メッシュのみ対応 ( dxlib の仕様 )
		inline void setBlendMode(const int mode) noexcept { render_param_.setBlendMode(mode); }

		// テクスチャサンプラフィルタ設定
		// arg1... DX_DRAWMODE_***
		// tips... MV メッシュのみ対応 ( dxlib の仕様 )
		inline void setSampleFileterMode(const int mode) noexcept { render_param_.setSampleFileterMode(mode); }

		// デフォルトライトの有効化
		inline void setDefaultLightEnable(const bool f) noexcept { render_param_.setDefaultLightEnable(f); }

		// 深度テストの有効化
		inline void setDepthTestEnable(const bool f) noexcept { render_param_.setDepthTestEnable(f); }

		// 深度バッファへの書き込み有効化
		inline void setWriteDepthBufferEnable(const bool f) noexcept { render_param_.setWriteDepthBufferEnable(f); }

		// ディフューズカラー設定
		inline void setMtrlDiffuse(const tnl::Vector3& col) noexcept {
			render_param_.setMtrlDiffuse(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialDifColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Diffuse);
		}
		// スペキュラカラー設定
		inline void setMtrlSpecular(const tnl::Vector3& col) noexcept {
			render_param_.setMtrlSpecular(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialSpcColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Specular);
		}
		// エミッシヴカラー設定
		inline void setMtrlEmissive(const tnl::Vector3& col) noexcept {
			render_param_.setMtrlEmissive(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialEmiColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Emissive);
		}
		// スペキュラパワー設定
		inline void setMtrlSpecPower(const float pow) noexcept {
			render_param_.setMtrlSpecPower( pow );
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialSpcPower(mv_hdl_, 0, render_param_.dxlib_mtrl_.Power);
		}

		// 一括設定
		inline void setRenderParam( const RenderParam& param ) {
			render_param_ = param;
			if (mesh_format_ != eMeshFormat::MESH_FMT_MV) return;
			MV1SetMaterialDifColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Diffuse);
			MV1SetMaterialSpcColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Specular);
			MV1SetMaterialEmiColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Emissive);
			MV1SetMaterialSpcPower(mv_hdl_, 0, render_param_.dxlib_mtrl_.Power);
		}

		//==========================================================================================================================
		//
		// 
		// static function
		// 
		// tips... 幾何学形状の Create *** MV 関数で MV が付いているものに関して
		//         内部的に生成されるメッシュは MV 対応になりますが、生成時に一時的に
		// 　　　　ｘファイルフォーマットで保存をしてから再ロードを行っているので生成には相応のコストが掛かります。
		// 
		//         MV 系統で生成したメッシュは内部にメッシュを構成する頂点データを保持していません。
		//


		//------------------------------------------------------------------------------------------------------------------------
		// 四角形
		// arg1... 幅・高さ ( z 不使用 )  
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Mesh* CreatePlane( const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true, const tnl::Vector3& ltop_uv = {0, 0, 0}, const tnl::Vector3 rbottom_uv = {1, 1, 0}) noexcept;
		static Mesh* CreatePlaneMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true, const tnl::Vector3& ltop_uv = { 0, 0, 0 }, const tnl::Vector3 rbottom_uv = { 1, 1, 0 }) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// 立方体
		// arg1... サイズ
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		// tips... UV は各面へのテクスチャが横並びになっている事を想定した作りになっています。
		//         正面・右・奥・左・上・下の横並びになっているテクスチャを用意してください
		static Mesh* CreateBox(const float size, const int div_w = 5, const int div_h = 5, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateBoxMV(const float size, const int div_w = 5, const int div_h = 5, const bool is_left_cycle = true) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// 球
		// arg1... 半径
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Mesh* CreateSphere( const float radius, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateSphereMV(const float radius, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// 二等辺三角形
		// arg1... 幅・高さ ( z 不使用 )  
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Mesh* CreateTriangleIsosceles(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateTriangleIsoscelesMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 直角三角形
		// arg1... 幅・高さ ( z 不使用 )  
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Mesh* CreateTriangleRight(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateTriangleRightMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 正三角形
		// arg1... 1 辺の長さ
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 左回りインデックス( 法線反転用 )
		static Mesh* CreateTriangleEquilateral(const float edge_lenght, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateTriangleEquilateralMV(const float edge_lenght, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// ドーム
		// arg1... 半径
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 360 度 で完全な半球
		// arg5... 左回りインデックス( 法線反転用 )
		static Mesh* CreateDome(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateDomeMV(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 底面なしの円錐
		// arg1... 半径
		// arg2... 高さ
		// arg3... 横分割数
		// arg4... 縦分割数
		// arg5... 360 度 で底面が完全な円
		// arg6... 左回りインデックス( 法線反転用 )
		static Mesh* CreateCone(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateConeMV(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 上下に穴の空いた円錐
		// arg1... 上部半径
		// arg2... 下部半径
		// arg3... 高さ
		// arg4... 横分割数
		// arg5... 縦分割数
		// arg6... 360 度 で底面が完全な円
		// arg7... 左回りインデックス( 法線反転用 )
		static Mesh* CreateConeCylinder(const float top_radius, const float bottom_radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateConeCylinderMV(const float top_radius, const float bottom_radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 円盤
		// arg1... 半径
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 360 度 で完全な円
		// arg5... 左回りインデックス( 法線反転用 )
		// tips... 360度 180度 90度のみ
		static Mesh* CreateDisk(const float radius, const int div_w = 10, const int div_h = 10, const int angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateDiskMV(const float radius, const int div_w = 10, const int div_h = 10, const int angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 円盤状のリング 
		// arg1... 半径
		// arg2... リングの太さ
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 360 度 で完全な円
		// arg5... 左回りインデックス( 法線反転用 )
		static Mesh* CreateDiskRing(const float radius, const float thickness, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateDiskRingMV(const float radius, const float thickness, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 円状にくり抜いた四角形
		// arg1... 半径
		// arg2... 横分割数
		// arg3... 縦分割数
		// arg4... 360 度 で円がくり抜かれた四角形
		// arg5... 左回りインデックス( 法線反転用 )
		static Mesh* CreateHollowOutDiskPlane(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateHollowOutDiskPlaneMV(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 円筒
		// arg1... 半径
		// arg2... 高さ
		// arg3... 横分割数
		// arg4... 縦分割数
		// arg5... 360 度 で円
		// arg6... 左回りインデックス( 法線反転用 )
		static Mesh* CreateCylinder(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateCylinderMV(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 樽
		// arg1... 上下の半径
		// arg2... 高さ
		// arg3... 上下の半径に対する膨らみ率
		// arg3... 横分割数
		// arg4... 縦分割数
		// arg5... 360 度 で円
		// arg6... 左回りインデックス( 法線反転用 )
		static Mesh* CreateBarrel(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateBarrelMV(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 上半分の樽
		// arg1... 上の半径
		// arg2... 高さ
		// arg3... 上の半径に対する膨らみ率
		// arg3... 横分割数
		// arg4... 縦分割数
		// arg5... 360 度 で円
		// arg6... 左回りインデックス( 法線反転用 )
		static Mesh* CreateHalfBarrel(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateHalfBarrelMV(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;


		//-----------------------------------------------------------------------------------------------------
		// トーラス
		// arg1... 半径
		// arg2... リングの太さ
		// arg3... 輪の分割数
		// arg4... 360 度 で円
		// tips... 360度 180度 90度 45度 のみ
		static Mesh* CreateTorus(const float radius, const float thickness, const int tessellation = 20, const int angle = 360) noexcept;
		static Mesh* CreateTorusMV(const float radius, const float thickness, const int tessellation = 20, const int angle = 360) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// 高さマップから作成
		// arg1... 高さ参照用画像のパス
		// arg2... 幅
		// arg3... 奥行き
		// arg4... 高さの上限値
		// arg5... 横分割数
		// arg6... 縦分割数
		static Mesh* CreateFromHeightMap(const std::string& file_path, const float width, const float depth, const float height_max, const int div_w = 100, const int div_h = 100);

		//-----------------------------------------------------------------------------------------------------
		// OBJ 形式ファイルからメッシュを作成
		// arg1... フィルパス
		// arg2... スケール( デフォルト1.0f )
		static std::vector<Mesh*> CreateFromFileObj( const std::string& file_path, const float scl = 1.0f );

		//-----------------------------------------------------------------------------------------------------
		// DXLib が対応している形式ファイルからメッシュを作成
		// arg1... フィルパス
		// arg2... スケール( デフォルト1.0f )
		static Mesh* CreateFromFileMV(const std::string& file_path, const float scl = 1.0f);


	private:
		Mesh(){}

		int vb_hdl_ = 0;
		int ib_hdl_ = 0;
		int mv_hdl_ = 0;

		std::shared_ptr<Texture> tex_ambient_	= nullptr;	// 未使用
		std::shared_ptr<Texture> tex_diffuse_	= nullptr;
		std::shared_ptr<Texture> tex_specular_	= nullptr;	// 未使用
		std::shared_ptr<Texture> tex_bump_		= nullptr;	// 未使用

		float						bd_sphere_radius_ = 0;
		tnl::Vector3				bd_box_size_;
		std::vector<uint32_t>		idxs_;
		std::vector<VERTEX3D>		vtxs_;
		RenderParam					render_param_;
		eMeshFormat					mesh_format_ = eMeshFormat::MESH_FMT_PG;

		void createPlaneIndex(const int div_w, const int div_h, const bool is_left_cycle);
		void createVBO();

		void drawVtxNormal();
		static Mesh* CreateConvertMV(Mesh* mesh);

	} ;


	constexpr Mesh::fDebugLine operator | (Mesh::fDebugLine left, Mesh::fDebugLine right) {
		return static_cast<Mesh::fDebugLine>(static_cast<int32_t>(left) | static_cast<int32_t>(right));
	}
	constexpr bool operator & (Mesh::fDebugLine left, Mesh::fDebugLine right) {
		return static_cast<bool>(static_cast<int32_t>(left) & static_cast<int32_t>(right));
	}

}

