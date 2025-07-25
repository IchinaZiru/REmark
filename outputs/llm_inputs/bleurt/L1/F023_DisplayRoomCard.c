// [Logical] この関数は、指定された部屋の情報をフォーマットして表示します。
void DisplayRoomCard(RoomPtr Room)
{
  printf("\n______________________________\n");
  printf(" - ID: %d", Room->ID);
  printf("\n - Name: %s", Room->Name);
  if (Room->IsReserved == true)
  {
    printf("\n - Reserved : Yes");
    printf("\n    => Date: %02d/%02d/%d",
           Room->Date.Day, Room->Date.Month, Room->Date.Year);
    printf("\n    => Time: %02d:%02d - %02d:%02d",
           Room->Start_Time.Hour, Room->Start_Time.Minute,
           Room->End_Time.Hour, Room->End_Time.Minute);
  }
  else
  {
    printf("\n - Reserved : No");
  }
  printf("\n______________________________\n");
}
