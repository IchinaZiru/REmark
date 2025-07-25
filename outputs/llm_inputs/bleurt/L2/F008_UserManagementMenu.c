void UserManagementMenu(UserPtr *UserList);
void RoomManagementMenu(RoomPtr *RoomList);
void ReservationManagementMenu(ReservationPtr *ReservationList, UserPtr UserList, RoomPtr *RoomList);

// [Logical] この関数は、終了画面を表示するためのものです。
// [Precise] 画面をクリアした後、プロジェクトの終了メッセージと作成者の情報を表示します。
void ExitScreen()
{
  system("cls");
  printf("\n\t\t\t\t\t\t*********************************\n");
  printf("\t\t\t\t\t\t\t   Exist Screen");
  printf("\n\t\t\t\t\t\t*********************************\n");
  printf("\t\t\t\t\t\t  Project Done by:\n");
  printf("\n\t\t\t\t\t\t  - Name   : Oussama Azzouz");
  printf("\n\t\t\t\t\t\t  - Group  : 05\n\n\n");
}
