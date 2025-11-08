// === Extracted misunderstood snippets: L7 / gpt-4o ===
// source: data\levels\L7\Meeting_Room_Reservation_System_L7.c

// [Logical] この関数は、予約リストから指定されたIDの予約を削除します。
// [Precise] リストを順に走査し、指定されたIDと一致する予約を見つけたら、そのノードをリストから削除します。
// [Unambiguous] 指定されたIDの予約を見つけた場合、そのノードをリストから削除し、メモリを解放します。
// [Exhaustive] 指定されたIDの予約が見つかると削除され、リストの先頭が変更される可能性があります。見つからない場合はリストは変更されません。
// [Troubleshooting] 削除する予約が見つからない場合、リストはそのまま返され、特にエラー処理は行われません。
// [Contextualizing] この関数は、予約管理システムなどで、特定の予約を削除する際に使用されます。
// [Condensing] 指定されたIDの予約をリストから削除し、メモリを解放します。

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
// [Unambiguous] 関数は、指定されたIDを持つノードを見つけて削除し、リストの先頭が削除された場合も考慮しています。
// [Exhaustive] 削除されたノードのメモリを解放し、更新されたリストの先頭を返しますが、IDが見つからない場合の処理はありません。
// [Troubleshooting] IDが見つからない場合、関数は何も削除せず、リストの先頭をそのまま返します。
// [Contextualizing] この関数は、部屋の情報を管理するアプリケーションで、特定の部屋をリストから削除する際に使用されます。
// [Condensing] 指定されたIDの部屋をリンクリストから削除し、メモリを解放して更新されたリストを返します。

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
// [Unambiguous] ユーザーが入力した整数が範囲外の場合、範囲内の整数が入力されるまで再度入力を求めます。
// [Exhaustive] 範囲内の整数が入力されると、その整数を返します。範囲外の入力があった場合は再入力を促します。
// [Troubleshooting] 範囲外の入力があった場合、範囲内の整数が入力されるまでプロンプトを表示し続けます。
// [Contextualizing] この関数は、ユーザーから特定の範囲内の整数入力を必要とする場面で使用されます。
// [Condensing] ユーザーから指定された範囲内の整数を取得し、範囲外の場合は再入力を促します。

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
// [Unambiguous] ユーザーが1から5の選択肢を入力し、その選択に基づいて対応する予約管理操作を実行します。
// [Exhaustive] 1から4の選択肢はそれぞれ予約追加、削除、表示、ソートを行い、5はメニューを終了します。範囲外の選択はエラーメッセージを表示します。
// [Troubleshooting] 無効な選択肢を入力した場合はエラーメッセージを表示し、再度選択を促します。
// [Contextualizing] この関数は予約管理システムの一部として、ユーザーが予約を管理する際に使用されます。
// [Condensing] 予約管理メニューを表示し、ユーザーの選択に基づいて予約操作を実行する関数です。

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
// [Unambiguous] 入力された名前でユーザーリストを検索し、見つからなければメッセージを表示します。
// [Exhaustive] ユーザーが見つかった場合は何も表示せず、見つからなければ「User not Found!」と表示します。
// [Troubleshooting] 入力エラーや検索エラーに対する具体的な対処はありませんが、ユーザーが見つからない場合にメッセージを表示します。
// [Contextualizing] ユーザー情報を管理するシステム内で、特定のユーザーを名前で検索するために使用されます。
// [Condensing] ユーザーの名前を入力してリストから検索し、見つからない場合はメッセージを表示します。

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
// [Unambiguous] プログラム開始時にMainMenu関数を実行し、終了コード0を返します。
// [Exhaustive] MainMenu関数の実行後、正常終了を示す0を返します。
// [Troubleshooting] この関数自体はエラー処理を行いませんが、MainMenu関数内でのエラー処理が必要です。
// [Contextualizing] この関数はC言語プログラムの開始地点として、ユーザーにメインメニューを提示する際に使用されます。
// [Condensing] プログラム開始時にメインメニューを表示し、正常終了します。

int main()
{
  MainMenu();
  return 0;
}

