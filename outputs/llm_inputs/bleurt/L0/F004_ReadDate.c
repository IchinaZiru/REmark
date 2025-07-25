stDate ReadDate(char Message[MAX_STRING_SIZE])
{
  stDate Date;
  printf("\n%s", Message);
  scanf("%d %d %d", &Date.Day, &Date.Month, &Date.Year);
  return Date;
}
