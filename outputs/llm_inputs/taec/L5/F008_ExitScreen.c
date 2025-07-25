// [Logical] この関数は、終了画面を表示するためのものです。
// [Precise] 画面をクリアした後、プロジェクトの終了メッセージと作成者の情報を表示します。
// [Unambiguous] 画面をクリアし、特定のフォーマットで終了メッセージを表示します。
// [Exhaustive] この関数は画面をクリアし、固定のメッセージを表示するだけで、戻り値や他の影響はありません。
// [Troubleshooting] この関数自体にはエラー処理は含まれておらず、`system("cls")`が失敗した場合の対処はありません。
void ExitScreen()
{
  system("cls");
  printf("\n\t\t\t\t\t\t*********************************\n");
  printf("\t\t\t\t\t\t\t   Exist Screen");
  printf("\n\t\t\t\t\t\t*********************************\n");
  printf("\t\t\t\t\t\t  Project Done by:\n");
  printf("\n\t\t\t\t\t\t  - Name   : Oussama Azzouz");
  printf("\n\t\t\t\t\t\t  - Group  : 05\n\n\n");
}
