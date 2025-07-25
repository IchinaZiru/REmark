// [Logical] この関数は、ユーザーリストから指定された名前を持つユーザーを検索し、見つかった場合にその情報を表示します。
// [Precise] ユーザーリストを線形探索し、指定された名前と一致するユーザーを見つけた場合にその情報を表示し、フラグをtrueに設定します。
// [Unambiguous] ユーザーリストを先頭から末尾まで順に確認し、一致する名前が見つかるとそのユーザー情報を表示し、フラグを更新します。
void SearchUserByFirstName(UserPtr UserList, char FirstName[MAX_STRING_SIZE], bool *IsExist)
{
  UserPtr current = UserList;
  while (current != NULL)
  {
    if (strcmp(current->FirstName, FirstName) == 0)
    {
      DisplayUserCard(current);
      *IsExist = true;
    }
    current = current->Next;
  }
}
