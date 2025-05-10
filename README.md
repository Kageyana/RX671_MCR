# 初期設定
1. ワークスペースのパスは2バイト文字禁止
2. e2studio プロジェクトの設定→C/C++ビルド→設定<br>ツール設定タブ→Compiler→Includesに<br>
```"${workspace_loc:/${ProjName}/inc}"```<br>
を追加
3. Renesas Extensions for Visual Studio Codeを使う場合<br>
	1. 下記ページを参考に拡張機能をインストールする
	https://qiita.com/y_toyoyama/items/98b7511f9f95343f7a40<br>
	2. launch.json<br>
	serverParametersはデバッガ情報をLLMに渡して整形してもらうと楽
	3. CMakeLists.txt<br>
	CMAKE_CURRENT_SOURCE_DIRに```"inc/*.h"```を追加<br>
	CMAKE_CXX_FLAGSにコンパイラオプションを追加する<br>
	CMAKE_EXE_LINKER_FLAGSにリンカオプションを追加する
	4. .clangd<br>
	下記リポジトリを参考に.clangdとc_cpp_intellisense_helper.hを追加する<br>
	https://github.com/NoMaY-jp/Renesas_VSCode_Extensions_tips_for_Renesas_CC_compilers.git