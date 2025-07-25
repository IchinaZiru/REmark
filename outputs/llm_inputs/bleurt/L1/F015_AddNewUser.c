// [Logical] この関数は、新しいユーザーを既存のユーザーリストの末尾に追加します。
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
