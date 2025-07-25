// [Logical] この関数は、指定された名前に一致する部屋をリストから検索し、見つかった場合にその情報を表示します。
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
