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
