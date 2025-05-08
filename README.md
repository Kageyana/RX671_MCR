# 初期設定
1. ワークスペースのパスは2バイト文字禁止
2. e2studio プロジェクトの設定→C/C++ビルド→設定<br>ツール設定タブ→Compiler→Includesに<br>
```"${workspace_loc:/${ProjName}/inc}"```<br>
を追加
3. 