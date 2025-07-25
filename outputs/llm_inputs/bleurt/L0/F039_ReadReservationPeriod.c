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
