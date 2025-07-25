// [Logical] この関数は、2つの日付を比較し、最初の日付が2番目の日付よりも前であるかを判定します。
// [Precise] 年、月、日の順に比較し、最初に異なる値を持つ単位で判定を行います。
// [Unambiguous] 年が異なる場合は年を、年が同じ場合は月を、月も同じ場合は日を比較して判定します。
// [Exhaustive] 最初の日付が2番目の日付より前の場合はtrueを返し、それ以外の場合はfalseを返します。
// [Troubleshooting] この関数はエラー処理を行わないため、入力データの妥当性は呼び出し元で確認する必要があります。
bool IsDate1BeforeDate2(stDate Date1, stDate Date2)
{
  if (Date1.Year < Date2.Year)
    return true;
  else if (Date1.Year == Date2.Year)
  {
    if (Date1.Month < Date2.Month)
      return true;
    else if (Date1.Month == Date2.Month)
    {
      if (Date1.Day < Date2.Day)
        return true;
    }
  }
  return false;
}
