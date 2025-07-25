// [Logical] この関数はユーザー情報を入力し、指定された構造体に格納します。
// [Precise] ユーザーにID、名前、苗字を入力させ、それらをUserPtr構造体のメンバーに格納します。
// [Unambiguous] ユーザーからIDと名前を取得し、構造体の対応するフィールドに保存します。
// [Exhaustive] IDは0から9999の範囲で入力され、NextポインタはNULLに設定されます。
// [Troubleshooting] IDの入力が範囲外の場合、再入力を促す処理が必要です。名前の入力でバッファオーバーフローが発生しないように注意が必要です。
// [Contextualizing] この関数はユーザー情報を管理するシステムで、ユーザー情報を初期化または更新する際に使用されます。
// [Condensing] ユーザーからIDと名前を入力し、構造体に保存する関数です。
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
