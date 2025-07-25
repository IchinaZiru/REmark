// [Logical] この関数は、2つの予約データの内容を入れ替える。
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
