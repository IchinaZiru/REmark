// [Logical] この関数は、指定された名前に一致する部屋をリストから検索し、見つかった場合にその情報を表示します。
// [Precise] 部屋のリストを順に走査し、名前が一致する部屋を見つけると、その部屋の情報を表示し、存在フラグを真に設定します。
// [Unambiguous] 部屋の名前が一致した場合にのみ情報を表示し、フラグを更新します。
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
