// [Logical] この関数は、指定されたIDの部屋をリストから削除するための画面を表示し、削除操作を実行します。
// [Precise] ユーザーに部屋IDを入力させ、そのIDの部屋を検索し、見つかった場合は削除の確認を求め、確認後に削除を実行します。
void DeleteRoomScreen(RoomPtr *RoomList)
{
  ScreenHeader("\tDelete Room");
  int ID;
  printf("\nPlease enter ID: ");
  ID = ReadIntBetween(0, 9999);
  RoomPtr Room = SearchRoomByID(*RoomList, ID);
  if (Room != NULL)
  {
    DisplayRoomCard(Room);
    char Choice = 'n';
    printf("\n\nAre you sure you want to delete this room? y/n? ");
    scanf(" %c", &Choice);
    if (Choice == 'y')
    {
      *RoomList = DeleteRoomByID(*RoomList, ID);
      printf("\nRoom with ID [%d] deleted successfully!\n", ID);
    }
  }
  else
    printf("\nRoom with ID [%d] not found!\n", ID);

  PauseProgram();
}
