// [Logical] この関数は、予約リストから指定されたIDの予約を検索して返します。
ReservationPtr SearchReservationByID(ReservationPtr ReservationList, int ID)
{
  ReservationPtr current = ReservationList;
  while (current != NULL)
  {
    if (current->ID == ID)
      return current;

    current = current->Next;
  }
  return NULL;
}
