#pragma once
#include "../dxlib_ext/dxlib_ext.h"
#include "dxlib_ext_texture.h"
#include "dxlib_ext_render_param.h"

namespace dxe {

	class Camera;
	class Mesh final {
	public :
		enum class eMeshFormat {
			MESH_FMT_PG,	// �v���O���}�u�����b�V��
			MESH_FMT_MV		// DxLib �p MV ���b�V��
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
		// Create*** �Ő��������v���O���}�u�����b�V���� X�t�@�C���t�H�[�}�b�g�Ń��\�[�X�Ƃ��ĕۑ�����
		// arg1... �ۑ���̃p�X
		// ret.... [ true : �ۑ����� ] [ false : �ۑ����s ]
		// tips... ���̋@�\���K�v�ɂȂ����o��
		//         DxLib �̋@�\�Ƃ��� MV*** �֐��̒��ɂ� DxLib ���Ή����Ă���t�H�[�}�b�g��
		//         ���f���f�[�^�����[�h���Ďg�p���Ȃ��Ƌ@�\���Ȃ��������邽�� ��.�V���h�E�}�b�v, �T���v���X�e�[�g�@�u�����h�X�e�[�g��
		bool exportForFileFormatX(const std::string& file_path);

		//-----------------------------------------------------------------------------------------------------
		// Create*** �Ő��������v���O���}�u�����b�V���� MV�t�@�C���t�H�[�}�b�g�Ń��\�[�X�Ƃ��ĕۑ�����
		// arg1... �ۑ���̃p�X
		// ret.... [ true : �ۑ����� ] [ false : �ۑ����s ]
		// tips... MV �̓o�C�i���`���Ȃ̂Ń��[�h�̍������[�u�Ƃ��ėp��
		bool exportForFileFormatMV(const std::string& file_path);

		//-----------------------------------------------------------------------------------------------------
		// �`��
		// arg1... �J����
		void render(const Camera* camera);


		//==========================================================================================================================
		// 
		//
		// inline function
		//
		//

		// �e�N�X�`���̐ݒ�
		inline void setTexture( std::shared_ptr<Texture> texture ) {
			tex_diffuse_ = texture;
			if (eMeshFormat::MESH_FMT_MV == mesh_format_) MV1SetTextureGraphHandle(mv_hdl_, 0, tex_diffuse_->getGraphHandle(), true);
		}
		const std::shared_ptr<Texture> getTexture() {
			return tex_diffuse_;
		}

		inline float& alpha() { return render_param_.alpha_; }

		// �J�����O���[�h�ݒ�
		// arg1... DX_CULLING_***
		inline void setCullingMode(const int mode) noexcept { render_param_.setCullingMode( mode ); }

		// �u�����h���[�h�ݒ�
		// arg1... DX_BLENDMODE_***
		// tips... MV ���b�V���̂ݑΉ� ( dxlib �̎d�l )
		inline void setBlendMode(const int mode) noexcept { render_param_.setBlendMode(mode); }

		// �e�N�X�`���T���v���t�B���^�ݒ�
		// arg1... DX_DRAWMODE_***
		// tips... MV ���b�V���̂ݑΉ� ( dxlib �̎d�l )
		inline void setSampleFileterMode(const int mode) noexcept { render_param_.setSampleFileterMode(mode); }

		// �f�t�H���g���C�g�̗L����
		inline void setDefaultLightEnable(const bool f) noexcept { render_param_.setDefaultLightEnable(f); }

		// �[�x�e�X�g�̗L����
		inline void setDepthTestEnable(const bool f) noexcept { render_param_.setDepthTestEnable(f); }

		// �[�x�o�b�t�@�ւ̏������ݗL����
		inline void setWriteDepthBufferEnable(const bool f) noexcept { render_param_.setWriteDepthBufferEnable(f); }

		// �f�B�t���[�Y�J���[�ݒ�
		inline void setMtrlDiffuse(const tnl::Vector3& col) noexcept {
			render_param_.setMtrlDiffuse(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialDifColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Diffuse);
		}
		// �X�y�L�����J���[�ݒ�
		inline void setMtrlSpecular(const tnl::Vector3& col) noexcept {
			render_param_.setMtrlSpecular(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialSpcColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Specular);
		}
		// �G�~�b�V���J���[�ݒ�
		inline void setMtrlEmissive(const tnl::Vector3& col) noexcept {
			render_param_.setMtrlEmissive(col);
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialEmiColor(mv_hdl_, 0, render_param_.dxlib_mtrl_.Emissive);
		}
		// �X�y�L�����p���[�ݒ�
		inline void setMtrlSpecPower(const float pow) noexcept {
			render_param_.setMtrlSpecPower( pow );
			if (mesh_format_ == eMeshFormat::MESH_FMT_MV) MV1SetMaterialSpcPower(mv_hdl_, 0, render_param_.dxlib_mtrl_.Power);
		}

		// �ꊇ�ݒ�
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
		// tips... �􉽊w�`��� Create *** MV �֐��� MV ���t���Ă�����̂Ɋւ���
		//         �����I�ɐ�������郁�b�V���� MV �Ή��ɂȂ�܂����A�������Ɉꎞ�I��
		// �@�@�@�@���t�@�C���t�H�[�}�b�g�ŕۑ������Ă���ă��[�h���s���Ă���̂Ő����ɂ͑����̃R�X�g���|����܂��B
		// 
		//         MV �n���Ő����������b�V���͓����Ƀ��b�V�����\�����钸�_�f�[�^��ێ����Ă��܂���B
		//


		//------------------------------------------------------------------------------------------------------------------------
		// �l�p�`
		// arg1... ���E���� ( z �s�g�p )  
		// arg2... ��������
		// arg3... �c������
		// arg4... �����C���f�b�N�X( �@�����]�p )
		static Mesh* CreatePlane( const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true, const tnl::Vector3& ltop_uv = {0, 0, 0}, const tnl::Vector3 rbottom_uv = {1, 1, 0}) noexcept;
		static Mesh* CreatePlaneMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true, const tnl::Vector3& ltop_uv = { 0, 0, 0 }, const tnl::Vector3 rbottom_uv = { 1, 1, 0 }) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// ������
		// arg1... �T�C�Y
		// arg2... ��������
		// arg3... �c������
		// arg4... �����C���f�b�N�X( �@�����]�p )
		// tips... UV �͊e�ʂւ̃e�N�X�`���������тɂȂ��Ă��鎖��z�肵�����ɂȂ��Ă��܂��B
		//         ���ʁE�E�E���E���E��E���̉����тɂȂ��Ă���e�N�X�`����p�ӂ��Ă�������
		static Mesh* CreateBox(const float size, const int div_w = 5, const int div_h = 5, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateBoxMV(const float size, const int div_w = 5, const int div_h = 5, const bool is_left_cycle = true) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// ��
		// arg1... ���a
		// arg2... ��������
		// arg3... �c������
		// arg4... �����C���f�b�N�X( �@�����]�p )
		static Mesh* CreateSphere( const float radius, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateSphereMV(const float radius, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;

		//------------------------------------------------------------------------------------------------------------------------
		// �񓙕ӎO�p�`
		// arg1... ���E���� ( z �s�g�p )  
		// arg2... ��������
		// arg3... �c������
		// arg4... �����C���f�b�N�X( �@�����]�p )
		static Mesh* CreateTriangleIsosceles(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateTriangleIsoscelesMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// ���p�O�p�`
		// arg1... ���E���� ( z �s�g�p )  
		// arg2... ��������
		// arg3... �c������
		// arg4... �����C���f�b�N�X( �@�����]�p )
		static Mesh* CreateTriangleRight(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateTriangleRightMV(const tnl::Vector3& size_wh, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// ���O�p�`
		// arg1... 1 �ӂ̒���
		// arg2... ��������
		// arg3... �c������
		// arg4... �����C���f�b�N�X( �@�����]�p )
		static Mesh* CreateTriangleEquilateral(const float edge_lenght, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateTriangleEquilateralMV(const float edge_lenght, const int div_w = 10, const int div_h = 10, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �h�[��
		// arg1... ���a
		// arg2... ��������
		// arg3... �c������
		// arg4... 360 �x �Ŋ��S�Ȕ���
		// arg5... �����C���f�b�N�X( �@�����]�p )
		static Mesh* CreateDome(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateDomeMV(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// ��ʂȂ��̉~��
		// arg1... ���a
		// arg2... ����
		// arg3... ��������
		// arg4... �c������
		// arg5... 360 �x �Œ�ʂ����S�ȉ~
		// arg6... �����C���f�b�N�X( �@�����]�p )
		static Mesh* CreateCone(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateConeMV(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �㉺�Ɍ��̋󂢂��~��
		// arg1... �㕔���a
		// arg2... �������a
		// arg3... ����
		// arg4... ��������
		// arg5... �c������
		// arg6... 360 �x �Œ�ʂ����S�ȉ~
		// arg7... �����C���f�b�N�X( �@�����]�p )
		static Mesh* CreateConeCylinder(const float top_radius, const float bottom_radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateConeCylinderMV(const float top_radius, const float bottom_radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �~��
		// arg1... ���a
		// arg2... ��������
		// arg3... �c������
		// arg4... 360 �x �Ŋ��S�ȉ~
		// arg5... �����C���f�b�N�X( �@�����]�p )
		// tips... 360�x 180�x 90�x�̂�
		static Mesh* CreateDisk(const float radius, const int div_w = 10, const int div_h = 10, const int angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateDiskMV(const float radius, const int div_w = 10, const int div_h = 10, const int angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �~�Տ�̃����O 
		// arg1... ���a
		// arg2... �����O�̑���
		// arg2... ��������
		// arg3... �c������
		// arg4... 360 �x �Ŋ��S�ȉ~
		// arg5... �����C���f�b�N�X( �@�����]�p )
		static Mesh* CreateDiskRing(const float radius, const float thickness, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateDiskRingMV(const float radius, const float thickness, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �~��ɂ��蔲�����l�p�`
		// arg1... ���a
		// arg2... ��������
		// arg3... �c������
		// arg4... 360 �x �ŉ~�����蔲���ꂽ�l�p�`
		// arg5... �����C���f�b�N�X( �@�����]�p )
		static Mesh* CreateHollowOutDiskPlane(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateHollowOutDiskPlaneMV(const float radius, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �~��
		// arg1... ���a
		// arg2... ����
		// arg3... ��������
		// arg4... �c������
		// arg5... 360 �x �ŉ~
		// arg6... �����C���f�b�N�X( �@�����]�p )
		static Mesh* CreateCylinder(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateCylinderMV(const float radius, const float height, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �M
		// arg1... �㉺�̔��a
		// arg2... ����
		// arg3... �㉺�̔��a�ɑ΂���c��ݗ�
		// arg3... ��������
		// arg4... �c������
		// arg5... 360 �x �ŉ~
		// arg6... �����C���f�b�N�X( �@�����]�p )
		static Mesh* CreateBarrel(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateBarrelMV(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �㔼���̒M
		// arg1... ��̔��a
		// arg2... ����
		// arg3... ��̔��a�ɑ΂���c��ݗ�
		// arg3... ��������
		// arg4... �c������
		// arg5... 360 �x �ŉ~
		// arg6... �����C���f�b�N�X( �@�����]�p )
		static Mesh* CreateHalfBarrel(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;
		static Mesh* CreateHalfBarrelMV(const float radius, const float height, const float bulge, const int div_w = 10, const int div_h = 10, const float angle = 360, const bool is_left_cycle = true) noexcept;


		//-----------------------------------------------------------------------------------------------------
		// �g�[���X
		// arg1... ���a
		// arg2... �����O�̑���
		// arg3... �ւ̕�����
		// arg4... 360 �x �ŉ~
		// tips... 360�x 180�x 90�x 45�x �̂�
		static Mesh* CreateTorus(const float radius, const float thickness, const int tessellation = 20, const int angle = 360) noexcept;
		static Mesh* CreateTorusMV(const float radius, const float thickness, const int tessellation = 20, const int angle = 360) noexcept;

		//-----------------------------------------------------------------------------------------------------
		// �����}�b�v����쐬
		// arg1... �����Q�Ɨp�摜�̃p�X
		// arg2... ��
		// arg3... ���s��
		// arg4... �����̏���l
		// arg5... ��������
		// arg6... �c������
		static Mesh* CreateFromHeightMap(const std::string& file_path, const float width, const float depth, const float height_max, const int div_w = 100, const int div_h = 100);

		//-----------------------------------------------------------------------------------------------------
		// OBJ �`���t�@�C�����烁�b�V�����쐬
		// arg1... �t�B���p�X
		// arg2... �X�P�[��( �f�t�H���g1.0f )
		static std::vector<Mesh*> CreateFromFileObj( const std::string& file_path, const float scl = 1.0f );

		//-----------------------------------------------------------------------------------------------------
		// DXLib ���Ή����Ă���`���t�@�C�����烁�b�V�����쐬
		// arg1... �t�B���p�X
		// arg2... �X�P�[��( �f�t�H���g1.0f )
		static Mesh* CreateFromFileMV(const std::string& file_path, const float scl = 1.0f);


	private:
		Mesh(){}

		int vb_hdl_ = 0;
		int ib_hdl_ = 0;
		int mv_hdl_ = 0;

		std::shared_ptr<Texture> tex_ambient_	= nullptr;	// ���g�p
		std::shared_ptr<Texture> tex_diffuse_	= nullptr;
		std::shared_ptr<Texture> tex_specular_	= nullptr;	// ���g�p
		std::shared_ptr<Texture> tex_bump_		= nullptr;	// ���g�p

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

		// ----- add : array of function pointer

	} ;


	constexpr Mesh::fDebugLine operator | (Mesh::fDebugLine left, Mesh::fDebugLine right) {
		return static_cast<Mesh::fDebugLine>(static_cast<int32_t>(left) | static_cast<int32_t>(right));
	}
	constexpr bool operator & (Mesh::fDebugLine left, Mesh::fDebugLine right) {
		return static_cast<bool>(static_cast<int32_t>(left) & static_cast<int32_t>(right));
	}

}

