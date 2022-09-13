#include "gm_model.h"

//----------------------------------------------------------------------------------------------------------
// �o�C�i���`���ۑ�
// arg1... �ۑ���t�@�C���p�X
// tips... �����̃p�[�c�ō\���������f���f�[�^�Ƃ��ăo�C�i���`���ŕۑ�����
// tips... ���̊֐��ŕۑ�����ɂ͊e�p�[�c�̃��b�V���� �v���O���}�u���`���Ő������Ă��鎖
void Model::exportForFileBinary(const std::string& file_path) {
	auto path = tnl::DetachmentFilePath(file_path);
	std::string parts_path;
	for (int i = 0; i < parts_.size(); ++i) {
		parts_path = std::get<0>(path) + "/" + "parts_" + std::to_string(i);
		parts_[i]->mesh_->exportForFileFormatMV(parts_path); 
	}

	FILE* fp = nullptr ;
	fopen_s(&fp, file_path.c_str(), "wb");
	if (fp) {
		int parts_num = parts_.size();
		fwrite(&parts_num, sizeof(int), 1, fp);

		char tex_path_buff[128] = { 0 };
		for (int i = 0; i < parts_.size(); ++i) {
			memset(tex_path_buff, 0, sizeof(tex_path_buff));
			sprintf_s(tex_path_buff, "%s", parts_[i]->mesh_->getTexture()->getFilePath().c_str());
			fwrite(tex_path_buff, sizeof(tex_path_buff), 1, fp);

			fwrite(&parts_[i]->mesh_->pos_, sizeof(tnl::Vector3), 1, fp);
			fwrite(&parts_[i]->mesh_->scl_, sizeof(tnl::Vector3), 1, fp);
			fwrite(&parts_[i]->mesh_->rot_q_, sizeof(tnl::Quaternion), 1, fp);

			fwrite(&parts_[i]->ofs_pos_, sizeof(tnl::Vector3), 1, fp);
			fwrite(&parts_[i]->ofs_rot_, sizeof(tnl::Quaternion), 1, fp);
		}
		fclose(fp);
	}

}

//----------------------------------------------------------------------------------------------------------
// �o�C�i���`���ۑ����ꂽ���f���f�[�^�̓ǂݍ���
// arg1... �t�@�C���p�X
// tips... exportForFileBinary �ŕۑ����ꂽ�t�@�C����ǂݍ��݂܂�
void Model::loadFromFileBinary(const std::string& file_path) {

	auto path = tnl::DetachmentFilePath(file_path);

	FILE* fp = nullptr;
	fopen_s(&fp, file_path.c_str(), "rb");
	if (fp) {
		int parts_num = 0;
		fread(&parts_num, sizeof(int), 1, fp);

		char tex_path_buff[128] = { 0 };
		for (int i = 0; i < parts_num; ++i) {
			fread(tex_path_buff, sizeof(tex_path_buff), 1, fp);

			Parts* parts = new Parts();
			parts->mesh_ = dxe::Mesh::CreateFromFileMV(std::get<0>(path) + "/" + "parts_" + std::to_string(i));
			fread(&parts->mesh_->pos_, sizeof(tnl::Vector3), 1, fp);
			fread(&parts->mesh_->scl_, sizeof(tnl::Vector3), 1, fp);
			fread(&parts->mesh_->rot_q_, sizeof(tnl::Quaternion), 1, fp);

			fread(&parts->ofs_pos_, sizeof(tnl::Vector3), 1, fp);
			fread(&parts->ofs_rot_, sizeof(tnl::Quaternion), 1, fp);

			parts->mesh_->setTexture( dxe::Texture::CreateFromFile(tex_path_buff) );

			parts_.emplace_back(parts);
		}

		fclose(fp);
	}

}


void Model::update(float delta_time) {
	for (auto pts : parts_) {
		pts->mesh_->pos_ = pos_ + tnl::Vector3::TransformCoord(pts->ofs_pos_, rot_);
		pts->mesh_->rot_q_ = pts->ofs_rot_ * rot_;
	}
}

void Model::render(dxe::Camera* camera) {
	for (auto pts : parts_) {
		if (!pts->is_render_) continue;
		pts->mesh_->render(camera);
	}
}

