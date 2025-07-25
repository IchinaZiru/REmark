// [Logical] この関数は、部屋情報を入力し、構造体に格納するためのものです。
// [Precise] ユーザーから部屋のIDと名前を入力し、構造体の対応するフィールドに格納します。また、予約状態を初期化し、次の部屋へのポインタをNULLに設定します。
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
