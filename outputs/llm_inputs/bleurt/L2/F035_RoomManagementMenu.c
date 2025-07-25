// [Logical] この関数は、部屋管理メニューを表示し、ユーザーの選択に応じて適切な処理を行います。
// [Precise] ユーザーに1から7の選択肢を提示し、選択に応じて部屋の追加、検索、表示、削除、空室確認、またはメインメニューへの移行を行います。
void RoomManagementMenu(RoomPtr *RoomList)
{
  int Choice = 0;
  do
  {
    system("cls");
    printf("\n---------------------------------------------\n");
    printf("\t   Room Management Menu");
    printf("\n---------------------------------------------\n");
    printf("  [1] Add New Room.");
    printf("\n  [2] Search by Name.");
    printf("\n  [3] Search by ID.");
    printf("\n  [4] Display Rooms.");
    printf("\n  [5] Delete Room.");
    printf("\n  [6] Check is Room Availabe.");
    printf("\n  [7] Main Menu.");
    printf("\n---------------------------------------------\n");
    printf(" - Please enter your choice: ");
    Choice = ReadIntBetween(1, 7);
    switch (Choice)
    {
    case 1:
      AddNewRoomScreen(RoomList);
      break;
    case 2:
      SearchRoomByNameScreen(*RoomList);
      break;
    case 3:
      SearchRoomByIDScreen(*RoomList);
      break;
    case 4:
      DisplayRoomListScreen(*RoomList);
      break;
    case 5:
      DeleteRoomScreen(RoomList);
      break;
    case 6:
      CheckRoomAvailabilityScreen(*RoomList);
      break;
    case 7:
      break;
    default:
      printf("\nError!");
      break;
    }
  } while (Choice != 7);
}
