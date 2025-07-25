// [Logical] この関数は、ユーザーリストから指定されたIDのユーザーを削除するための画面を表示します。
// [Precise] ユーザーIDを入力し、該当ユーザーを検索して表示、削除の確認を行い、削除が承認された場合はユーザーリストから削除します。
// [Unambiguous] ユーザーIDを入力し、存在する場合は削除確認を行い、承認されれば削除、存在しない場合はエラーメッセージを表示します。
// [Exhaustive] ユーザーが存在し削除された場合は成功メッセージを表示し、存在しない場合はエラーメッセージを表示します。
// [Troubleshooting] ユーザーが見つからない場合はエラーメッセージを表示し、削除は行われません。
// [Contextualizing] この関数は、ユーザー管理システムの一部として、ユーザーを削除する際に使用されます。
// [Condensing] 指定されたIDのユーザーを削除するための確認と処理を行う関数です。
void DeleteUserScreen(UserPtr *UserList)
{
  ScreenHeader("\tDelete User");

  int ID;
  printf("\nPlease enter ID: ");
  ID = ReadIntBetween(0, 9999);

  UserPtr User = SearchUserByID(*UserList, ID);
  if (User != NULL)
  {
    DisplayUserCard(User);
    char Choice = 'n';
    printf("\n\nAre you sure you want to delete this user? y/n? ");
    scanf(" %c", &Choice);
    if (Choice == 'y')
    {
      *UserList = DeleteUserByID(*UserList, ID);
      printf("\nUser with ID [%d] deleted successfully!\n", ID);
    }
  }
  else
    printf("\nUser wiht ID [%d] not found!\n", ID);

  PauseProgram();
}
