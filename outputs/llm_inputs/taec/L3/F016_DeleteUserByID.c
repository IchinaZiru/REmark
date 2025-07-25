// [Logical] この関数は、指定されたIDを持つユーザーをリンクリストから削除します。
// [Precise] リンクリストを線形探索し、指定されたIDを持つノードを見つけて削除し、リストを更新します。
// [Unambiguous] 指定されたIDを持つユーザーが見つかると、そのノードをリストから削除し、メモリを解放します。
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
