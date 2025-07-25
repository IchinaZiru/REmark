// [Logical] この関数は、ユーザーがEnterキーを押すまでプログラムを一時停止し、メインメニューに戻るよう促します。
// [Precise] 標準出力にメッセージを表示し、ユーザーの入力を待機してEnterキーが押されると次の処理に進みます。
void PauseProgram()
{
  printf("\n\nPress Enter to Go To Main Menu...");
  char enter;
  scanf("%s", &enter);
}
