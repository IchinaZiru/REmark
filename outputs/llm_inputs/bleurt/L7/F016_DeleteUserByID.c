// [Logical] この関数は、指定されたIDを持つユーザーをリンクリストから削除します。
// [Precise] リンクリストを線形探索し、指定されたIDを持つノードを見つけて削除し、リストを更新します。
// [Unambiguous] 指定されたIDを持つユーザーが見つかると、そのノードをリストから削除し、メモリを解放します。
// [Exhaustive] ユーザーが見つからない場合はリストに変更はなく、見つかった場合はそのユーザーを削除して更新されたリストを返します。
// [Troubleshooting] 削除対象のユーザーが見つからない場合は何も削除せず、メモリ解放時にNULLポインタを渡さないように注意が必要です。
// [Contextualizing] この関数は、ユーザー管理システムなどでユーザー情報を動的に管理する際に使用されます。
// [Condensing] 指定されたIDのユーザーをリンクリストから削除し、リストを更新して返します。
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
