// [Logical] この関数は、指定されたIDの部屋をリストから削除するための画面を表示し、削除操作を実行します。
// [Precise] ユーザーに部屋IDを入力させ、そのIDの部屋を検索し、見つかった場合は削除の確認を求め、確認後に削除を実行します。
// [Unambiguous] 部屋IDを入力し、存在する場合は削除確認を行い、ユーザーが承認した場合にのみ削除を実行します。
// [Exhaustive] 部屋が見つかれば削除し、成功メッセージを表示し、見つからなければエラーメッセージを表示します。
// [Troubleshooting] 部屋が見つからない場合はエラーメッセージを表示し、削除操作は行われません。
// [Contextualizing] この関数は、部屋管理システムで部屋を削除する際に使用されます。
// [Condensing] 指定されたIDの部屋を削除するための確認と操作を行う関数です。
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
