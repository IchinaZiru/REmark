// [Logical] この関数は、ユーザーリストから指定されたIDを持つユーザーを検索する。
// [Precise] リストを先頭から順に走査し、各ユーザーのIDを比較して一致した場合にそのユーザーを返す。
// [Unambiguous] ユーザーIDが一致した場合はそのユーザーへのポインタを返し、一致しない場合は次のユーザーを確認し、リストの終端まで一致しない場合はNULLを返す。
// [Exhaustive] 一致するユーザーが見つかればそのポインタを返し、見つからなければNULLを返す。
// [Troubleshooting] リストがNULLの場合やIDが見つからない場合はNULLを返し、エラーとして扱うことができる。
// [Contextualizing] この関数は、ユーザー管理システムやデータベース操作で特定のユーザーを検索する際に使用される。
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
