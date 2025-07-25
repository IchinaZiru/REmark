// [Logical] この関数は、リンクリストから指定されたIDの部屋を削除します。
// [Precise] リストを先頭から順に探索し、指定されたIDの部屋を見つけたら、そのノードをリストから削除し、メモリを解放します。
// [Unambiguous] 関数は、指定されたIDを持つノードを見つけて削除し、リストの先頭が削除された場合も考慮しています。
// [Exhaustive] 削除されたノードのメモリを解放し、更新されたリストの先頭を返しますが、IDが見つからない場合の処理はありません。
// [Troubleshooting] IDが見つからない場合、関数は何も削除せず、リストの先頭をそのまま返します。
// [Contextualizing] この関数は、部屋の情報を管理するアプリケーションで、特定の部屋をリストから削除する際に使用されます。
// [Condensing] 指定されたIDの部屋をリンクリストから削除し、メモリを解放して更新されたリストを返します。
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
