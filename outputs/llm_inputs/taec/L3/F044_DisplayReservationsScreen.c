// [Logical] この関数は、予約リストを表示するための画面を出力します。
// [Precise] 予約リストが空かどうかをチェックし、空でない場合は各予約を順に表示します。
// [Unambiguous] 予約リストがNULLの場合は'List is empty!'と表示し、そうでない場合は各予約を順に表示します。
void DisplayReservationsScreen(ReservationPtr ReservationList)
{
  ScreenHeader("\t Reservations List");

  if (ReservationList == NULL)
  {
    printf("\nList is empty!");
  }
  else
  {
    ReservationPtr current = ReservationList;
    int i = 0;
    while (current != NULL)
    {
      printf("\n - Reservation [%d]:", ++i);
      DisplayReservationCard(current);
      current = current->Next;
    }
  }
  PauseProgram();
}
