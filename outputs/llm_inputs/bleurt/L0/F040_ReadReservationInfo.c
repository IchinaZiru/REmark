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
