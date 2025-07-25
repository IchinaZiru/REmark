// [Logical] この関数は、新しい予約を既存の予約リストの末尾に追加します。
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
