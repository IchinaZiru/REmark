// [Logical] この関数は、予約リストを日付順にソートするための画面を表示します。
// [Precise] 予約リストが空でない場合、`SortReservationsByDate`関数を呼び出してリストをソートし、結果を表示します。
// [Unambiguous] 予約リストがNULLでない場合にのみソートを実行し、成功メッセージを表示します。
// [Exhaustive] リストが空の場合はメッセージを表示し、リストが空でない場合はソートを実行して成功メッセージを表示します。
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
