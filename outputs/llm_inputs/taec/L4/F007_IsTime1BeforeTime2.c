// [Logical] この関数は、Time1がTime2よりも前の時間かどうかを判定します。
// [Precise] Time1とTime2の時間（Hour）を比較し、同じ場合は分（Minute）を比較して判定します。
// [Unambiguous] Time1の時間がTime2よりも早い場合はtrueを返し、そうでない場合はfalseを返します。
// [Exhaustive] Time1がTime2よりも早い場合にtrueを返し、同時または遅い場合にはfalseを返します。
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
