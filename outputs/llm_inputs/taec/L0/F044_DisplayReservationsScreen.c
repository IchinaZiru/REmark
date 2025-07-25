void DisplayReservationsScreen(ReservationPtr ReservationList)
{
  ScreenHeader("\t Reservations List");

  if (ReservationList == NULL)
  {
    printf("\nList is empty!");
  }
  else
  {
    ReservationPtr current = ReservationList;
    int i = 0;
    while (current != NULL)
    {
      printf("\n - Reservation [%d]:", ++i);
      DisplayReservationCard(current);
      current = current->Next;
    }
  }
  PauseProgram();
}
