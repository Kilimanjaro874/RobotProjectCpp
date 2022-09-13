//
//  json.h
//
//  Created by T.Tsuneki
//

#pragma once

#include <iostream>
#include "picojson.h"
#include "tnl_using.h"
#include "tnl_hierarchy_tree.h"

#if 0 // 削除予定
namespace tnl {

/*
* 
*  使用法サンプル
* 

----------------------------------------------------------------------------------------
    json_sample.json

{"root": {
  "id" : "file",
  "value" : "file",
  "visible" : {
    "menu_element" : [
      {"value" : "New", "onclick" : "CreateNewDoc()", "ary0" : "A"},
      {"value" : "Open", "onclick" :"OpenDoc()", "ary1" : "B"},
      {"value" : "Close", "onclick" :"CloseDoc()", "ary2" : "C"}
    ]
  }
}}

----------------------------------------------------------------------------------------
    main.cpp

int main()
{
    Shared<tnl::Json> root = tnl::Json::CreateFromFile("../json_sample.json");

    std::vector<Shared<tnl::Json>> elements = root->getElements("menu_element");

    Shared<tnl::Json> elm = std::static_pointer_cast<tnl::Json > (elements[0]->getChild());
    elm->roundupLinear([]( tnl::hierarchy_tree::shared obj ) {
         Shared<tnl::Json> node = std::static_pointer_cast<tnl::Json>( obj );
         printf( "%s : %s \n", node->getTagName().c_str(), node->getText().c_str());
    });
}

* 
*/


    //-------------------------------------------------------------------------------------------------------------------
    //
    // 
    // json 操作クラス
    // 
    //
    class Json final : public tnl::hierarchy_tree {
    public:
        Json() {}
        ~Json() {}

        //---------------------------------------------------------------------------------------------
        // arg1... json ファイルのロード
        // ret.... ファイルパス
        // ret.... パース済みのルートエレメント
        static Shared<Json> CreateFromFile(const std::string& file_path);

        //---------------------------------------------------------------------------------------------
        // arg1... json ファイルのセーブ
        // ret.... ファイルパス
        void Save(const std::string& file_path);

        //---------------------------------------------------------------------------------------------
        // arg1... エレメントの取得
        // ret.... タグ名
        // ret.... 該当するタグを持つエレメント配列
        // tips... このメソッドを実行したエレメント以下の階層で検索します
        std::vector<Shared<Json>> getElements(const std::string& find_tag);

        //---------------------------------------------------------------------------------------------
        // 
        // get
        //
        inline std::string& getTagName(){ return tag_name_; }
        inline std::string& getText(){ return text_ ; }
        inline int getInt() const { return std::atoi( text_.c_str() ); }
        inline float getFloat() const { return static_cast<float>(std::atof( text_.c_str() )) ; }
        
        //---------------------------------------------------------------------------------------------
        // 
        // set
        // 
        inline void set(const std::string& tag, const int32_t val) noexcept { text_ = tag; text_ = std::to_string(val); }
        inline void set(const std::string& tag, const float val) noexcept { text_ = tag; text_ = std::to_string(val); }
        inline void setTag(const std::string& tag) noexcept { tag_name_ = tag; }
        inline void setText(const int32_t val) noexcept { text_ = std::to_string(val); }
        inline void setText(const float val) noexcept { text_ = std::to_string(val); }
        inline void setText(const std::string& txt) noexcept { text_ = txt; }        

    private:
        std::string	tag_name_ = "";
        std::string	text_ = "";
        std::vector<Shared<Json>> array_;
        static Shared<Json> LoadParseProcess(const std::string& file_path, const char* reader, Shared<Json> parent, picojson::value& value, const bool root = true);
        static void SaveParseProcess(FILE* fp, Shared<Json> elem, int depth, const bool is_root);

    } ;

}

#endif