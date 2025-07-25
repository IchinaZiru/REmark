// [Logical] この関数は予約管理メニューを表示し、ユーザーの選択に応じた操作を実行します。
// [Precise] メニューを表示し、ユーザーの選択を受け取り、選択に応じて予約の追加、削除、表示、日付でのソートを行います。
// [Unambiguous] ユーザーが1から5の選択肢を入力し、その選択に基づいて対応する予約管理操作を実行します。
// [Exhaustive] 1から4の選択肢はそれぞれ予約追加、削除、表示、ソートを行い、5はメニューを終了します。範囲外の選択はエラーメッセージを表示します。
void ReservationManagementMenu(ReservationPtr *ReservationList, UserPtr UserList, RoomPtr *RoomList)
{
  int Choice = 0;
  do
  {
    system("cls");
    printf("\n---------------------------------------------\n");
    printf("\t Reservation Management Menu");
    printf("\n---------------------------------------------\n");
    printf("  [1] Add New Reservation.");
    printf("\n  [2] Delete Reservation.");
    printf("\n  [3] Display Reservations.");
    printf("\n  [4] Sort by Date.");
    printf("\n  [5] Main Menu.");
    printf("\n---------------------------------------------\n");
    printf(" - Please enter your choice: ");
    Choice = ReadIntBetween(1, 5);
    switch (Choice)
    {
    case 1:
      AddNewReservationScreen(ReservationList, UserList, RoomList);
      break;
    case 2:
      DeleteReservationScreen(ReservationList, RoomList);
      break;
    case 3:
      DisplayReservationsScreen(*ReservationList);
      break;
    case 4:
      SortReservationsByDateScreen(ReservationList);
      break;
    case 5:
      break;
    default:
      printf("\nError!");
      break;
    }
  } while (Choice != 5);
}