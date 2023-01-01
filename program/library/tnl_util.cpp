#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <crtdbg.h>
#include <tchar.h>
#include <wrl.h>
#include "tnl_util.h"
#include "tnl_vector.h"

namespace tnl {

	//----------------------------------------------------------------------------------------------
	// デバッグコンソールへの出力
	void DebugTrace(const char* _str, ...) {
#if _DEBUG
#ifdef UNICODE
		char buff[2048] = { 0 };
		WCHAR wbuff[2048] = { 0 };
		va_list argptr;
		va_start(argptr, _str);
		vsprintf_s(buff, sizeof(buff), _str, argptr);
		va_end(argptr);
		ToWChara(wbuff, buff, 2048);
		OutputDebugString(wbuff);
#else
		char buff[2048] = { 0 };
		va_list argptr;
		va_start(argptr, _str);
		vsprintf_s(buff, _str, argptr);
		va_end(argptr);
		OutputDebugString(buff);
#endif
#endif
	}

	//----------------------------------------------------------------------------------------------
	// 警告出力
	void WarningMassage(const char* _str, ...) {
#if _DEBUG
#ifdef UNICODE
		OutputDebugString(L"\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		tnl::DebugTrace("");

		char buff[2048] = { 0 };
		WCHAR wbuff[2048] = { 0 };
		va_list argptr;
		va_start(argptr, _str);
		vsprintf_s(buff, sizeof(buff), _str, argptr);
		va_end(argptr);
		ToWChara(wbuff, buff, 2048);
		OutputDebugString(wbuff);
		OutputDebugString(L"\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
#else
		OutputDebugString("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		tnl::DebugTrace("FILE : %s\n", __FILE__);
		tnl::DebugTrace("LINE : %d\n", __LINE__);
		char buff[2048] = { 0 };
		va_list argptr;
		va_start(argptr, _str);
		vsprintf_s(buff, _str, argptr);
		va_end(argptr);
		OutputDebugString(buff);
		OutputDebugString("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");
#endif
#endif
	}

	uint64_t GetFileSize(const char* fileName)
	{
		FILE* fp;
		auto err = fopen_s(&fp, fileName, "rb");
		if (err != 0) return -1LL;

		if (fseek(fp, 0L, SEEK_END) == 0) {
			fpos_t pos;

			if (fgetpos(fp, &pos) == 0) {
				fclose(fp);
				return (long long int)pos;
			}
		}

		fclose(fp);
		return -1LL;
	}


	std::tuple<std::string, std::string, std::string> 
		DetachmentFilePath(const std::string& file_path) {

		uint32_t n1 = static_cast<uint32_t>(file_path.find_last_of("/"));
		std::string path = file_path.substr(0, n1);

		uint32_t n2 = static_cast<uint32_t>(file_path.find_last_of("."));
		std::string file = file_path.substr( n1 + 1, n2 - path.length() - 1 );

		uint32_t n3 = static_cast<uint32_t>(file_path.length()) - n2;
		std::string ext = file_path.substr(n2+1, n3);

		return std::make_tuple(path, file, ext);
	}


	void AddFontTTF(const std::string& ttf_file_path) {
#ifdef UNICODE
		wchar_t buff[256] = { 0 };
		tnl::ToWChara(buff, 256, ttf_file_path);
		DESIGNVECTOR design;
		if (0 == AddFontResourceEx(
			buff,
			FR_PRIVATE,
			&design
		)) {
			// フォントの追加に失敗
			WarningMassage("フォントの追加に失敗 %s", ttf_file_path.c_str());
		}
#else
		DESIGNVECTOR design;
		if (0 == AddFontResourceEx(
			ttf_file_path.c_str(),
			FR_PRIVATE,
			&design
		)) {
			// フォントの追加に失敗
			WarningMassage("フォントの追加に失敗 %s", ttf_file_path.c_str());
		}
#endif
	}



	void ToWChara(wchar_t* wstrDest, const char* strSrc, int length)
	{
		if (wstrDest == NULL || strSrc == NULL || length < 1) assert(0);
		int nResult = MultiByteToWideChar(CP_ACP, 0, strSrc, -1, wstrDest, length);
		wstrDest[length - 1] = 0;
		if (nResult == 0) TNL_DEBUG_ASSERTE("toWChara error");
	}

	void ToWChara(wchar_t* wstrDest, uint32_t wstr_lenght, const std::string& src) {
		uint32_t size = static_cast<uint32_t>(src.length()) + 1;
		char* buff = new char[src.length() + 1];
		memset(buff, 0, size);
		sprintf_s( buff, sizeof(char)*size, "%s", src.c_str() );
		ToWChara( wstrDest, buff, wstr_lenght );
	}

	wchar_t ToOnceWChara(const char* strSrc) {
		wchar_t buff[2];
		tnl::ToWChara(buff, strSrc, 2);
		return buff[0];
	}


	int SpaceBit32(int n) {
		n = (n | (n << 8)) & 0x00ff00ff;
		n = (n | (n << 4)) & 0x0f0f0f0f;
		n = (n | (n << 2)) & 0x33333333;
		return (n | (n << 1)) & 0x55555555;
	}

	int CountBit8(const uint8_t v) {
		uint8_t count = (v & 0x55) + ((v >> 1) & 0x55);
		count = (count & 0x33) + ((count >> 2) & 0x33);
		return (count & 0x0f) + ((count >> 4) & 0x0f);
	}

	int CountBit16(const uint16_t v) {
		uint16_t count = (v & 0x5555) + ((v >> 1) & 0x5555);
		count = (count & 0x3333) + ((count >> 2) & 0x3333);
		count = (count & 0x0f0f) + ((count >> 4) & 0x0f0f);
		return (count & 0x00ff) + ((count >> 8) & 0x00ff);
	}

	int CountBit32(const uint32_t v) {
		uint32_t count = (v & 0x55555555) + ((v >> 1) & 0x55555555);
		count = (count & 0x33333333) + ((count >> 2) & 0x33333333);
		count = (count & 0x0f0f0f0f) + ((count >> 4) & 0x0f0f0f0f);
		count = (count & 0x00ff00ff) + ((count >> 8) & 0x00ff00ff);
		return (count & 0x0000ffff) + ((count >> 16) & 0x0000ffff);
	}

	int CountBit64(const uint64_t v) {
		uint64_t count = (v & 0x5555555555555555) + ((v >> 1) & 0x5555555555555555);
		count = (count & 0x3333333333333333) + ((count >> 2) & 0x3333333333333333);
		count = (count & 0x0f0f0f0f0f0f0f0f) + ((count >> 4) & 0x0f0f0f0f0f0f0f0f);
		count = (count & 0x00ff00ff00ff00ff) + ((count >> 8) & 0x00ff00ff00ff00ff);
		count = (count & 0x0000ffff0000ffff) + ((count >> 16) & 0x0000ffff0000ffff);
		return (int)((count & 0x00000000ffffffff) + ((count >> 32) & 0x00000000ffffffff));
	}

	bool MostBit8(const uint8_t value, int& out) {
		if (value == 0) return false;
		uint8_t v = value;
		v |= (v >> 1);
		v |= (v >> 2);
		v |= (v >> 4);
		out = CountBit8(v) - 1;
		return true;
	}

	bool MostBit16(const uint16_t value, int& out) {
		if (value == 0) return false;
		uint16_t v = value;
		v |= (v >> 1);
		v |= (v >> 2);
		v |= (v >> 4);
		v |= (v >> 8);
		out = CountBit16(v) - 1;
		return true;
	}

	bool MostBit32(const uint32_t value, int& out) {
		if (value == 0) return false;
		uint32_t v = value;
		v |= (v >> 1);
		v |= (v >> 2);
		v |= (v >> 4);
		v |= (v >> 8);
		v |= (v >> 16);
		out = CountBit32(v) - 1;
		return true;
	}

	bool MostBit64(const uint64_t value, int& out) {
		if (value == 0) return false;
		uint64_t v = value;
		v |= (v >> 1);
		v |= (v >> 2);
		v |= (v >> 4);
		v |= (v >> 8);
		v |= (v >> 16);
		v |= (v >> 32);
		out = CountBit64(v) - 1;
		return true;
	}

	bool LeastBit8(const uint8_t value, int& out) {
		if (value == 0) return false;
		uint8_t v = value;
		v |= (v << 1);
		v |= (v << 2);
		v |= (v << 4);
		out = 8 - CountBit8(v);
		return true;
	}

	bool LeastBit16(const uint16_t value, int& out) {
		if (value == 0) return false;
		uint16_t v = value;
		v |= (v << 1);
		v |= (v << 2);
		v |= (v << 4);
		v |= (v << 8);
		out = 16 - CountBit16(v);
		return true;
	}

	bool LeastBit32(const uint32_t value, int& out) {
		if (value == 0) return false;
		uint32_t v = value;
		v |= (v << 1);
		v |= (v << 2);
		v |= (v << 4);
		v |= (v << 8);
		v |= (v << 16);
		out = 32 - CountBit32(v);
		return true;
	}

	bool LeastBit64(const uint64_t value, int& out) {
		if (value == 0) return false;
		uint64_t v = value;
		v |= (v << 1);
		v |= (v << 2);
		v |= (v << 4);
		v |= (v << 8);
		v |= (v << 16);
		v |= (v << 32);
		out = 64 - CountBit64(v);
		return true;
	}


	std::vector<std::vector<std::string>> LoadCsv(const std::string& file_path) {

		std::vector<std::vector<std::string>> ret;

		FILE* fp = nullptr;
		fopen_s(&fp, file_path.c_str(), "r");

		char buff[1024] = { 0 };
		while (fgets(buff, sizeof(buff), fp)) {
			std::string line = buff;
			std::vector<std::string> data;

			while (1) {
				size_t c = line.find(",");
				if (c == std::string::npos) {
					c = line.find("\n");
				}
				std::string s = line.substr(0, c);
				data.emplace_back(std::move(s));
				line = line.substr(c+1, line.length()-(c+1));
				if (line.empty() || line == "/n") break;
			}
			ret.emplace_back(std::move(data));
			memset(buff, 0, sizeof(buff));
		}

		fclose(fp);
		return ret;
	}


}

