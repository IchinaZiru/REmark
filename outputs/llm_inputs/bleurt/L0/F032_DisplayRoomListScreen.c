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
