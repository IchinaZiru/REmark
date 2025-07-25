// [Logical] この関数は、ユーザーリストから指定された名前を持つユーザーを検索し、見つかった場合にその情報を表示します。
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
