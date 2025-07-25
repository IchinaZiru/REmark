// [Logical] この関数はユーザー情報を入力し、指定された構造体に格納します。
// [Precise] ユーザーにID、名前、苗字を入力させ、それらをUserPtr構造体のメンバーに格納します。
// [Unambiguous] ユーザーからIDと名前を取得し、構造体の対応するフィールドに保存します。
void ReadUserInfo(UserPtr *UserInfo)
{
  printf("\nEnter User Info:\n--------------------------");
  printf("\nID: ");
  (*UserInfo)->ID = ReadIntBetween(0, 9999);
  printf("\nFirst Name: ");
  scanf("%s", (*UserInfo)->FirstName);
  printf("\nLast Name : ");
  scanf("%s", (*UserInfo)->LastName);
  (*UserInfo)->Next = NULL;
}
