void AddNewReservationScreen(ReservationPtr *ReservationList, UserPtr UserList, RoomPtr *RoomList)
{
  ScreenHeader("\tAdd New Reservation");

  ReservationPtr NewReservation = (Reservation *)malloc(sizeof(Reservation));
  ReadReservationInfo(*ReservationList, &NewReservation, UserList, RoomList);

  *ReservationList = AddNewReservation(*ReservationList, NewReservation);
  printf("\nReservation Added Successfully!\n");

  PauseProgram();
}
