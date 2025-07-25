// [Logical] この関数は、リンクリストから指定されたIDを持つ部屋を検索して返します。
// [Precise] リストの各要素を順にチェックし、IDが一致する要素を見つけたらそのポインタを返します。
// [Unambiguous] IDが一致する部屋が見つかった場合はそのポインタを、見つからなかった場合はNULLを返します。
// [Exhaustive] 一致するIDが見つかればその部屋のポインタを返し、見つからなければNULLを返します。
// [Troubleshooting] IDが見つからない場合はNULLが返されるため、呼び出し側でNULLチェックを行う必要があります。
// [Contextualizing] この関数は、部屋の情報を管理するシステムで特定の部屋を検索する際に使用されます。
// [Condensing] 指定されたIDを持つ部屋をリンクリストから検索し、見つかればそのポインタを返します。
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
