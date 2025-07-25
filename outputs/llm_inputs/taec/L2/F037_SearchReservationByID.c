// [Logical] この関数は、予約リストから指定されたIDの予約を検索して返します。
// [Precise] リストの先頭から順に各予約のIDを比較し、一致する予約が見つかればそのポインタを返します。
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
