// [Logical] この関数は、予約リストから指定されたIDの予約を削除します。
// [Precise] リストを順に走査し、指定されたIDと一致する予約を見つけたら、そのノードをリストから削除します。
// [Unambiguous] 指定されたIDの予約を見つけた場合、そのノードをリストから削除し、メモリを解放します。
// [Exhaustive] 指定されたIDの予約が見つかると削除され、リストの先頭が変更される可能性があります。見つからない場合はリストは変更されません。
// [Troubleshooting] 削除する予約が見つからない場合、リストはそのまま返され、特にエラー処理は行われません。
// [Contextualizing] この関数は、予約管理システムなどで、特定の予約を削除する際に使用されます。
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
