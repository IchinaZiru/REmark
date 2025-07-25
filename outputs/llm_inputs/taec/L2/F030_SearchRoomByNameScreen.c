// [Logical] この関数は、ユーザーが入力した名前に基づいて部屋を検索し、結果を表示します。
// [Precise] ユーザーに部屋の名前を入力させ、その名前を用いてリスト内の部屋を検索し、見つからなければメッセージを表示します。
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
