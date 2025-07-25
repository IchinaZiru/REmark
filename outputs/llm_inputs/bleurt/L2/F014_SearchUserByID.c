// [Logical] この関数は、ユーザーリストから指定されたIDを持つユーザーを検索する。
// [Precise] リストを先頭から順に走査し、各ユーザーのIDを比較して一致した場合にそのユーザーを返す。
UserPtr SearchUserByID(UserPtr UserList, int ID)
{
  UserPtr current = UserList;
  while (current != NULL)
  {
    if (current->ID == ID)
      return current;

    current = current->Next;
  }
  return NULL;
}
