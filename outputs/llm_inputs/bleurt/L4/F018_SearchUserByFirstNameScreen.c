// [Logical] この関数は、ユーザーのリストから指定された名前を持つユーザーを検索し、結果を表示します。
// [Precise] ユーザーに名前の入力を求め、その名前でユーザーリストを検索し、結果を表示します。
// [Unambiguous] 入力された名前でユーザーリストを検索し、見つからなければメッセージを表示します。
// [Exhaustive] ユーザーが見つかった場合は何も表示せず、見つからなければ「User not Found!」と表示します。
void SearchUserByFirstNameScreen(UserPtr UserList)
{
  ScreenHeader(" Search By First Name");

  bool IsExist = false;
  char FirstName[MAX_STRING_SIZE];
  printf("\nPlease enter first name: ");
  scanf("%s", FirstName);

  SearchUserByFirstName(UserList, FirstName, &IsExist);

  if (IsExist == false)
    printf("\nUser not Found!\n");

  PauseProgram();
}
