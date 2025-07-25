void SearchRoomByIDScreen(RoomPtr RoomList)
{
  ScreenHeader("\tSearch By ID");
  int ID;
  printf("\nPlease enter ID: ");
  ID = ReadIntBetween(0, 9999);

  RoomPtr Room = SearchRoomByID(RoomList, ID);

  if (Room == NULL)
    printf("\nRoom not Found!\n");
  else
    DisplayRoomCard(Room);

  PauseProgram();
}
