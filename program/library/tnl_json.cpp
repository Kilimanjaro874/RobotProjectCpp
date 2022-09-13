#if 0 // 削除予定
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>

#include "tnl_util.h"
#include "tnl_json.h"
#include "tnl_shared_factory.h"

namespace tnl {

    //---------------------------------------------------------------------------------------------
    // ブロックコメント / * ~ * / とインラインコメント // ~ をすべて削除
    inline void removeComments(std::string& str) {
        std::string::size_type posOfBlockHead = 0, posOfInlineHead = 0, posOfTail, posOfCR, posOfLF;
        while ((posOfBlockHead = str.find("/*", posOfBlockHead)) != (posOfInlineHead = str.find("//", posOfInlineHead)))
        {
            if (posOfBlockHead < posOfInlineHead) {

                // ブロックコメントを削除
                posOfTail = str.find("*/", posOfBlockHead + 2);

                // 改行は残す処理
                int count = 0;
                const char* pc = str.c_str() + posOfBlockHead;
                for (size_t i = 0; i < (posOfTail - posOfBlockHead + 2); i++) {
                    if ((*(pc + i)) == '\n') count++;
                }
                for (int i = 0; i < count; i++) {
                    str.insert((posOfBlockHead + (posOfTail - posOfBlockHead + 2)), "\n", 1);
                }

                str.erase(posOfBlockHead, posOfTail - posOfBlockHead + 2);
                posOfInlineHead = posOfBlockHead;
            }
            else {
                // インラインコメントを削除
                posOfCR = str.find("\r", posOfInlineHead + 2);
                posOfLF = str.find("\n", posOfInlineHead + 2);

                // 改行は残す
                str.insert((posOfInlineHead - 1), "\n", 1);
                str.erase(posOfInlineHead + 1, posOfCR < posOfLF ? posOfCR - posOfInlineHead + 1 : posOfLF - posOfInlineHead + 1);
                posOfBlockHead = posOfInlineHead + 1;
            }
        }
    };


    //---------------------------------------------------------------------------------------------
    Shared<Json> Json::LoadParseProcess(const std::string& file_path, const char* reader, Shared<Json> parent, picojson::value& value, const bool root) {

        Shared<tnl::Json> top;

        if (root == true) {
            std::string json_str = reader;
            removeComments(json_str);
            const char* mp = reader;
            std::string err = picojson::parse(value, mp, mp + strlen(mp));

            // json のパースに失敗
            if (!err.empty()) {
                TNL_DEBUG_ASSERTE("json parse errror");
            }
        }

        if (value.is<picojson::object>()) {

            picojson::object o = value.get<picojson::object>();
            picojson::object::iterator it_obj = o.begin();

            while (it_obj != o.end()) {

                Shared<Json> new_obj = std::make_shared<Json>();
                new_obj->tag_name_ = (*it_obj).first;
                if ((*it_obj).second.is< std::string >()) {
                    new_obj->text_ = (*it_obj).second.get< std::string >();
                }
                else if ((*it_obj).second.is< double >()) {
                    new_obj->text_ = std::to_string((*it_obj).second.get< double >());
                }
                if (root && !top) {
                    parent = top = new_obj;
                }
                parent->addChild(new_obj);
                LoadParseProcess(file_path, nullptr, new_obj, (*it_obj).second, false);
                it_obj++;
            }

        }
        else if (value.is<picojson::array>()) {

            picojson::array a = value.get<picojson::array>();
            picojson::array::iterator it_a = a.begin();
 
            while (it_a != a.end()) {
                picojson::object o = (*it_a).get<picojson::object>();
                picojson::object::iterator it_obj = o.begin();

                Shared<Json> ary_obj = std::make_shared<Json>();
                parent->array_.emplace_back(ary_obj);

                while (it_obj != o.end()) {

                    Shared<Json> new_obj = std::make_shared<Json>();
                    new_obj->tag_name_ = (*it_obj).first;
                    if ((*it_obj).second.is< std::string >()) {
                        new_obj->text_ = (*it_obj).second.get< std::string >();
                    }
                    else if ((*it_obj).second.is< double >()) {
                        new_obj->text_ = std::to_string((*it_obj).second.get< double >());
                    }
                    if (root && !top) {
                        parent = top = new_obj;
                    }
                    ary_obj->addChild(new_obj);
                    LoadParseProcess(file_path, nullptr, new_obj, (*it_obj).second, false);
                    it_obj++;
                }
                it_a++;
            }

            //picojson::array ary = value.get<picojson::array>();
            //for (int i = 0; i < ary.size(); i++) {
            //    LoadParseProcess(file_path, nullptr, parent, ary[i], false);
            //}

        }

        if (root) {
            return top;
        }
        return nullptr;

    }


    //---------------------------------------------------------------------------------------------
    std::vector<Shared<Json>> Json::getElements(const std::string& find_tag) {
        std::vector<Shared<Json>> ret;
        roundup([&](Shared<tnl::hierarchy_tree> link) {
            Shared<tnl::Json> node = std::static_pointer_cast<tnl::Json>(link);
            if (node->getTagName() != find_tag) return ;
            ret.emplace_back(node);
        });
        return ret;
    }


    //---------------------------------------------------------------------------------------------
    Shared<Json> Json::CreateFromFile(const std::string& file_path) {

        long long int file_size = GetFileSize(file_path.c_str());
        FILE* fp = nullptr;
        char* buff = nullptr;
        auto err = fopen_s(&fp, file_path.c_str(), "r");
        if (err != 0) {
            // error
            return nullptr;
        }
        buff = new char[ (uint32_t)file_size ];
        fread_s(buff, (size_t)file_size, (size_t)file_size, 1, fp);
        fclose(fp);

        picojson::value v;
        Shared<tnl::Json> root = tnl::Json::LoadParseProcess("", buff, root, v);
        delete[] buff;
        return root;
    }

    //---------------------------------------------------------------------------------------------
    static void write( const std::string& str, int depth, FILE* fp) {
        std::string s = "";
        for (int i = 0; i < depth*4; ++i) s += " ";
        s += str;
        fwrite(s.c_str(), s.length(), 1, fp);
    }
    //---------------------------------------------------------------------------------------------
    void Json::SaveParseProcess(FILE* fp, Shared<Json> elem, int depth, bool is_root) {

        write("", depth, fp);
        if (is_root) write("{\n", 0, fp);

        if (!elem->getTagName().empty()) write('"' + elem->getTagName() + '"' + " : ", 0, fp);
        if (!elem->getText().empty()) write('"' + elem->getText() + '"', 0, fp);

        if (!elem->array_.empty()) {
            write("[\n", 0, fp);
            for (uint32_t i = 0; i < elem->array_.size(); ++i) {
                SaveParseProcess(fp, std::static_pointer_cast<Json>(elem->array_[i]), depth+1, false);
                if(i < elem->array_.size()-1) write(",\n", depth, fp);
            }
            write("]\n", depth, fp);
        }

        if (elem->getChild()) {
            write("{\n", 0, fp);
            SaveParseProcess(fp, std::static_pointer_cast<Json>(elem->getChild()), depth+1, false);
            write("}\n", depth, fp);
        }
        if (elem->getNext()) {
            write(",\n", 0, fp);
            SaveParseProcess(fp, std::static_pointer_cast<Json>(elem->getNext()), depth, false);
        }

        if (is_root) write("}\n", 0, fp);
    }

    //---------------------------------------------------------------------------------------------
    void Json::Save(const std::string& file_path) {
        FILE* fp = nullptr;
        fopen_s(&fp, file_path.c_str(), "w");
        if (!fp) {
            tnl::WarningMassage("Json Save Error : %s", file_path.c_str() );
            return;
        }
        SaveParseProcess(fp, std::static_pointer_cast<Json>(shared_from_this()), 0, true);
        fclose(fp);
    }


}

#endif