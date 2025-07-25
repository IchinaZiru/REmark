// [Logical] この関数は、部屋のリストを画面に表示する機能を持つ。
// [Precise] 部屋リストが空であるかを確認し、空であればメッセージを表示し、そうでなければ各部屋情報を順に表示する。
// [Unambiguous] 部屋リストがNULLの場合は空リストのメッセージを表示し、そうでない場合はリスト内の各部屋を順に表示する。
// [Exhaustive] 部屋リストがNULLの場合は"List is empty!"を表示し、リストが存在する場合は各部屋を順に表示する。エラー処理は特に行っていない。
// [Troubleshooting] 部屋リストがNULLの場合に"List is empty!"と表示するが、その他のエラー処理は実装されていない。
// [Contextualizing] この関数は、ユーザーインターフェースの一部として、部屋情報を表示する場面で使用される。
void DisplayRoomListScreen(RoomPtr RoomList)
{
  ScreenHeader("\t Room List");

  if (RoomList == NULL)
  {
    printf("\nList is empty!");
  }
  else
  {
    RoomPtr current = RoomList;
    int i = 0;
    while (current != NULL)
    {
      printf("\n - Room [%d]:", ++i);
      DisplayRoomCard(current);
      current = current->Next;
    }
  }
  PauseProgram();
}
