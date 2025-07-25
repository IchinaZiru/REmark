// [Logical] この関数は、2つの予約データの内容を入れ替える。
// [Precise] 一時変数を用いて、2つの予約構造体の各フィールドの値を交換している。
// [Unambiguous] Reservation1とReservation2の各フィールド（ID、UserID、RoomID、Date、Start_Time、End_Time）を正確に入れ替える。
// [Exhaustive] 関数はvoid型で戻り値はなく、直接渡された予約データの内容を変更する。
// [Troubleshooting] エラー処理は含まれておらず、無効なポインタが渡された場合の動作は未定義。
// [Contextualizing] 予約データを管理するシステム内で、予約情報の入れ替えが必要な場面で使用される。
// [Condensing] 2つの予約データの内容を一時変数を使って入れ替える関数。
void SwapReservationData(ReservationPtr Reservation1, ReservationPtr Reservation2)
{
  int tmpID = Reservation1->ID;
  int tmpUserID = Reservation1->UserID;
  int tmpRoomID = Reservation1->RoomID;

  stDate tmpDate = Reservation1->Date;
  stTime tmpStart_Time = Reservation1->Start_Time;
  stTime tmpEnd_Time = Reservation1->End_Time;

  Reservation1->ID = Reservation2->ID;
  Reservation1->UserID = Reservation2->UserID;
  Reservation1->RoomID = Reservation2->RoomID;

  Reservation1->Date = Reservation2->Date;
  Reservation1->Start_Time = Reservation2->Start_Time;
  Reservation1->End_Time = Reservation2->End_Time;

  Reservation2->ID = tmpID;
  Reservation2->UserID = tmpUserID;
  Reservation2->RoomID = tmpRoomID;

  Reservation2->Date = tmpDate;
  Reservation2->Start_Time = tmpStart_Time;
  Reservation2->End_Time = tmpEnd_Time;
}
