#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include "tnl_vector.h"

namespace tnl {

	//----------------------------------------------------------------------------------------------
	// �t�H���g�e�N�X�`���̐���
	// arg1... �t�H���g�T�C�Y�@�� �e�N�X�`���T�C�Y�ł͂���܂���
	// arg2... �������� 1 ���� 
	// arg3... �c�����p�Ȃ� true
	// arg4... �g�̑����@���ő�T�s�N�Z���ł����������t�H���g�T�C�Y�ɑ�g�͖����ɂȂ�܂�
	// arg5... �g�̐F
	// arg6... �t�H���g�^�C�v�@���w��Ȃ��̏ꍇ�� �l�r �o���� �ɂȂ�܂�
	// arg7... �㕔�̐F ( x = R, y = G, z = B )
	// arg8... �����̐F ( x = R, y = G, z = B )
	// ret....  
	// tuple [ 0 : 4 byte �J���[�z�� ] 
	// tuple [ 1 : �e�N�X�`���̕� ] 
	// tuple [ 2 : �e�N�X�`���̍��� ]
	// tuple [ 3 : �����̕� ]
	// tuple [ 4 : �����̍��� ]
	// tips... �g�p��
	// auto tex1 = tnl::CreateFontTexture(32, tnl::ToOnceWChara("��"));
	//
	std::tuple<std::shared_ptr<char>, uint32_t, uint32_t, uint32_t, uint32_t>
		CreateFontTexture(
			const uint32_t font_size,
			const wchar_t& once,
			bool is_vertical = false,
			int32_t thickness = 0,
			const tnl::Vector3& thickness_color = tnl::Vector3(0, 0, 0),
			const std::string& font_type = "",
			const tnl::Vector3& top_color = tnl::Vector3(1, 1, 1),
			const tnl::Vector3& bottom_color = tnl::Vector3(1, 1, 1));

}
