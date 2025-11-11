// === Extracted misunderstood snippets: L99 / gpt-4o ===
// source: data\levels\L99\Meeting_Room_Reservation_System_L99.c

// === RE5 BEGIN ===
// CONTRACT
// @purpose 会議室の空き状況を確認するための画面を表示する。
// @pre     ユーザーが会議室の空き状況を確認したい。
// @post    ユーザーが選択した方法で会議室の空き状況を検索する。
// @errors  無効な選択肢が入力された場合、エラーメッセージを表示する。
// DATAFLOW
// @reads   ユーザーの選択肢を読み取る。
// @writes  選択された方法に基づいて会議室の検索を実行する。
// @calls   SearchRoomByNameScreenまたはSearchRoomByIDScreenを呼び出す。
// ENTITIES & ER
// @entity  Room
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  会議室空き状況確認画面
// @flow    ユーザーが選択肢を選び、対応する検索画面に遷移する。
// === RE5 END ===

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

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約をIDで削除する
// @pre     削除する予約IDを指定する
// @post    予約が削除され、更新された予約リストが返される
// @errors  指定されたIDの予約が存在しない場合、エラーメッセージを返す
// DATAFLOW
// @reads   予約リストから指定されたIDの予約を検索する
// @writes  指定されたIDの予約を削除し、更新された予約リストを返す
// @calls   メモリ解放のためにfree関数を呼び出す
// ENTITIES & ER
// @entity  Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約管理画面
// @flow    ユーザーが予約IDを入力し、削除ボタンを押すと、予約が削除される
// === RE5 END ===

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

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約管理システムの予約追加、検索、更新機能を提供する。
// @pre     ユーザーIDと会議室IDの存在を確認する。
// @post    予約情報をデータベースに保存し、成功メッセージを返す。
// @errors  ユーザーIDが存在しません、会議室IDが存在しません、予約が重複しています。
// DATAFLOW
// @reads   ユーザー情報、会議室情報、予約情報。
// @writes  新しい予約情報。
// @calls   会議室IDの存在チェック、予約情報の更新処理。
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約追加画面、予約検索画面、予約更新画面。
// @flow    ユーザーID、会議室ID、予約情報を入力し、処理結果を表示する。
// === RE5 END ===

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

// =========================================================================== Reservation Management ===========================================================================

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約カードの表示
// @pre     ReservationPtr型の予約情報が必要
// @post    予約情報がフォーマットされた形で表示される
// @errors  予約情報がNULLの場合、未定義の動作が発生する可能性がある
// DATAFLOW
// @reads   Reservationの各フィールド
// @writes  コンソールへの出力
// @calls   printf関数
// ENTITIES & ER
// @entity  Reservation, User, Room
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約表示画面
// @flow    予約情報を取得し、表示する
// === RE5 END ===

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

// === RE5 BEGIN ===
// CONTRACT
// @purpose ユーザーが指定した範囲内の整数を入力するための関数
// @pre     ユーザーが整数を入力する準備をする
// @post    指定された範囲内の整数を返す
// @errors  範囲外の整数が入力された場合、再入力を促す
// DATAFLOW
// @reads   ユーザーからの整数入力
// @writes  なし
// @calls   scanf, printf
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  会議室追加画面
// @flow    会議室情報を入力し、データベースに保存する
// === RE5 END ===

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

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約管理メニューの表示と操作
// @pre     予約リスト、ユーザーリスト、部屋リストを準備すること
// @post    ユーザーの選択に応じた操作を実行すること
// @errors  無効な選択肢が入力された場合にエラーメッセージを表示
// DATAFLOW
// @reads   予約リスト、ユーザーリスト、部屋リスト
// @writes  新しい予約の追加、予約の削除、予約の表示、予約のソート
// @calls   AddNewReservationScreen, DeleteReservationScreen, DisplayReservationsScreen, SortReservationsByDateScreen
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約管理メニュー
// @flow    ユーザーが選択肢を選び、対応する操作を実行
// === RE5 END ===

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

// === RE5 BEGIN ===
// CONTRACT
// @purpose ユーザーの名前で検索する機能
// @pre     ユーザーリストが提供されること
// @post    検索結果が表示されること
// @errors  ユーザーが見つからない場合のエラーメッセージ
// DATAFLOW
// @reads   ユーザーリスト、ユーザーの名前
// @writes  検索結果の表示
// @calls   SearchUserByFirstName
// ENTITIES & ER
// @entity  User(id, 名前, メールアドレス, 作成日時)
// @er      Reservation.userId→User.id
// UI & FLOW
// @screen  名前で検索する画面
// @flow    ユーザーが名前を入力し、結果が表示される
// === RE5 END ===

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

// === RE5 BEGIN ===
// CONTRACT
// @purpose メインメニューを表示するため
// @pre     初期化処理が必要
// @post    メインメニューが表示される
// @errors  メニュー表示に失敗した場合のエラーハンドリング
// DATAFLOW
// @reads   ユーザー情報、部屋情報、予約情報
// @writes  予約情報の作成
// @calls   MainMenu関数
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  メインメニュー画面
// @flow    ユーザーがメインメニューを操作する
// === RE5 END ===

int main()
{
  MainMenu();
  return 0;
}

