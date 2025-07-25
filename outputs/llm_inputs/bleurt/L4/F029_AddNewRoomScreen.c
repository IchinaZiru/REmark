// [Logical] この関数は、新しい部屋をリストに追加するための画面を表示し、部屋情報を入力してリストに追加します。
// [Precise] 新しい部屋の情報をメモリに確保し、入力された情報を元に部屋IDを検索し、存在しなければリストに追加します。
// [Unambiguous] 部屋IDが既に存在する場合は追加せず、存在しない場合のみ新しい部屋をリストに追加します。
// [Exhaustive] 部屋が追加された場合は成功メッセージを表示し、既存の場合はエラーメッセージを表示します。メモリ確保や入力時のエラー処理は行っていません。
void AddNewRoomScreen(RoomPtr *RoomList)
{
  ScreenHeader("\tAdd New Room");

  RoomPtr NewRoom = (Room *)malloc(sizeof(Room));
  ReadRoomInfo(&NewRoom);

  if (SearchRoomByID(*RoomList, NewRoom->ID) == NULL)
  {
    *RoomList = AddNewRoom(*RoomList, NewRoom);
    printf("\nRoom Added Successfully!\n");
  }
  else
    printf("\nRoom with ID [%d] already exist!\n", NewRoom->ID);

  PauseProgram();
}
