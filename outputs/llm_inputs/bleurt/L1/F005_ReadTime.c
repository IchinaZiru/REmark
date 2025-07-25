// [Logical] この関数は、ユーザーから時間を入力として受け取り、stTime構造体として返します。
stTime ReadTime(char Message[MAX_STRING_SIZE])
{
  stTime Time;
  printf("\n%s", Message);
  scanf("%d %d", &Time.Hour, &Time.Minute);
  return Time;
}
