// [Logical] この関数は、指定された範囲内の整数をユーザーから入力として取得します。
// [Precise] ユーザーから整数を入力させ、その値が指定された範囲内に収まるまで再入力を促します。
// [Unambiguous] ユーザーが入力した整数が範囲外の場合、範囲内の整数が入力されるまで再度入力を求めます。
// [Exhaustive] 範囲内の整数が入力されると、その整数を返します。範囲外の入力があった場合は再入力を促します。
// [Troubleshooting] 範囲外の入力があった場合、範囲内の整数が入力されるまでプロンプトを表示し続けます。
int ReadIntBetween(int from, int to)
{
  int choice = 0;
  scanf("%d", &choice);
  while (choice < from || choice > to)
  {
    printf("\nPlease enter a number in the range [%d-%d]: ", from, to);
    scanf("%d", &choice);
  }
  return choice;
}
