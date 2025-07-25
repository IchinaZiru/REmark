// [Logical] この関数は、指定された範囲内の整数をユーザーから入力として取得します。
// [Precise] ユーザーから整数を入力させ、その値が指定された範囲内に収まるまで再入力を促します。
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
