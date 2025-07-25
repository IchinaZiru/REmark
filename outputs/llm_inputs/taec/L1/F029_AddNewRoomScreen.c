// [Logical] この関数は、新しい部屋をリストに追加するための画面を表示し、部屋情報を入力してリストに追加します。
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
