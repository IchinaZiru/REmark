// [Logical] この関数は、ユーザーから日付を入力として受け取り、それを構造体として返します。
stDate ReadDate(char Message[MAX_STRING_SIZE])
{
  stDate Date;
  printf("\n%s", Message);
  scanf("%d %d %d", &Date.Day, &Date.Month, &Date.Year);
  return Date;
}
