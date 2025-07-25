// [Logical] この関数は、部屋情報を入力し、構造体に格納するためのものです。
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
