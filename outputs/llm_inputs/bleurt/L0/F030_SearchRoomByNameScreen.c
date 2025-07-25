void SearchRoomByNameScreen(RoomPtr RoomList)
{
  ScreenHeader("\tSearch by Name");

  bool IsExist = false;

  char Name[MAX_STRING_SIZE];
  printf("\nPlease enter name: ");
  scanf("%s", Name);

  SearchRoomByName(RoomList, Name, &IsExist);

  if (IsExist == false)
    printf("\nRoom not Found!\n");

  PauseProgram();
}
