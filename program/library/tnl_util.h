#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include "tnl_vector.h"
#include "tnl_using.h"

namespace tnl{

	inline int ARGB8( uint8_t a, uint8_t r, uint8_t g, uint8_t b ) { return ( a << 24 | r << 16 | g << 8 | b ); }
	inline int RGBA8( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255 ) { return ( r << 24 | g << 16 | b << 8 | a ); }
	inline int ABGR8( uint8_t a, uint8_t b, uint8_t g, uint8_t r ) { return (a << 24 | b << 16 | g << 8 | r); }

	//----------------------------------------------------------------------------------------------
	// index �t�� for( auto n : xxx )
	// ���p�@
	// std::list<int> xxx ;
	// for_with_index( it, xxx, i, 0, ++ ){
	// }
	#define for_with_index( it, data, idx, start_idx, fluct ) for( auto[it, idx] = std::pair{ (data).begin(), (start_idx) }; (it) != (data).end(); (it)++, (idx)fluct )


	//----------------------------------------------------------------------------------------------
	// �񋓌^�r�b�g�t���O
	// enum class Test{
	//		TNL_ENUM_BIT_FLAG(A),	// Test::A == 1 << 0
	//		TNL_ENUM_BIT_FLAG(B),	// Test::B == 1 << 1
	//		TNL_ENUM_BIT_FLAG(C),	// Test::C == 1 << 2
	//		TNL_ENUM_BIT_FLAG(D),	// Test::D == 1 << 3
	// } ;
	#define TNL_ENUM_BIT_FLAG(f) prefix_##f, f = (prefix_##f < 1)? 1 : ((prefix_##f)-1) << 1


	//----------------------------------------------------------------------------------------------
	// �t�@�C���T�C�Y�̎擾
	uint64_t GetFileSize(const char* fileName);

	//----------------------------------------------------------------------------------------------
	// CSV Loader
	std::vector<std::vector<std::string>> LoadCsv( const std::string& file_name );

	//----------------------------------------------------------------------------------------------
	// �t�@�C���p�X���p�X�ƃt�@�C�����Ɗg���q�ɕ�������
	// ret.... tuple [ 0 : �p�X ] [ 1 : �t�@�C���� ] [ 2 : �g���q ]
	std::tuple<std::string, std::string, std::string> DetachmentFilePath( const std::string& file_path );

	//----------------------------------------------------------------------------------------------
	// ttf �t�H���g�̒ǉ�
	void AddFontTTF( const std::string& ttf_file_path );


	//----------------------------------------------------------------------------------------------
	// char ���� wchar �ւ̕ϊ�
	void ToWChara(wchar_t* wstrDest, const char* strSrc, int length);
	void ToWChara(wchar_t* wstrDest, uint32_t wstr_lenght, const std::string& src);
	wchar_t ToOnceWChara(const char* strSrc);


	//----------------------------------------------------------------------------------------------
	// �f�o�b�O�R���\�[���ւ̏o��
	void DebugTrace(const char* _str, ...);


	//----------------------------------------------------------------------------------------------
	// ASSERT
#define TNL_DEBUG_ASSERTE( msg ) {}

	//----------------------------------------------------------------------------------------------
	// �x���o��
	void WarningMassage(const char* _str, ...);

	//----------------------------------------------------------------------------------------------
	// ���ʃr�b�g 0 ���� �w�茅�̃r�b�g���L���Ȑ��l�����
	inline int OnBit(int n) { return (1 << n); }

	//----------------------------------------------------------------------------------------------
	// �P�r�b�g�ÂԊu���J����
	int SpaceBit32(int n);

	//----------------------------------------------------------------------------------------------
	// �L���ȃr�b�g���J�E���g����
	int CountBit8(const uint8_t v);
	int CountBit16(const uint16_t v);
	int CountBit32(const uint32_t v);
	int CountBit64(const uint64_t v);

	//----------------------------------------------------------------------------------------------
	// �ő�L���r�b�g���iMSB�FMost Significant Bit�j
	// �S�Ẵr�b�g�� 0 �Ȃ� false ���A��
	// ��) value �� 0xFF �Ȃ� out �ɂ� value �� Bit��-1 ������ uint8_t �Ȃ� 7
	bool MostBit8(const uint8_t value, int& out);
	bool MostBit16(const uint16_t value, int& out);
	bool MostBit32(const uint32_t value, int& out);
	bool MostBit64(const uint64_t value, int& out);

	//----------------------------------------------------------------------------------------------
	// �ŏ��L���r�b�g���iLSB�FLeast Significant Bit�j
	// �S�Ẵr�b�g�� 0 �Ȃ� false ���A��
	// ��) value �� 0x01 �Ȃ� out �ɂ� 0 ������
	bool LeastBit8(const uint8_t value, int& out);
	bool LeastBit16(const uint16_t value, int& out);
	bool LeastBit32(const uint32_t value, int& out);
	bool LeastBit64(const uint64_t value, int& out);

}


