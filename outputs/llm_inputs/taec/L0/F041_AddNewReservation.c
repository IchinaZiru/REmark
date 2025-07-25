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
