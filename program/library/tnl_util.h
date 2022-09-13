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
	// index 付き for( auto n : xxx )
	// 利用法
	// std::list<int> xxx ;
	// for_with_index( it, xxx, i, 0, ++ ){
	// }
	#define for_with_index( it, data, idx, start_idx, fluct ) for( auto[it, idx] = std::pair{ (data).begin(), (start_idx) }; (it) != (data).end(); (it)++, (idx)fluct )


	//----------------------------------------------------------------------------------------------
	// 列挙型ビットフラグ
	// enum class Test{
	//		TNL_ENUM_BIT_FLAG(A),	// Test::A == 1 << 0
	//		TNL_ENUM_BIT_FLAG(B),	// Test::B == 1 << 1
	//		TNL_ENUM_BIT_FLAG(C),	// Test::C == 1 << 2
	//		TNL_ENUM_BIT_FLAG(D),	// Test::D == 1 << 3
	// } ;
	#define TNL_ENUM_BIT_FLAG(f) prefix_##f, f = (prefix_##f < 1)? 1 : ((prefix_##f)-1) << 1


	//----------------------------------------------------------------------------------------------
	// ファイルサイズの取得
	uint64_t GetFileSize(const char* fileName);

	//----------------------------------------------------------------------------------------------
	// CSV Loader
	std::vector<std::vector<std::string>> LoadCsv( const std::string& file_name );

	//----------------------------------------------------------------------------------------------
	// ファイルパスをパスとファイル名と拡張子に分離する
	// ret.... tuple [ 0 : パス ] [ 1 : ファイル名 ] [ 2 : 拡張子 ]
	std::tuple<std::string, std::string, std::string> DetachmentFilePath( const std::string& file_path );

	//----------------------------------------------------------------------------------------------
	// ttf フォントの追加
	void AddFontTTF( const std::string& ttf_file_path );


	//----------------------------------------------------------------------------------------------
	// char から wchar への変換
	void ToWChara(wchar_t* wstrDest, const char* strSrc, int length);
	void ToWChara(wchar_t* wstrDest, uint32_t wstr_lenght, const std::string& src);
	wchar_t ToOnceWChara(const char* strSrc);


	//----------------------------------------------------------------------------------------------
	// デバッグコンソールへの出力
	void DebugTrace(const char* _str, ...);


	//----------------------------------------------------------------------------------------------
	// ASSERT
#define TNL_DEBUG_ASSERTE( msg ) {}

	//----------------------------------------------------------------------------------------------
	// 警告出力
	void WarningMassage(const char* _str, ...);

	//----------------------------------------------------------------------------------------------
	// 下位ビット 0 から 指定桁のビットが有効な数値を作る
	inline int OnBit(int n) { return (1 << n); }

	//----------------------------------------------------------------------------------------------
	// １ビットづつ間隔を開ける
	int SpaceBit32(int n);

	//----------------------------------------------------------------------------------------------
	// 有効なビットをカウントする
	int CountBit8(const uint8_t v);
	int CountBit16(const uint16_t v);
	int CountBit32(const uint32_t v);
	int CountBit64(const uint64_t v);

	//----------------------------------------------------------------------------------------------
	// 最大有効ビット数（MSB：Most Significant Bit）
	// 全てのビットが 0 なら false が帰る
	// 例) value が 0xFF なら out には value の Bit数-1 が入る uint8_t なら 7
	bool MostBit8(const uint8_t value, int& out);
	bool MostBit16(const uint16_t value, int& out);
	bool MostBit32(const uint32_t value, int& out);
	bool MostBit64(const uint64_t value, int& out);

	//----------------------------------------------------------------------------------------------
	// 最小有効ビット数（LSB：Least Significant Bit）
	// 全てのビットが 0 なら false が帰る
	// 例) value が 0x01 なら out には 0 が入る
	bool LeastBit8(const uint8_t value, int& out);
	bool LeastBit16(const uint16_t value, int& out);
	bool LeastBit32(const uint32_t value, int& out);
	bool LeastBit64(const uint64_t value, int& out);

}


