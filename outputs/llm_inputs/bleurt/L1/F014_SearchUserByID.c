// [Logical] この関数は、ユーザーリストから指定されたIDを持つユーザーを検索する。
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
