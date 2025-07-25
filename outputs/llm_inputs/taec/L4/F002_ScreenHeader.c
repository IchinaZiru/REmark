// [Logical] この関数は、指定されたタイトルを画面のヘッダーとして表示します。
// [Precise] 画面をクリアした後、タイトルを囲む装飾ラインと共に標準出力に表示します。
// [Unambiguous] 関数は、画面をクリアし、タイトルを装飾ラインで囲んで表示します。
// [Exhaustive] 戻り値はなく、標準出力に影響を与えますが、エラー処理は行っていません。
void ScreenHeader(char Title[MAX_STRING_SIZE])
{
  system("cls");
  printf("\n******************************\n");
  printf("%s", Title);
  printf("\n******************************\n");
}
