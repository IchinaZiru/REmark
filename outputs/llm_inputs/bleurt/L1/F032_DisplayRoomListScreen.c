// [Logical] この関数は、部屋のリストを画面に表示する機能を持つ。
void DisplayRoomListScreen(RoomPtr RoomList)
{
  ScreenHeader("\t Room List");

  if (RoomList == NULL)
  {
    printf("\nList is empty!");
  }
  else
  {
    RoomPtr current = RoomList;
    int i = 0;
    while (current != NULL)
    {
      printf("\n - Room [%d]:", ++i);
      DisplayRoomCard(current);
      current = current->Next;
    }
  }
  PauseProgram();
}
