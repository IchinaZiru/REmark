// [Logical] この関数は、指定されたタイトルを画面のヘッダーとして表示します。
void ScreenHeader(char Title[MAX_STRING_SIZE])
{
  system("cls");
  printf("\n******************************\n");
  printf("%s", Title);
  printf("\n******************************\n");
}
