// [Logical] この関数は、予約リストを日付順にソートするための画面を表示します。
void SortReservationsByDateScreen(ReservationPtr *ReservationList)
{
  ScreenHeader("  Sort Reservations By Date");

  if (*ReservationList == NULL)
  {
    printf("\nList is empty!");
  }
  else
  {
    *ReservationList = SortReservationsByDate(*ReservationList);
    printf("\nReservations Sorted Successfully!\n");
  }
  PauseProgram();
}
