// [Logical] この関数は予約情報と部屋情報に予約期間を読み込み、設定します。
// [Precise] ユーザーから予約の日付と開始・終了時間を入力させ、それを予約情報と部屋情報に格納します。
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
