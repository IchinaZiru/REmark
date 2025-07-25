// [Logical] この関数はプログラムのエントリーポイントとして、メインメニューを表示するために呼び出されます。
// [Precise] MainMenu関数を呼び出し、その後プログラムを終了します。
// [Unambiguous] プログラム開始時にMainMenu関数を実行し、終了コード0を返します。
// [Exhaustive] MainMenu関数の実行後、正常終了を示す0を返します。
// [Troubleshooting] この関数自体はエラー処理を行いませんが、MainMenu関数内でのエラー処理が必要です。
// [Contextualizing] この関数はC言語プログラムの開始地点として、ユーザーにメインメニューを提示する際に使用されます。
// [Condensing] プログラム開始時にメインメニューを表示し、正常終了します。
int main()
{
  MainMenu();
  return 0;
}
