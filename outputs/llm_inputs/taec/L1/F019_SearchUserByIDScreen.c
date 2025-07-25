// [Logical] この関数は、ユーザーIDを入力して該当するユーザーを検索し、その結果を表示する。
void SearchUserByIDScreen(UserPtr UserList)
{
  ScreenHeader("\tSearch By ID");
  int ID;
  printf("\nPlease enter ID: ");
  ID = ReadIntBetween(0, 9999);

  UserPtr User = SearchUserByID(UserList, ID);
  if (User == NULL)
    printf("\nUser not Found!\n");
  else
  {
    DisplayUserCard(User);
  }
  PauseProgram();
}
