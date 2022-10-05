#pragma once
#include "../library/tnl_util.h"
#include "../library/tnl_font_texture.h"
#include "../library/tnl_hierarchy_tree.h"
#include "../library/tnl_input.h"
#include "../library/tnl_intersect.h"
#include "../library/tnl_json.h"
#include "../library/tnl_link_linear.h"
#include "../library/tnl_math.h"
#include "../library/tnl_matrix.h"
#include "../library/tnl_quaternion.h"
#include "../library/tnl_sequence.h"
#include "../library/tnl_shared_factory.h"
#include "../library/tnl_timer_callback.h"
#include "../library/tnl_timer_fluct.h"
#include "../library/tnl_vector.h"
#include "../library/tnl_seek_unit.h"
#include "DxLib.h"
#include "dxlib_ext_camera.h"
#include "dxlib_ext_mesh.h"
#include "dxlib_ext_texture.h"

const int DXE_WINDOW_WIDTH = 1024;
const int DXE_WINDOW_HEIGHT = 768;
const float DXE_FIX_FPS = 60.0f;

void DrawStringEx(const int x, const int y, const int color, const char* _str, ...);
void DrawLineEx(const tnl::Vector3& p, const tnl::Vector3& v, const float length, const int color = -1);
void DrawBoxEx(const tnl::Vector3& p, const float width, const float height, const bool fill = true, const int color = -1);

// デバッグ用のグリッド地面
// arg1... カメラ
// arg2... グリッドマスのサイズ
// arg3... グリッドを並べる個数 ( 偶数個で指定 )
// arg4... 16進数カラー
// tips... この関数を使用する前にカメラを生成して update 関数を実行してください
void DrawGridGround( const dxe::Camera* camera, const float square_size, int row_num, int color = 0x99999999 );


void DrawAxis( const dxe::Camera* camera, const tnl::Vector3& pos, const tnl::Quaternion& rot, const float length);
void DrawOBB( const dxe::Camera* camera, const tnl::Vector3& pos, const tnl::Quaternion& rot, const tnl::Vector3& size, const int color = 0x00ff00ff );
void DrawAABB( const dxe::Camera* camera, const tnl::Vector3& pos, const tnl::Vector3& size, const int color = 0x0000ffff);
void DrawBdSphere( const dxe::Camera* camera, const tnl::Vector3& pos, const float radius);


/***************************************************************************************** 
//
//3D start up sample
//

#include <time.h>
#include <algorithm>
#include "game_main.h"
#include "../dxlib_ext/dxlib_ext.h"

dxe::Camera* camera = nullptr;
dxe::Mesh* mesh = nullptr;

bool init = false;

void gameMain(float delta_time) {
	if (!init) {

		camera = new dxe::Camera(DXE_WINDOW_WIDTH, DXE_WINDOW_HEIGHT);
		camera->pos_ = { 100, 100, -300 };
		mesh = dxe::Mesh::CreateBoxMV(50);
		mesh->setTexture("graphics/box.bmp");
		mesh->flg_dbg_line_ = dxe::Mesh::fDebugLine::FLG_AXIS;

		init = true;
	}

	if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
		mesh->rot_q_ *= tnl::Quaternion::RotationAxis({ 0, 1, 0 }, tnl::ToRadian(1));
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_UP)) {
		mesh->rot_q_ *= tnl::Quaternion::RotationAxis({ 1, 0, 0 }, tnl::ToRadian(1));
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_Z)) {
		mesh->rot_q_ *= tnl::Quaternion::RotationAxis({ 0, 0, 1 }, tnl::ToRadian(1));
	}

	camera->update();

	mesh->render(camera);

	DrawGridGround(50, 20);
}


*****************************************************************************************/
