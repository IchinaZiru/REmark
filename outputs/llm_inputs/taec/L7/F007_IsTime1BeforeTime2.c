// [Logical] この関数は、Time1がTime2よりも前の時間かどうかを判定します。
// [Precise] Time1とTime2の時間（Hour）を比較し、同じ場合は分（Minute）を比較して判定します。
// [Unambiguous] Time1の時間がTime2よりも早い場合はtrueを返し、そうでない場合はfalseを返します。
// [Exhaustive] Time1がTime2よりも早い場合にtrueを返し、同時または遅い場合にはfalseを返します。
// [Troubleshooting] この関数はエラーや例外を考慮していないため、入力データの整合性は事前に確認する必要があります。
// [Contextualizing] この関数は、時間の順序を確認する必要があるスケジューリングや時間管理の場面で使用されます。
// [Condensing] Time1がTime2よりも前の時間かどうかを判定する関数です。
bool IsTime1BeforeTime2(stTime Time1, stTime Time2)
{
  if (Time1.Hour < Time2.Hour)
    return true;
  else if (Time1.Hour == Time2.Hour)
  {
    if (Time1.Minute < Time2.Minute)
      return true;
  }
  return false;
}
