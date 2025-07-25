// [Logical] この関数は、新しい予約を既存の予約リストの末尾に追加します。
// [Precise] 予約リストが空の場合は新しい予約をそのまま返し、そうでない場合はリストの末尾を探索して新しい予約を追加します。
// [Unambiguous] 予約リストがNULLの場合、新しい予約を返し、そうでない場合はリストの最後に新しい予約を接続します。
// [Exhaustive] 予約リストがNULLなら新しい予約を返し、そうでない場合は更新された予約リストを返します。
// [Troubleshooting] エラー処理は特に行われておらず、入力がNULLである場合の動作のみを考慮しています。
ReservationPtr AddNewReservation(ReservationPtr ReservationList, ReservationPtr NewReservation)
{
  if (ReservationList == NULL)
  {
    return NewReservation;
  }

  ReservationPtr current = ReservationList;
  while (current->Next != NULL)
  {
    current = current->Next;
  }
  current->Next = NewReservation;
  return ReservationList;
}
