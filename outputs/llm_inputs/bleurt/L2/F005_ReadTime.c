// [Logical] この関数は、ユーザーから時間を入力として受け取り、stTime構造体として返します。
// [Precise] 関数はメッセージを表示し、ユーザーから時間（時と分）を標準入力から取得してstTime構造体に格納します。
stTime ReadTime(char Message[MAX_STRING_SIZE])
{
  stTime Time;
  printf("\n%s", Message);
  scanf("%d %d", &Time.Hour, &Time.Minute);
  return Time;
}
