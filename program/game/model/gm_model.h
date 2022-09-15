#pragma once
#include "gm_parts.h"


class Model {
public :
	Model(){};
	virtual ~Model() {
		for (auto pts : parts_) delete pts;
	}
	tnl::Vector3 pos_;
	tnl::Quaternion rot_;
	std::vector<Parts*> parts_;


	//----------------------------------------------------------------------------------------------------------
	// �o�C�i���`���ۑ�
	// arg1... �ۑ���t�@�C���p�X
	// tips... �����̃p�[�c�ō\���������f���f�[�^�Ƃ��ăo�C�i���`���ŕۑ�����
	// tips... ���̊֐��ŕۑ�����ɂ͊e�p�[�c�̃��b�V���� �v���O���}�u���`���Ő������Ă��鎖
	void exportForFileBinary( const std::string& file_path );

	//----------------------------------------------------------------------------------------------------------
	// �o�C�i���`���ۑ����ꂽ���f���f�[�^�̓ǂݍ���
	// arg1... �t�@�C���p�X
	// tips... exportForFileBinary �ŕۑ����ꂽ�t�@�C���̓ǂݍ���
	void loadFromFileBinary( const std::string& file_path );


	virtual void update(float delta_time);


	virtual void render(dxe::Camera* camera);

};



