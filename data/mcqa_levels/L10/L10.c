void CheckRoomAvailabilityScreen(RoomPtr RoomList)
{
  ScreenHeader("  Check Room Availability");

  int Choice = 0;
  printf("\nChoose how you want to check:\n[1] Name.\t[2] ID.\n\n => ");
  Choice = ReadIntBetween(1, 2);
  switch (Choice)
  {
  case 1:
    SearchRoomByNameScreen(RoomList);
    break;
  case 2:
    SearchRoomByIDScreen(RoomList);

  default:
    printf("\nInvalid Choice!");
    break;
  }
}

// @brief Remove one reservation node by exact ID from a singly-linked list.
// @kind DS-MUTATOR  // no UI
// @inputs ReservationList (nullable head), ID (assumed to exist)
// @returns New head pointer (may differ if head was removed)
// @side-effects Unlinks the matching node and frees it. No printf/scanf.
// @undefined If ID is absent, 'current' can be NULL before dereference (bug).
// @contrast Not: DeleteReservationScreen (no user prompt/confirmation)
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

// @brief Remove one room node by exact ID from a singly-linked list.
// @kind DS-MUTATOR
// @inputs RoomList (nullable head), ID (assumed to exist)
// @returns New head pointer
// @side-effects Unlinks and frees the matching node; no I/O.
// @undefined Absent ID leads to undefined behavior in current impl.
// @contrast Not: DeleteRoomScreen / Not: any UI wrapper
RoomPtr DeleteRoomByID(RoomPtr RoomList, int ID)
{
  RoomPtr current = RoomList;
  RoomPtr previous = NULL;

  while (current != NULL && current->ID != ID)
  {
    previous = current;
    current = current->Next;
  }

  if (previous == NULL)
    RoomList = current->Next;
  else
    previous->Next = current->Next;

  free(current);
  return RoomList;
}

// @brief Print a single reservation as a formatted "card" to stdout.
// @kind PURE-DISPLAY
// @inputs Reservation (non-NULL); prints ID/UserID/RoomID/Date/Time
// @side-effects stdout output only; no mutation, no scan.
// @contrast Not: DisplayReservationsScreen (not a list), Not: any Screen(UI)
void DisplayReservationCard(ReservationPtr Reservation)
{
  printf("\n______________________________\n");
  printf(" - ID: %d", Reservation->ID);
  printf("\n - User ID: %d", Reservation->UserID);
  printf("\n - Room ID: %d", Reservation->RoomID);
  printf("\n - Date: %02d/%02d/%d",
         Reservation->Date.Day, Reservation->Date.Month, Reservation->Date.Year);
  printf("\n - Time: %02d:%02d - %02d:%02d",
         Reservation->Start_Time.Hour, Reservation->Start_Time.Minute,
         Reservation->End_Time.Hour, Reservation->End_Time.Minute);
  printf("\n______________________________\n");
}

// @brief Print a single user as a formatted "card" to stdout.
// @kind PURE-DISPLAY
// @inputs User (non-NULL); prints ID/FirstName/LastName
// @side-effects stdout only; no mutation.
// @contrast Not: DisplayUserList / Not: Search/Screen wrappers
void DisplayUserCard(UserPtr User)
{
  printf("\n______________________________\n");
  printf(" - ID: %d", User->ID);
  printf("\n - First Name: %s", User->FirstName);
  printf("\n - Last Name : %s", User->LastName);
  printf("\n______________________________\n");
}

// @brief Read an integer from stdin until it falls within [from, to] (inclusive).
// @kind INPUT (blocking)
// @inputs from, to (from <= to). Assumes numeric input; non-numeric is undefined.
// @returns int within [from, to]
// @side-effects reads stdin; prints prompt only when out-of-range.
// @contrast Not: menu logic itself; validation loop only.
int ReadIntBetween(int from, int to)
{
  int choice = 0;
  scanf("%d", &choice);
  while (choice < from || choice > to)
  {
    printf("\nPlease enter a number in the range [%d-%d]: ", from, to);
    scanf("%d", &choice);
  }
  return choice;
}

// @brief UI loop for reservation operations; maps 1..5 to handlers.
// @kind UI-SCREEN
// @calls 1:AddNewReservationScreen, 2:DeleteReservationScreen,
//        3:DisplayReservationsScreen, 4:SortReservationsByDateScreen
// @loop while(choice != 5); uses ReadIntBetween(1,5)
// @side-effects prints to stdout and reads from stdin; no DS mutation itself.
// @contrast Not: DeleteReservationByID (that one mutates the list)
void ReservationManagementMenu(ReservationPtr *ReservationList, UserPtr UserList, RoomPtr *RoomList)
{
  int Choice = 0;
  do
  {
    system("cls");
    printf("\n---------------------------------------------\n");
    printf("\t Reservation Management Menu");
    printf("\n---------------------------------------------\n");
    printf("  [1] Add New Reservation.");
    printf("\n  [2] Delete Reservation.");
    printf("\n  [3] Display Reservations.");
    printf("\n  [4] Sort by Date.");
    printf("\n  [5] Main Menu.");
    printf("\n---------------------------------------------\n");
    printf(" - Please enter your choice: ");
    Choice = ReadIntBetween(1, 5);
    switch (Choice)
    {
    case 1:
      AddNewReservationScreen(ReservationList, UserList, RoomList);
      break;
    case 2:
      DeleteReservationScreen(ReservationList, RoomList);
      break;
    case 3:
      DisplayReservationsScreen(*ReservationList);
      break;
    case 4:
      SortReservationsByDateScreen(ReservationList);
      break;
    case 5:
      break;
    default:
      printf("\nError!");
      break;
    }
  } while (Choice != 5);
}

// @brief UI wrapper: read first name → SearchUserByFirstName → show result/“not found”.
// @kind UI-SCREEN
// @calls SearchUserByFirstName(UserList, name, &IsExist), PauseProgram
// @side-effects I/O only; does not mutate user list.
// @contrast Not: SearchUserByFirstName (pure search), Not: DisplayUserCard
void SearchUserByFirstNameScreen(UserPtr UserList)
{
  ScreenHeader(" Search By First Name");

  bool IsExist = false;
  char FirstName[MAX_STRING_SIZE];
  printf("\nPlease enter first name: ");
  scanf("%s", FirstName);

  SearchUserByFirstName(UserList, FirstName, &IsExist);

  if (IsExist == false)
    printf("\nUser not Found!\n");

  PauseProgram();
}

// @brief Program entry point; delegates to MainMenu then exits.
// @kind BOOTSTRAP
// @calls MainMenu()
// @returns 0; no domain logic here.
// @contrast Not: any menu handler or DS operation
int main()
{
  MainMenu();
  return 0;
}