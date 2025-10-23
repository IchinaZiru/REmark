// [Logical] この関数は、部屋の空き状況を名前またはIDで確認するための画面を表示します。
// [Precise] ユーザーに名前またはIDでの検索方法を選択させ、その選択に基づいて対応する検索関数を呼び出します。
// [Unambiguous] ユーザーの選択に応じて、名前での検索またはIDでの検索を行うことができるように分岐処理を行います。
// [Exhaustive] 選択肢が1または2の場合にそれぞれの検索関数を呼び出し、それ以外の選択肢ではエラーメッセージを表示します。
// [Troubleshooting] 無効な選択肢が入力された場合にエラーメッセージを表示し、再入力を促します。
// [Contextualizing] この関数は、ホテルや施設の予約システムにおいて、ユーザーが部屋の空き状況を確認する際に使用されます。
// [Condensing] 部屋の空き状況を名前またはIDで確認するための選択画面を提供します。
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

// @kind DS-MUTATOR (singly linked list)
// @pre head may be NULL; ID may be absent
// @action unlink FIRST node whose ID==target; free(node)
// @post returns (possibly new) head; IF ID ABSENT => LIST UNCHANGED (NO-OP)
// @io none; @time O(n); @dup removes first match ONLY
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

// Remove exactly one room node from a singly linked list.
// Match: the first node with ID == target.
// Returns: the head pointer (a new head only if the deleted node was the head).
// If no node matches, the list is unchanged and the original head is returned.
// Side effects: unlink and free one node. No printing or input.
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

// PRINT-ONLY; returns void; NO mutation.
// FORMAT (EXACT): Date "%02d/%02d/%d"; Time "%02d:%02d - %02d:%02d"
// ORDER (EXACT): separator -> ID -> User ID -> Room ID -> Date -> Time -> separator
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

// [Logical] この関数は、ユーザーの情報をフォーマットしてコンソールに表示します。
// [Precise] ユーザー構造体のポインタを受け取り、そのID、名前、姓をprintf関数を用いて出力します。
// [Unambiguous] ユーザー情報は、ID、名前、姓の順に一貫したフォーマットで表示されます。
// [Exhaustive] この関数は戻り値を持たず、ユーザー情報を標準出力に表示するだけです。
// [Troubleshooting] ユーザーポインタがNULLの場合、未定義の動作が発生する可能性があります。
// [Contextualizing] この関数は、ユーザー情報を確認したいときやログに記録したいときに使用されます。
void DisplayUserCard(UserPtr User)
{
  printf("\n______________________________\n");
  printf(" - ID: %d", User->ID);
  printf("\n - First Name: %s", User->FirstName);
  printf("\n - Last Name : %s", User->LastName);
  printf("\n______________________________\n");
}

// CLOSED interval [from, to]; return value is the INPUT as-is (no clamping).
// PROMPT (EXACT, only when out-of-range): "\nPlease enter a number in the range [%d-%d]: "
// scanf once, then re-read until from <= choice <= to; non-numeric NOT handled; console I/O only.
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

// Contract: interactive menu loop (1..5) that DISPATCHES to sub-screens.
// Menu items (via ReadIntBetween(1,5)):
//   1 -> AddNewReservationScreen(ReservationList, UserList, RoomList)
//   2 -> DeleteReservationScreen(ReservationList, RoomList)
//   3 -> DisplayReservationsScreen(*ReservationList)
//   4 -> SortReservationsByDateScreen(ReservationList)
//   5 -> Exit this menu (return to caller)
// Effects: console I/O here; actual data mutations (add/delete/sort) occur
//          INSIDE the called screens, not in this dispatcher.
// Preconditions: pointers may be NULL (empty lists are allowed);
//                called screens are responsible for NULL-safe handling.
// Postconditions: returns when user selects 5; ReservationList pointer may be
//                 updated by sub-screens (mutations are propagated via pointer).
// Troubleshooting:
//   - If selection seems ignored, check that ReadIntBetween enforces [1..5].
//   - Screen clears via system("cls"); on non-Windows consoles it may no-op.
// Context: reservation domain entrypoint used by MainMenu.
// Complexity: O(k) user interactions; no heap work here beyond callees.
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

// @kind UI-SCREEN; @io scan+print; @ds NO-MUTATION
// @action read first name -> SearchUserByFirstName(...,&IsExist) -> show result -> PauseProgram()
// @post if IsExist==false prints "User not Found!"; list NEVER changed
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

// Contract: bootstrap only; NO domain logic.
// Action: call MainMenu(); return 0.
// Effects: console I/O happens inside menus; no allocation/cleanup here.
// Troubleshooting: intended as thin entrypoint; memory lifecycle is managed
//                  by callees; process exits when menus finish.
int main()
{
  MainMenu();
  return 0;
}