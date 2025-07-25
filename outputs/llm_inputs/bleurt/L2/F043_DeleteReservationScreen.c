// [Logical] この関数は、予約リストから指定されたIDの予約を削除するためのものです。
// [Precise] ユーザーに予約IDを入力させ、そのIDに基づいて予約を検索し、確認後に削除します。
void DeleteReservationScreen(ReservationPtr *ReservationList, RoomPtr *RoomList)
{
  ScreenHeader("\tDelete Reservation");

  int ID;
  ReservationPtr Reservation;
  printf("\nPlease enter ID: ");
  ID = ReadIntBetween(0, 9999);

  if ((Reservation = SearchReservationByID(*ReservationList, ID)) != NULL)
  {
    DisplayReservationCard(Reservation);
    char Choice = 'n';
    printf("\nAre you sure you want to delete this reservation? y/n? ");
    scanf(" %c", &Choice);
    if (Choice == 'y')
    {
      // Delete Room Reseration
      RoomPtr Room = SearchRoomByID(*RoomList, Reservation->RoomID);
      Room->IsReserved = false;
      Room->Date = (stDate){0, 0, 0};
      Room->Start_Time = (stTime){0, 0};
      Room->End_Time = (stTime){0, 0};

      *ReservationList = DeleteReservationByID(*ReservationList, ID);
      printf("\nReservation with ID [%d] deleted successfully!\n", ID);
    }
  }
  else
    printf("\nReservation with ID [%d] not found!\n", ID);

  PauseProgram();
}
