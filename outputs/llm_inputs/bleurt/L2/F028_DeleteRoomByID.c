// [Logical] この関数は、リンクリストから指定されたIDの部屋を削除します。
// [Precise] リストを先頭から順に探索し、指定されたIDの部屋を見つけたら、そのノードをリストから削除し、メモリを解放します。
RoomPtr DeleteRoomByID(RoomPtr RoomList, int ID)
{
  RoomPtr current = RoomList;
  RoomPtr previous = NULL;

  while (current != NULL && current->ID != ID)
  {
    previous = current;
    current = current->Next;
  }

  if (previous == NULL)
    RoomList = current->Next;
  else
    previous->Next = current->Next;

  free(current);
  return RoomList;
}
