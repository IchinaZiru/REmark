// [Logical] ユーザーリストを画面に表示する関数です。
// [Precise] ユーザーリストが空でない場合、リスト内の各ユーザーを順に表示し、空の場合はその旨を表示します。
// [Unambiguous] ユーザーリストがNULLの場合は空であると表示し、そうでない場合は各ユーザーを順に表示します。
void DisplayUserListScreen(UserPtr UserList)
{
  ScreenHeader("\tUser List");

  if (UserList == NULL)
  {
    printf("\nList is empty!");
  }
  else
  {
    UserPtr current = UserList;
    int i = 0;
    while (current != NULL)
    {
      printf("\n - User [%d]:", ++i);
      DisplayUserCard(current);
      current = current->Next;
    }
  }
  PauseProgram();
}
