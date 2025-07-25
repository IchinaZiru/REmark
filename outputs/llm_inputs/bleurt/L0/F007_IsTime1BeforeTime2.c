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
