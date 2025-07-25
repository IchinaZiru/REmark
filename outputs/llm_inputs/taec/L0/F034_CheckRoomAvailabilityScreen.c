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
