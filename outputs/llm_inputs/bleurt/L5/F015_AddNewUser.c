// [Logical] この関数は、新しいユーザーを既存のユーザーリストの末尾に追加します。
// [Precise] ユーザーリストが空の場合は新しいユーザーを返し、そうでない場合はリストの末尾に新しいユーザーを追加します。
// [Unambiguous] ユーザーリストがNULLの場合、新しいユーザーがリストの先頭となり、そうでない場合はリストの最後に新しいユーザーが追加されます。
// [Exhaustive] ユーザーリストがNULLの場合は新しいユーザーを返し、正常に追加された場合は更新されたユーザーリストを返します。
// [Troubleshooting] ユーザーリストや新しいユーザーがNULLであることを考慮しており、NULLチェックを行うことでエラーを回避します。
UserPtr AddNewUser(UserPtr UserList, UserPtr NewUser)
{
  if (UserList == NULL)
  {
    return NewUser;
  }

  UserPtr current = UserList;
  while (current->Next != NULL)
  {
    current = current->Next;
  }
  current->Next = NewUser;
  return UserList;
}
