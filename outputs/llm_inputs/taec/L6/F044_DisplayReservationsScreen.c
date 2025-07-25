// [Logical] この関数は、予約リストを表示するための画面を出力します。
// [Precise] 予約リストが空かどうかをチェックし、空でない場合は各予約を順に表示します。
// [Unambiguous] 予約リストがNULLの場合は'List is empty!'と表示し、そうでない場合は各予約を順に表示します。
// [Exhaustive] 予約リストがNULLの場合は'List is empty!'を表示し、リストが存在する場合は各予約を番号付きで表示します。
// [Troubleshooting] 予約リストがNULLの場合は適切に'List is empty!'と表示されるため、エラーは発生しません。
// [Contextualizing] この関数は、予約情報をユーザーに表示する必要がある場面で使用されます。
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
