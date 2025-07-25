// [Logical] この関数は予約情報を入力し、予約リストに追加するためのデータを取得します。
// [Precise] ユーザーから予約ID、ユーザーID、部屋IDを入力させ、既存のデータと照合して有効な情報を取得します。
// [Unambiguous] 予約IDは既存の予約と重複しないこと、ユーザーIDは存在すること、部屋IDは予約可能であることを確認します。
// [Exhaustive] 予約ID、ユーザーID、部屋IDの入力が完了し、部屋が予約可能であれば、予約期間を設定し、予約情報のリストに追加します。
// [Troubleshooting] 入力されたIDが無効または重複している場合、再入力を促すメッセージを表示します。
// [Contextualizing] この関数は予約管理システムの一部として、新しい予約情報を追加する際に使用されます。
void ReadReservationInfo(ReservationPtr ReservationList, ReservationPtr *ReservationInfo, UserPtr UserList, RoomPtr *RoomList)
{
  printf("\nEnter Reservation Info:\n--------------------------");

  printf("\n - ID: ");
  (*ReservationInfo)->ID = ReadIntBetween(0, 9999);
  while (SearchReservationByID(ReservationList, (*ReservationInfo)->ID) != NULL)
  {
    printf("\nReservation with ID [%d] already exist!", (*ReservationInfo)->ID);
    printf("\n - Please enter another ID: ");
    (*ReservationInfo)->ID = ReadIntBetween(0, 9999);
  }

  printf("\n - User ID: ");
  (*ReservationInfo)->UserID = ReadIntBetween(0, 9999);
  while (SearchUserByID(UserList, (*ReservationInfo)->UserID) == NULL)
  {
    printf("\nUser with ID [%d] not found!", (*ReservationInfo)->UserID);
    printf("\n - Please enter a valid User ID: ");
    (*ReservationInfo)->UserID = ReadIntBetween(0, 9999);
  }

  RoomPtr Room;
  printf("\n - Room ID: ");
  (*ReservationInfo)->RoomID = ReadIntBetween(0, 9999);
  while ((Room = SearchRoomByID(*RoomList, (*ReservationInfo)->RoomID)) == NULL || Room->IsReserved == true)
  {
    printf("\nRoom with ID [%d] not available!", (*ReservationInfo)->RoomID);
    printf("\n - Please enter another Room ID: ");
    (*ReservationInfo)->RoomID = ReadIntBetween(0, 9999);
  }

  ReadReservationPeriod(ReservationInfo, &Room);

  (*ReservationInfo)->Next = NULL;
}
