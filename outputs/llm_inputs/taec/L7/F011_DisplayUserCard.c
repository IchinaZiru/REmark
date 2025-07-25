// [Logical] この関数は、ユーザーの情報をフォーマットしてコンソールに表示します。
// [Precise] ユーザー構造体のポインタを受け取り、そのID、名前、姓をprintf関数を用いて出力します。
// [Unambiguous] ユーザー情報は、ID、名前、姓の順に一貫したフォーマットで表示されます。
// [Exhaustive] この関数は戻り値を持たず、ユーザー情報を標準出力に表示するだけです。
// [Troubleshooting] ユーザーポインタがNULLの場合、未定義の動作が発生する可能性があります。
// [Contextualizing] この関数は、ユーザー情報を確認したいときやログに記録したいときに使用されます。
// [Condensing] ユーザーのID、名前、姓をフォーマットしてコンソールに表示する関数です。
void DisplayUserCard(UserPtr User)
{
  printf("\n______________________________\n");
  printf(" - ID: %d", User->ID);
  printf("\n - First Name: %s", User->FirstName);
  printf("\n - Last Name : %s", User->LastName);
  printf("\n______________________________\n");
}
