// [Logical] この関数は、ユーザーIDを入力して該当するユーザーを検索し、その結果を表示する。
// [Precise] ユーザーにIDを入力させ、そのIDを用いてユーザーリストからユーザーを検索し、見つかった場合はユーザー情報を表示する。
// [Unambiguous] ユーザーIDを入力し、該当するユーザーが見つかればその情報を表示し、見つからなければ見つからなかった旨を表示する。
// [Exhaustive] ユーザーが見つからない場合はエラーメッセージを表示し、見つかった場合はユーザー情報を表示する。
// [Troubleshooting] ユーザーが見つからない場合は"User not Found!"と表示し、プログラムは続行される。
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
