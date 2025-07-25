// [Logical] この関数は、新しい部屋を既存の部屋リストの末尾に追加します。
// [Precise] 関数は、まず部屋リストが空かどうかを確認し、空であれば新しい部屋を返し、そうでなければリストの末尾まで移動して新しい部屋を追加します。
// [Unambiguous] 部屋リストがNULLの場合、新しい部屋をそのまま返し、そうでない場合はリストの最後に新しい部屋を接続してリスト全体を返します。
// [Exhaustive] 部屋リストがNULLの場合は新しい部屋を返し、そうでない場合はリストの末尾に新しい部屋を追加してからリスト全体を返します。
// [Troubleshooting] 関数内でエラー処理は行われておらず、NULLポインタが渡された場合の動作は未定義です。
// [Contextualizing] この関数は、部屋を管理するシステムにおいて、新しい部屋をリストに追加する際に使用されます。
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
