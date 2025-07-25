// [Logical] この関数は、ユーザーリストから指定された名前を持つユーザーを検索し、見つかった場合にその情報を表示します。
// [Precise] ユーザーリストを線形探索し、指定された名前と一致するユーザーを見つけた場合にその情報を表示し、フラグをtrueに設定します。
// [Unambiguous] ユーザーリストを先頭から末尾まで順に確認し、一致する名前が見つかるとそのユーザー情報を表示し、フラグを更新します。
// [Exhaustive] 一致するユーザーが見つかるとその情報を表示し、フラグをtrueに設定しますが、見つからない場合は何も表示されず、フラグは変更されません。
// [Troubleshooting] 関数内でエラー処理は行われておらず、NULLポインタや不正な入力に対する対処は呼び出し元で行う必要があります。
// [Contextualizing] この関数は、ユーザー情報を管理するシステムで、特定の名前を持つユーザーを検索する際に使用されます。
// [Condensing] 指定された名前を持つユーザーをリストから検索し、見つかった場合にその情報を表示します。
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
