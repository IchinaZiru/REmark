// [Logical] この関数は、ユーザーが入力した名前に基づいて部屋を検索し、結果を表示します。
// [Precise] ユーザーに部屋の名前を入力させ、その名前を用いてリスト内の部屋を検索し、見つからなければメッセージを表示します。
// [Unambiguous] ユーザーが入力した名前で部屋を検索し、見つからない場合は「Room not Found!」と表示します。
// [Exhaustive] 部屋が見つかった場合は何も表示せず、見つからない場合はエラーメッセージを表示し、プログラムを一時停止します。
// [Troubleshooting] 部屋が見つからない場合は、エラーメッセージを表示し、ユーザーに再試行の機会を与えるためにプログラムを一時停止します。
// [Contextualizing] この関数は、部屋の管理システムにおいて、ユーザーが特定の部屋を名前で検索する際に使用されます。
void SearchRoomByNameScreen(RoomPtr RoomList)
{
  ScreenHeader("\tSearch by Name");

  bool IsExist = false;

  char Name[MAX_STRING_SIZE];
  printf("\nPlease enter name: ");
  scanf("%s", Name);

  SearchRoomByName(RoomList, Name, &IsExist);

  if (IsExist == false)
    printf("\nRoom not Found!\n");

  PauseProgram();
}
