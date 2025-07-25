// [Logical] この関数は、ユーザーにメインメニューを表示し、選択に応じて異なる管理メニューを呼び出す役割を持ちます。
void MainMenu()
{
  UserPtr UserList = NULL;
  RoomPtr RoomList = NULL;
  ReservationPtr ReservationList = NULL;

  int Choice = 0;
  do
  {
    system("cls");
    printf("\n\n=============================================\n");
    printf("\t\tMain Menu");
    printf("\n=============================================\n");
    printf("  [1] User Management Menu.");
    printf("\n  [2] Room Management Menu.");
    printf("\n  [3] Reservation Management Menu.");
    printf("\n  [4] Exit.");
    printf("\n=============================================\n");
    printf(" - Please enter your choice: ");
    Choice = ReadIntBetween(1, 4);
    switch (Choice)
    {
    case 1:
      UserManagementMenu(&UserList);
      break;
    case 2:
      RoomManagementMenu(&RoomList);
      break;
    case 3:
      ReservationManagementMenu(&ReservationList, UserList, &RoomList);
      break;
    case 4:
      ExitScreen();
      break;
    default:
      printf("\nError!");
      break;
    }
  } while (Choice != 4);
}
