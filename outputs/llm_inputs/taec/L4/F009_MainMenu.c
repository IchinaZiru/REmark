// [Logical] この関数は、ユーザーにメインメニューを表示し、選択に応じて異なる管理メニューを呼び出す役割を持ちます。
// [Precise] メニューを表示し、ユーザーの選択を受け取り、その選択に基づいて適切な管理メニュー関数を呼び出します。
// [Unambiguous] ユーザーの選択に基づいて、ユーザー管理、部屋管理、予約管理の各メニューを呼び出し、選択が4の場合はプログラムを終了します。
// [Exhaustive] 選択が1〜3の場合は対応する管理メニューを呼び出し、4の場合は終了し、それ以外はエラーメッセージを表示します。
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
