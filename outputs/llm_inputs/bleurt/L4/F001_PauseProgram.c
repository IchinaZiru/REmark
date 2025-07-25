// [Logical] この関数は、ユーザーがEnterキーを押すまでプログラムを一時停止し、メインメニューに戻るよう促します。
// [Precise] 標準出力にメッセージを表示し、ユーザーの入力を待機してEnterキーが押されると次の処理に進みます。
// [Unambiguous] ユーザーがEnterキーを押すまでプログラムの進行を停止するための関数です。
// [Exhaustive] Enterキー以外の入力を受け取ると意図した動作をしない可能性がありますが、エラー処理は実装されていません。
void PauseProgram()
{
  printf("\n\nPress Enter to Go To Main Menu...");
  char enter;
  scanf("%s", &enter);
}
