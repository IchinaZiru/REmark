stTime ReadTime(char Message[MAX_STRING_SIZE])
{
  stTime Time;
  printf("\n%s", Message);
  scanf("%d %d", &Time.Hour, &Time.Minute);
  return Time;
}
