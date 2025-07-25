// [Logical] この関数は、新しい部屋を既存の部屋リストの末尾に追加します。
RoomPtr AddNewRoom(RoomPtr RoomList, RoomPtr NewRoom)
{
  if (RoomList == NULL)
  {
    return NewRoom;
  }

  RoomPtr current = RoomList;
  while (current->Next != NULL)
  {
    current = current->Next;
  }
  current->Next = NewRoom;
  return RoomList;
}
