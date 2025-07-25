void SearchRoomByName(RoomPtr RoomList, char Name[MAX_STRING_SIZE], bool *IsExist)
{
  RoomPtr current = RoomList;
  while (current != NULL)
  {
    if (strcmp(current->Name, Name) == 0)
    {
      DisplayRoomCard(current);
      *IsExist = true;
    }
    current = current->Next;
  }
}
