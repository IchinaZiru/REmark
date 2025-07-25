// [Logical] ユーザーリストを画面に表示する関数です。
// [Precise] ユーザーリストが空でない場合、リスト内の各ユーザーを順に表示し、空の場合はその旨を表示します。
// [Unambiguous] ユーザーリストがNULLの場合は空であると表示し、そうでない場合は各ユーザーを順に表示します。
// [Exhaustive] ユーザーリストがNULLの場合は"List is empty!"を表示し、各ユーザーを表示した後にプログラムを一時停止します。
// [Troubleshooting] ユーザーリストがNULLの場合は"List is empty!"と表示されるため、リストが空であることが明示されます。
// [Contextualizing] ユーザー情報を一覧表示する画面で使用される関数です。
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
