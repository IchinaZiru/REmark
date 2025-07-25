// [Logical] この関数は、指定された部屋の情報をフォーマットして表示します。
// [Precise] 部屋のID、名前、予約状況、および予約されている場合はその日時をprintf関数で出力します。
// [Unambiguous] 部屋が予約されているかどうかに応じて、予約の詳細を含むかどうかが決まります。
// [Exhaustive] 部屋が予約されている場合は、予約の日付と時間を表示し、予約されていない場合はその旨を表示します。
// [Troubleshooting] この関数にはエラー処理が含まれておらず、NULLポインタが渡された場合に未定義の動作を引き起こす可能性があります。
// [Contextualizing] この関数は、部屋の詳細をユーザーに表示するために、ユーザーインターフェースやデバッグの場面で使用されます。
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
