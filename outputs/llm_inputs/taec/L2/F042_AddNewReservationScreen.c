// [Logical] この関数は、新しい予約を追加するための画面を表示し、予約情報を取得してリストに追加します。
// [Precise] 関数は、まず画面ヘッダーを表示し、次に新しい予約のメモリを確保し、ユーザーと部屋のリストを基に予約情報を読み込み、予約リストに新しい予約を追加します。
void AddNewReservationScreen(ReservationPtr *ReservationList, UserPtr UserList, RoomPtr *RoomList)
{
  ScreenHeader("\tAdd New Reservation");

  ReservationPtr NewReservation = (Reservation *)malloc(sizeof(Reservation));
  ReadReservationInfo(*ReservationList, &NewReservation, UserList, RoomList);

  *ReservationList = AddNewReservation(*ReservationList, NewReservation);
  printf("\nReservation Added Successfully!\n");

  PauseProgram();
}
