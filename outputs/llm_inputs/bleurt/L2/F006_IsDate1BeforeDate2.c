// [Logical] この関数は、2つの日付を比較し、最初の日付が2番目の日付よりも前であるかを判定します。
// [Precise] 年、月、日の順に比較し、最初に異なる値を持つ単位で判定を行います。
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
