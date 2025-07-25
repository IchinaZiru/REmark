// [Logical] この関数は、予約リストを日付順にソートします。
// [Precise] リスト内の各予約を二重ループで比較し、必要に応じてデータを入れ替えることでソートを行います。
// [Unambiguous] 各予約のデータを比較し、日付が前のものを先にするようにデータを交換します。
// [Exhaustive] 関数はソートされた予約リストを返し、リスト内のデータを直接変更します。
ReservationPtr SortReservationsByDate(ReservationPtr ReservationList)
{
  ReservationPtr current1 = ReservationList;
  ReservationPtr current2 = NULL;

  while (current1->Next != NULL)
  {
    current2 = current1->Next;
    while (current2 != NULL)
    {
      if (IsDate1BeforeDate2(current2->Date, current1->Date))
      {
        SwapReservationData(current1, current2);
      }
      current2 = current2->Next;
    }
    current1 = current1->Next;
  }
  return ReservationList;
}
