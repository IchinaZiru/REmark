// [Logical] この関数は予約情報と部屋情報に予約期間を読み込み、設定します。
// [Precise] ユーザーから予約の日付と開始・終了時間を入力させ、それを予約情報と部屋情報に格納します。
// [Unambiguous] ユーザー入力を通じて予約の日付と時間を取得し、予約情報と部屋情報の両方に同じ値を設定します。
// [Exhaustive] 関数は予約情報と部屋情報のDate、Start_Time、End_Timeを設定し、部屋の予約状態をtrueにします。
// [Troubleshooting] ユーザー入力が不適切な場合、ReadDateやReadTime関数でエラー処理が行われる必要がありますが、この関数内では特にエラー処理は行われていません。
void ReadReservationPeriod(ReservationPtr *ReservationInfo, RoomPtr *Room)
{
  printf("\nEnter Reservation Date & Time:");
  (*ReservationInfo)->Date = ReadDate(" => Date [Day Month Year]: ");
  (*ReservationInfo)->Start_Time = ReadTime(" => Start Time [Hour Minute]: ");
  (*ReservationInfo)->End_Time = ReadTime(" => End Time [Hour Minute]: ");

  (*Room)->Date = (*ReservationInfo)->Date;
  (*Room)->Start_Time = (*ReservationInfo)->Start_Time;
  (*Room)->End_Time = (*ReservationInfo)->End_Time;
  (*Room)->IsReserved = true;
}
