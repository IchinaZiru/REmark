// [Logical] この関数は、新しいユーザーをユーザーリストに追加するための画面を表示し、ユーザー情報を入力してリストに追加します。
// [Precise] 関数は、新しいユーザーの情報を読み込み、既存のユーザーリストに同じIDのユーザーが存在しない場合にリストに追加します。
// [Unambiguous] ユーザーIDが既存のリストに存在しない場合にのみ、新しいユーザーをリストに追加します。
// [Exhaustive] ユーザーが正常に追加された場合は成功メッセージを表示し、既に存在する場合は警告メッセージを表示します。
// [Troubleshooting] ユーザーIDの重複がある場合は、ユーザーが追加されず、重複の警告メッセージが表示されます。
// [Contextualizing] この関数は、ユーザー管理システムにおいて新しいユーザーを追加する際に使用されます。
// [Condensing] 新しいユーザーをリストに追加し、成功または重複のメッセージを表示します。
void AddNewUserScreen(UserPtr *UserList)
{
  ScreenHeader("\tAdd New User");

  UserPtr NewUser = (User *)malloc(sizeof(User));
  ReadUserInfo(&NewUser);

  if (SearchUserByID(*UserList, NewUser->ID) == NULL)
  {
    *UserList = AddNewUser(*UserList, NewUser);
    printf("\nUser Added Successfully!\n");
  }
  else
  {
    printf("\nUser with ID [%d] already exist!\n", NewUser->ID);
  }

  PauseProgram();
}
