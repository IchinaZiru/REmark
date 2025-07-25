// [Logical] この関数は、予約リストから指定されたIDの予約を削除します。
// [Precise] リストを順に走査し、指定されたIDと一致する予約を見つけたら、そのノードをリストから削除します。
ReservationPtr DeleteReservationByID(ReservationPtr ReservationList, int ID)
{
  ReservationPtr current = ReservationList;
  ReservationPtr previous = NULL;

  while (current != NULL && current->ID != ID)
  {
    previous = current;
    current = current->Next;
  }

  if (previous == NULL)
    ReservationList = current->Next;
  else
    previous->Next = current->Next;

  free(current);
  return ReservationList;
}
