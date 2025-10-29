// === Extracted misunderstood snippets: L2 / gpt-4o ===
// source: data\levels\L2\Meeting_Room_Reservation_System_L2.c

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

// [Logical] この関数は、リンクリストから指定されたIDの部屋を削除します。
// [Precise] リストを先頭から順に探索し、指定されたIDの部屋を見つけたら、そのノードをリストから削除し、メモリを解放します。

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

// [Logical] この関数は、指定された範囲内の整数をユーザーから入力として取得します。
// [Precise] ユーザーから整数を入力させ、その値が指定された範囲内に収まるまで再入力を促します。

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

// [Logical] この関数は予約管理メニューを表示し、ユーザーの選択に応じた操作を実行します。
// [Precise] メニューを表示し、ユーザーの選択を受け取り、選択に応じて予約の追加、削除、表示、日付でのソートを行います。

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

// [Logical] この関数は、ユーザーのリストから指定された名前を持つユーザーを検索し、結果を表示します。
// [Precise] ユーザーに名前の入力を求め、その名前でユーザーリストを検索し、結果を表示します。

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

// [Logical] この関数はプログラムのエントリーポイントとして、メインメニューを表示するために呼び出されます。
// [Precise] MainMenu関数を呼び出し、その後プログラムを終了します。

int main()
{
  MainMenu();
  return 0;
}

