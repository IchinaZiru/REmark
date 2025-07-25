// [Logical] この関数は、ユーザー管理メニューを表示し、ユーザーに関連する操作を選択して実行します。
// [Precise] ユーザーはメニューから選択し、選択に応じたユーザー管理操作が実行されます。
// [Unambiguous] ユーザーは1から6の選択肢を入力し、それに応じた機能が呼び出されます。
void UserManagementMenu(UserPtr *UserList)
{
  int Choice = 0;
  do
  {
    system("cls");
    printf("\n---------------------------------------------\n");
    printf("\t   User Management Menu");
    printf("\n---------------------------------------------\n");
    printf("  [1] Add New User.");
    printf("\n  [2] Search by First Name.");
    printf("\n  [3] Search by ID.");
    printf("\n  [4] Display Users.");
    printf("\n  [5] Delete User.");
    printf("\n  [6] Main Menu.");
    printf("\n---------------------------------------------\n");
    printf(" - Please enter your choice: ");
    Choice = ReadIntBetween(1, 6);
    switch (Choice)
    {
    case 1:
      AddNewUserScreen(UserList);
      break;
    case 2:
      SearchUserByFirstNameScreen(*UserList);
      break;
    case 3:
      SearchUserByIDScreen(*UserList);
      break;
    case 4:
      DisplayUserListScreen(*UserList);
      break;
    case 5:
      DeleteUserScreen(UserList);
    case 6:
      break;
    default:
      printf("\nError!");
      break;
    }
  } while (Choice != 6);
}
