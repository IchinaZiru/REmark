// [Logical] この関数は、部屋情報を入力し、構造体に格納するためのものです。
// [Precise] ユーザーから部屋のIDと名前を入力し、構造体の対応するフィールドに格納します。また、予約状態を初期化し、次の部屋へのポインタをNULLに設定します。
// [Unambiguous] 関数は、部屋のIDを0から9999の範囲で取得し、名前を標準入力から読み取ります。予約状態をfalseに設定し、次のポインタをNULLにします。
// [Exhaustive] 関数は戻り値を持たず、入力されたデータを直接構造体に格納します。エラー処理は特に行っていません。
// [Troubleshooting] IDの入力が範囲外の場合や名前入力でエラーが発生した場合の処理は実装されていません。
void ReadRoomInfo(RoomPtr *RoomInfo)
{
  printf("\nEnter Room Info:\n--------------------------");
  printf("\nID: ");
  (*RoomInfo)->ID = ReadIntBetween(0, 9999);
  printf("\nName: ");
  scanf("%s", (*RoomInfo)->Name);

  (*RoomInfo)->IsReserved = false;
  (*RoomInfo)->Next = NULL;
}
