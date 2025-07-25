// [Logical] この関数は、予約リストから指定されたIDの予約を検索して返します。
// [Precise] リストの先頭から順に各予約のIDを比較し、一致する予約が見つかればそのポインタを返します。
// [Unambiguous] リストを線形探索し、IDが一致する最初の予約を見つけた時点でそのポインタを返し、見つからない場合はNULLを返します。
// [Exhaustive] 一致する予約が見つかればその予約のポインタを返し、見つからなければNULLを返します。
// [Troubleshooting] 予約リストがNULLの場合やIDが見つからない場合はNULLを返しますが、エラー処理は特に行っていません。
// [Contextualizing] この関数は、予約管理システムなどで特定の予約を検索する際に使用されます。
// [Condensing] 予約リストから指定IDの予約を検索し、見つかればそのポインタを返します。
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
