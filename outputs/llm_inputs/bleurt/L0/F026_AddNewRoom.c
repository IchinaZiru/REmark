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
