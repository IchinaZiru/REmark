// [Logical] この関数は、指定されたIDを持つユーザーをリンクリストから削除します。
UserPtr DeleteUserByID(UserPtr UserList, int ID)
{
  UserPtr current = UserList;
  UserPtr previous = NULL;

  while (current != NULL && current->ID != ID)
  {
    previous = current;
    current = current->Next;
  }

  if (previous == NULL)
    UserList = current->Next;
  else
    previous->Next = current->Next;

  free(current);
  return UserList;
}
