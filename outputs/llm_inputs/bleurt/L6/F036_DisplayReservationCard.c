// [Logical] この関数は予約情報をフォーマットして表示します。
// [Precise] 予約情報の各フィールドを取得し、フォーマットされた文字列として標準出力に表示します。
// [Unambiguous] Reservation構造体のメンバーを使用して、予約の詳細を明確に表示します。
// [Exhaustive] この関数は出力を行うだけで、戻り値はなく、他のシステムへの影響もありません。
// [Troubleshooting] ReservationがNULLの場合、プログラムがクラッシュする可能性があるため、NULLチェックを追加することが推奨されます。
// [Contextualizing] この関数は、予約管理システムで予約の詳細をユーザーに表示する際に使用されます。
void DisplayReservationCard(ReservationPtr Reservation)
{
  printf("\n______________________________\n");
  printf(" - ID: %d", Reservation->ID);
  printf("\n - User ID: %d", Reservation->UserID);
  printf("\n - Room ID: %d", Reservation->RoomID);
  printf("\n - Date: %02d/%02d/%d",
         Reservation->Date.Day, Reservation->Date.Month, Reservation->Date.Year);
  printf("\n - Time: %02d:%02d - %02d:%02d",
         Reservation->Start_Time.Hour, Reservation->Start_Time.Minute,
         Reservation->End_Time.Hour, Reservation->End_Time.Minute);
  printf("\n______________________________\n");
}
