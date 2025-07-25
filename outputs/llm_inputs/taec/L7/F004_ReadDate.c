// [Logical] この関数は、ユーザーから日付を入力として受け取り、それを構造体として返します。
// [Precise] 関数は、指定されたメッセージを表示し、ユーザーから日付の3つの整数（年、月、日）を入力として受け取り、それを構造体stDateに格納して返します。
// [Unambiguous] 関数は標準入力から日付を取得し、stDate構造体に格納して返します。
// [Exhaustive] 関数はユーザーの入力に基づいてstDate構造体を返しますが、入力が不正な場合のエラーハンドリングはありません。
// [Troubleshooting] 不正な入力があった場合、scanfは期待通りに動作しない可能性があるため、入力の検証やエラーチェックを追加する必要があります。
// [Contextualizing] この関数は、ユーザーから日付情報を取得する必要があるプログラム内で使用されます。
// [Condensing] ユーザーから日付を入力させ、それをstDate構造体として返す関数です。
stDate ReadDate(char Message[MAX_STRING_SIZE])
{
  stDate Date;
  printf("\n%s", Message);
  scanf("%d %d %d", &Date.Day, &Date.Month, &Date.Year);
  return Date;
}
