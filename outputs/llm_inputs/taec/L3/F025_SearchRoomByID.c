// [Logical] この関数は、リンクリストから指定されたIDを持つ部屋を検索して返します。
// [Precise] リストの各要素を順にチェックし、IDが一致する要素を見つけたらそのポインタを返します。
// [Unambiguous] IDが一致する部屋が見つかった場合はそのポインタを、見つからなかった場合はNULLを返します。
RoomPtr SearchRoomByID(RoomPtr RoomList, int ID)
{
  RoomPtr current = RoomList;
  while (current != NULL)
  {
    if (current->ID == ID)
      return current;

    current = current->Next;
  }
  return NULL;
}
