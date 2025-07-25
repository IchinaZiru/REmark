// [Logical] この関数は、ユーザーが入力したIDに基づいて部屋を検索し、その結果を表示します。
// [Precise] ユーザーからIDを入力させ、そのIDを用いて部屋リストから部屋を検索し、見つかった場合は部屋情報を表示します。
void SearchRoomByIDScreen(RoomPtr RoomList)
{
  ScreenHeader("\tSearch By ID");
  int ID;
  printf("\nPlease enter ID: ");
  ID = ReadIntBetween(0, 9999);

  RoomPtr Room = SearchRoomByID(RoomList, ID);

  if (Room == NULL)
    printf("\nRoom not Found!\n");
  else
    DisplayRoomCard(Room);

  PauseProgram();
}
