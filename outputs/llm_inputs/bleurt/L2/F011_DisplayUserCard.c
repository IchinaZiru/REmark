// [Logical] この関数は、ユーザーの情報をフォーマットしてコンソールに表示します。
// [Precise] ユーザー構造体のポインタを受け取り、そのID、名前、姓をprintf関数を用いて出力します。
void DisplayUserCard(UserPtr User)
{
  printf("\n______________________________\n");
  printf(" - ID: %d", User->ID);
  printf("\n - First Name: %s", User->FirstName);
  printf("\n - Last Name : %s", User->LastName);
  printf("\n______________________________\n");
}
