// [Logical] この関数は、部屋の空き状況を名前またはIDで確認するための画面を表示します。
// [Precise] ユーザーに名前またはIDでの検索方法を選択させ、その選択に基づいて対応する検索関数を呼び出します。
// [Unambiguous] ユーザーの選択に応じて、名前での検索またはIDでの検索を行うことができるように分岐処理を行います。
void CheckRoomAvailabilityScreen(RoomPtr RoomList)
{
  ScreenHeader("  Check Room Availability");

  int Choice = 0;
  printf("\nChoose how you want to check:\n[1] Name.\t[2] ID.\n\n => ");
  Choice = ReadIntBetween(1, 2);
  switch (Choice)
  {
  case 1:
    SearchRoomByNameScreen(RoomList);
    break;
  case 2:
    SearchRoomByIDScreen(RoomList);

  default:
    printf("\nInvalid Choice!");
    break;
  }
}
