#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STRING_SIZE 50

typedef struct stDate
{
  int Day;
  int Month;
  int Year;
} stDate;

typedef struct stTime
{
  int Hour;
  int Minute;
} stTime;

typedef struct User *UserPtr;
typedef struct User
{
  int ID;
  char FirstName[MAX_STRING_SIZE];
  char LastName[MAX_STRING_SIZE];
  UserPtr Next;
} User;

typedef struct Room *RoomPtr;
typedef struct Room
{
  int ID;
  char Name[MAX_STRING_SIZE];

  stDate Date;
  stTime Start_Time;
  stTime End_Time;

  bool IsReserved;

  RoomPtr Next;
} Room;

typedef struct Reservation *ReservationPtr;
typedef struct Reservation
{
  int ID;
  int UserID;
  int RoomID;

  stDate Date;
  stTime Start_Time;
  stTime End_Time;

  ReservationPtr Next;
} Reservation;

// === RE5 BEGIN ===
// CONTRACT
// @purpose プログラムを一時停止し、ユーザーがEnterキーを押すのを待つ。
// @pre     プログラムが実行中であること。
// @post    ユーザーがEnterキーを押した後、メインメニューに戻る。
// @errors  入力エラーが発生する可能性があるが、特に処理しない。
// DATAFLOW
// @reads   ユーザーからの入力（Enterキー）。
// @writes  コンソールにメッセージを表示。
// @calls   printf関数とscanf関数。
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  メインメニュー
// @flow    プログラムが一時停止し、ユーザーがEnterを押すまで待機。
// === RE5 END ===
void PauseProgram()
{
  printf("\n\nPress Enter to Go To Main Menu...");
  char enter;
  scanf("%s", &enter);
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 画面のヘッダーを表示する
// @pre     タイトル文字列を準備する
// @post    画面がクリアされ、タイトルが表示される
// @errors  タイトルが空の場合、何も表示されない
// DATAFLOW
// @reads   引数のタイトル文字列
// @writes  画面出力
// @calls   system, printf
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  ヘッダー画面
// @flow    ユーザーがタイトルを入力し、ヘッダーが表示される
// === RE5 END ===
void ScreenHeader(char Title[MAX_STRING_SIZE])
{
  system("cls");
  printf("\n******************************\n");
  printf("%s", Title);
  printf("\n******************************\n");
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
// @purpose 会議室の予約管理
// @pre     ユーザーが予約情報を入力する前の状態
// @post    予約情報がデータベースに保存された後の状態
// @errors  入力エラーや予約の重複エラー
// DATAFLOW
// @reads   予約情報の取得
// @writes  新しい予約の作成、会議室情報の更新
// @calls   APIエンドポイントへの呼び出し
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約画面
// @flow    ユーザーが予約情報を入力し、確認後に送信
// === RE5 END ===
stDate ReadDate(char Message[MAX_STRING_SIZE])
{
  stDate Date;
  printf("\n%s", Message);
  scanf("%d %d %d", &Date.Day, &Date.Month, &Date.Year);
  return Date;
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 会議室の予約を管理するための機能
// @pre     ユーザーが予約情報を入力する前の状態
// @post    予約が正常に作成された後の状態
// @errors  入力エラーや予約の競合
// DATAFLOW
// @reads   ユーザーID、会議室ID、開始日時、終了日時
// @writes  予約情報
// @calls   ReadTime関数
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約入力画面
// @flow    ユーザーが情報を入力し、予約を送信
// === RE5 END ===
stTime ReadTime(char Message[MAX_STRING_SIZE])
{
  stTime Time;
  printf("\n%s", Message);
  scanf("%d %d", &Time.Hour, &Time.Minute);
  return Time;
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 日付の比較を行う関数
// @pre     二つの日付を引数として受け取る
// @post    最初の日付が二番目の日付より前であればtrueを返す
// @errors  無効な日付の場合のエラーハンドリングは含まれていない
// DATAFLOW
// @reads   Date1とDate2の年、月、日
// @writes  bool型の結果
// @calls   なし
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  メインメニュー, ユーザー管理メニュー, 会議室管理メニュー
// @flow    ユーザー管理や会議室管理のサブメニューを表示し、選択肢に基づいて操作を実行
// === RE5 END ===
bool IsDate1BeforeDate2(stDate Date1, stDate Date2)
{
  if (Date1.Year < Date2.Year)
    return true;
  else if (Date1.Year == Date2.Year)
  {
    if (Date1.Month < Date2.Month)
      return true;
    else if (Date1.Month == Date2.Month)
    {
      if (Date1.Day < Date2.Day)
        return true;
    }
  }
  return false;
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 時間の比較を行い、Time1がTime2より前かどうかを判定する。
// @pre     Time1とTime2はstTime型の構造体である必要がある。
// @post    Time1がTime2より前であればtrueを返し、そうでなければfalseを返す。
// @errors  引数が無効な場合のエラーハンドリングは実装されていない。
// DATAFLOW
// @reads   Time1.Hour, Time1.Minute, Time2.Hour, Time2.Minute
// @writes  なし
// @calls   なし
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  メインメニュー, ユーザー管理メニュー, 会議室管理メニュー
// @flow    ユーザー管理や会議室管理の選択肢を表示し、選択に応じたサブメニューを表示する。
// === RE5 END ===
bool IsTime1BeforeTime2(stTime Time1, stTime Time2)
{
  if (Time1.Hour < Time2.Hour)
    return true;
  else if (Time1.Hour == Time2.Hour)
  {
    if (Time1.Minute < Time2.Minute)
      return true;
  }
  return false;
}

void UserManagementMenu(UserPtr *UserList);
void RoomManagementMenu(RoomPtr *RoomList);
void ReservationManagementMenu(ReservationPtr *ReservationList, UserPtr UserList, RoomPtr *RoomList);

// === RE5 BEGIN ===
// CONTRACT
// @purpose 終了画面を表示する
// @pre     システムが初期化されていること
// @post    終了メッセージが表示される
// @errors  システムコマンドの失敗
// DATAFLOW
// @reads   なし
// @writes  画面にメッセージを表示
// @calls   system, printf
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  終了画面
// @flow    ユーザーが終了を選択すると表示される
// === RE5 END ===
void ExitScreen()
{
  system("cls");
  printf("\n\t\t\t\t\t\t*********************************\n");
  printf("\t\t\t\t\t\t\t   Exist Screen");
  printf("\n\t\t\t\t\t\t*********************************\n");
  printf("\t\t\t\t\t\t  Project Done by:\n");
  printf("\n\t\t\t\t\t\t  - Name   : Oussama Azzouz");
  printf("\n\t\t\t\t\t\t  - Group  : 05\n\n\n");
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose メインメニューを表示し、ユーザーの選択に基づいて異なる管理メニューを呼び出す。
// @pre     初期化されたユーザー、部屋、予約リスト。
// @post    選択されたメニューに遷移するか、プログラムを終了する。
// @errors  無効な選択肢が入力された場合にエラーメッセージを表示。
// DATAFLOW
// @reads   ユーザーの選択。
// @writes  ユーザー、部屋、予約リストの状態。
// @calls   ユーザー管理メニュー、部屋管理メニュー、予約管理メニュー、終了画面。
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  メインメニュー画面
// @flow    ユーザーの選択に基づいて異なる管理メニューに遷移。
// === RE5 END ===
void MainMenu()
{
  UserPtr UserList = NULL;
  RoomPtr RoomList = NULL;
  ReservationPtr ReservationList = NULL;

  int Choice = 0;
  do
  {
    system("cls");
    printf("\n\n=============================================\n");
    printf("\t\tMain Menu");
    printf("\n=============================================\n");
    printf("  [1] User Management Menu.");
    printf("\n  [2] Room Management Menu.");
    printf("\n  [3] Reservation Management Menu.");
    printf("\n  [4] Exit.");
    printf("\n=============================================\n");
    printf(" - Please enter your choice: ");
    Choice = ReadIntBetween(1, 4);
    switch (Choice)
    {
    case 1:
      UserManagementMenu(&UserList);
      break;
    case 2:
      RoomManagementMenu(&RoomList);
      break;
    case 3:
      ReservationManagementMenu(&ReservationList, UserList, &RoomList);
      break;
    case 4:
      ExitScreen();
      break;
    default:
      printf("\nError!");
      break;
    }
  } while (Choice != 4);
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

// =========================================================================== User Management ===========================================================================
// === RE5 BEGIN ===
// CONTRACT
// @purpose ユーザー情報を表示する
// @pre     UserPtr Userが有効であること
// @post    ユーザー情報がコンソールに表示される
// @errors  UserがNULLの場合は表示しない
// DATAFLOW
// @reads   UserのID、FirstName、LastName
// @writes  コンソール出力
// @calls   printf関数
// ENTITIES & ER
// @entity  User(id, 名前, メールアドレス, 作成日時)
// @er      Reservation.userId→User.id
// UI & FLOW
// @screen  予約表示画面
// @flow    ユーザーが予約を選択し、予約リストを表示する
// === RE5 END ===
void DisplayUserCard(UserPtr User)
{
  printf("\n______________________________\n");
  printf(" - ID: %d", User->ID);
  printf("\n - First Name: %s", User->FirstName);
  printf("\n - Last Name : %s", User->LastName);
  printf("\n______________________________\n");
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose ユーザー情報を読み取る
// @pre     ユーザー情報ポインタが有効であること
// @post    ユーザー情報が正しく設定される
// @errors  入力エラーやメモリ不足
// DATAFLOW
// @reads   ユーザーからの入力
// @writes  ユーザー情報構造体への書き込み
// @calls   ReadIntBetween関数
// ENTITIES & ER
// @entity  User(id, 名前, メールアドレス, 作成日時)
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約表示画面
// @flow    ユーザーが予約を表示/ソートし、ユーザーIDと会議室IDを入力
// === RE5 END ===
void ReadUserInfo(UserPtr *UserInfo)
{
  printf("\nEnter User Info:\n--------------------------");
  printf("\nID: ");
  (*UserInfo)->ID = ReadIntBetween(0, 9999);
  printf("\nFirst Name: ");
  scanf("%s", (*UserInfo)->FirstName);
  printf("\nLast Name : ");
  scanf("%s", (*UserInfo)->LastName);
  (*UserInfo)->Next = NULL;
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose ユーザーの名前で検索し、存在する場合はユーザー情報を表示する。
// @pre     UserListがNULLでないこと。
// @post    ユーザーが見つかった場合、IsExistがtrueに設定される。
// @errors  ユーザーが見つからない場合、IsExistはfalseのまま。
// DATAFLOW
// @reads   UserListからユーザー情報を読み取る。
// @writes  IsExistの値を更新する。
// @calls   DisplayUserCard関数を呼び出す。
// ENTITIES & ER
// @entity  User(id, 名前, メールアドレス, 作成日時)
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約表示画面
// @flow    ユーザーが名前を入力し、予約情報を表示する。
// === RE5 END ===
void SearchUserByFirstName(UserPtr UserList, char FirstName[MAX_STRING_SIZE], bool *IsExist)
{
  UserPtr current = UserList;
  while (current != NULL)
  {
    if (strcmp(current->FirstName, FirstName) == 0)
    {
      DisplayUserCard(current);
      *IsExist = true;
    }
    current = current->Next;
  }
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約管理システムの機能を提供する。
// @pre     ユーザーID、会議室ID、予約情報を入力する。
// @post    予約追加、検索、更新の結果を表示する。
// @errors  ユーザーIDが存在しません、会議室IDが存在しません、予約が重複しています
// DATAFLOW
// @reads   ユーザー情報、会議室情報、予約情報
// @writes  新しい予約情報、更新された予約情報
// @calls   SearchUserByID
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約表示/ソート機能
// @flow    ユーザーが予約情報を入力し、システムが結果を表示する。
// === RE5 END ===
UserPtr SearchUserByID(UserPtr UserList, int ID)
{
  UserPtr current = UserList;
  while (current != NULL)
  {
    if (current->ID == ID)
      return current;

    current = current->Next;
  }
  return NULL;
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 新しいユーザーをリストに追加する
// @pre     UserListがNULLまたは新しいユーザーが提供されること
// @post    新しいユーザーがリストの最後に追加される
// @errors  UserListがNULLの場合、新しいユーザーを返す
// DATAFLOW
// @reads   UserListの現在の状態
// @writes  UserListに新しいユーザーを追加
// @calls   この関数は他の関数から呼び出される
// ENTITIES & ER
// @entity  User(id, 名前, メールアドレス, 作成日時)
// @er      Reservation.userId→User.id
// UI & FLOW
// @screen  予約表示/ソート機能
// @flow    ユーザーが予約リストを表示し、フィルタリングを行う
// === RE5 END ===
UserPtr AddNewUser(UserPtr UserList, UserPtr NewUser)
{
  if (UserList == NULL)
  {
    return NewUser;
  }

  UserPtr current = UserList;
  while (current->Next != NULL)
  {
    current = current->Next;
  }
  current->Next = NewUser;
  return UserList;
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約管理システムの機能を提供する。
// @pre     ユーザーID、会議室ID、予約情報を入力する。
// @post    予約追加、検索、更新の結果を返す。
// @errors  ユーザーIDが存在しません、会議室IDが存在しません、予約が重複しています
// DATAFLOW
// @reads   ユーザー情報、会議室情報、予約情報
// @writes  新しい予約情報、更新された予約情報
// @calls   予約追加、予約検索、予約更新
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約表示/ソート機能
// @flow    ユーザーが予約情報を入力し、システムが結果を表示する
// === RE5 END ===
UserPtr DeleteUserByID(UserPtr UserList, int ID)
{
  UserPtr current = UserList;
  UserPtr previous = NULL;

  while (current != NULL && current->ID != ID)
  {
    previous = current;
    current = current->Next;
  }

  if (previous == NULL)
    UserList = current->Next;
  else
    previous->Next = current->Next;

  free(current);
  return UserList;
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 新しいユーザーを追加するための画面を表示する。
// @pre     ユーザーリストが渡される。
// @post    新しいユーザーが追加されるか、既存のユーザーIDが通知される。
// @errors  メモリ割り当て失敗、ユーザーIDの重複
// DATAFLOW
// @reads   ユーザー情報、ユーザーリスト、ユーザーID
// @writes  新しいユーザーをユーザーリストに追加
// @calls   ScreenHeader, ReadUserInfo, SearchUserByID, AddNewUser, PauseProgram
// ENTITIES & ER
// @entity  User(id (PRIMARY KEY), 名前 (NOT NULL), メールアドレス (NOT NULL, UNIQUE), 作成日時 (NOT NULL))
// @er      Reservation.userId→User.id
// UI & FLOW
// @screen  新しいユーザー追加画面
// @flow    ユーザー情報を入力し、追加ボタンを押す
// === RE5 END ===
void AddNewUserScreen(UserPtr *UserList)
{
  ScreenHeader("\tAdd New User");

  UserPtr NewUser = (User *)malloc(sizeof(User));
  ReadUserInfo(&NewUser);

  if (SearchUserByID(*UserList, NewUser->ID) == NULL)
  {
    *UserList = AddNewUser(*UserList, NewUser);
    printf("\nUser Added Successfully!\n");
  }
  else
  {
    printf("\nUser with ID [%d] already exist!\n", NewUser->ID);
  }

  PauseProgram();
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
// @purpose 予約管理システムの予約追加機能
// @pre     ユーザーID、会議室ID、予約ID、予約開始日時、予約終了日時を入力する。
// @post    予約が追加され、成功メッセージまたはエラーメッセージを表示する。
// @errors  ユーザーIDが存在しません、会議室IDが存在しません、予約が重複しています
// DATAFLOW
// @reads   ユーザーID、会議室ID、予約情報
// @writes  新しい予約情報
// @calls   ユーザーと会議室の存在確認、予約追加処理
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約追加画面
// @flow    ユーザーが必要な情報を入力し、予約を追加する
// === RE5 END ===
void SearchUserByIDScreen(UserPtr UserList)
{
  ScreenHeader("\tSearch By ID");
  int ID;
  printf("\nPlease enter ID: ");
  ID = ReadIntBetween(0, 9999);

  UserPtr User = SearchUserByID(UserList, ID);
  if (User == NULL)
    printf("\nUser not Found!\n");
  else
  {
    DisplayUserCard(User);
  }
  PauseProgram();
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose ユーザーリストを表示する
// @pre     UserListがNULLでないこと
// @post    ユーザー情報が表示される
// @errors  リストが空の場合はメッセージを表示
// DATAFLOW
// @reads   UserListからユーザー情報を読み取る
// @writes  画面にユーザー情報を表示
// @calls   DisplayUserCard関数を呼び出す
// ENTITIES & ER
// @entity  User(id, 名前, メールアドレス, 作成日時)
// @er      Reservation.userId→User.id
// UI & FLOW
// @screen  ユーザーリスト画面
// @flow    ユーザーリストを表示し、空の場合はメッセージを表示
// === RE5 END ===
void DisplayUserListScreen(UserPtr UserList)
{
  ScreenHeader("\tUser List");

  if (UserList == NULL)
  {
    printf("\nList is empty!");
  }
  else
  {
    UserPtr current = UserList;
    int i = 0;
    while (current != NULL)
    {
      printf("\n - User [%d]:", ++i);
      DisplayUserCard(current);
      current = current->Next;
    }
  }
  PauseProgram();
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose ユーザーを削除するための画面を表示する。
// @pre     ユーザーIDを入力する。
// @post    指定されたユーザーが削除されるか、見つからない場合はエラーメッセージを表示する。
// @errors  ユーザーが見つからない場合、または無効な入力があった場合のエラーメッセージ。
// DATAFLOW
// @reads   ユーザーリスト、ユーザーID、ユーザー選択。
// @writes  更新されたユーザーリスト。
// @calls   SearchUserByID, DisplayUserCard, DeleteUserByID, PauseProgram
// ENTITIES & ER
// @entity  User(id, 名前, メールアドレス, 作成日時)
// @er      Reservation.userId→User.id
// UI & FLOW
// @screen  ユーザー削除画面
// @flow    ユーザーIDを入力し、確認後に削除を実行する。
// === RE5 END ===
void DeleteUserScreen(UserPtr *UserList)
{
  ScreenHeader("\tDelete User");

  int ID;
  printf("\nPlease enter ID: ");
  ID = ReadIntBetween(0, 9999);

  UserPtr User = SearchUserByID(*UserList, ID);
  if (User != NULL)
  {
    DisplayUserCard(User);
    char Choice = 'n';
    printf("\n\nAre you sure you want to delete this user? y/n? ");
    scanf(" %c", &Choice);
    if (Choice == 'y')
    {
      *UserList = DeleteUserByID(*UserList, ID);
      printf("\nUser with ID [%d] deleted successfully!\n", ID);
    }
  }
  else
    printf("\nUser wiht ID [%d] not found!\n", ID);

  PauseProgram();
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose ユーザー管理メニューの表示と操作
// @pre     ユーザーリストが渡されること
// @post    ユーザーの追加、検索、表示、削除が行われる
// @errors  無効な選択肢の場合はエラーメッセージを表示
// DATAFLOW
// @reads   ユーザーリストのデータを読み取る
// @writes  ユーザーリストの更新
// @calls   ユーザー追加、検索、表示、削除の各画面関数を呼び出す
// ENTITIES & ER
// @entity  User(id, 名前, メールアドレス, 作成日時)
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  ユーザー管理メニュー
// @flow    ユーザーが選択肢を選び、対応する操作を実行
// === RE5 END ===
void UserManagementMenu(UserPtr *UserList)
{
  int Choice = 0;
  do
  {
    system("cls");
    printf("\n---------------------------------------------\n");
    printf("\t   User Management Menu");
    printf("\n---------------------------------------------\n");
    printf("  [1] Add New User.");
    printf("\n  [2] Search by First Name.");
    printf("\n  [3] Search by ID.");
    printf("\n  [4] Display Users.");
    printf("\n  [5] Delete User.");
    printf("\n  [6] Main Menu.");
    printf("\n---------------------------------------------\n");
    printf(" - Please enter your choice: ");
    Choice = ReadIntBetween(1, 6);
    switch (Choice)
    {
    case 1:
      AddNewUserScreen(UserList);
      break;
    case 2:
      SearchUserByFirstNameScreen(*UserList);
      break;
    case 3:
      SearchUserByIDScreen(*UserList);
      break;
    case 4:
      DisplayUserListScreen(*UserList);
      break;
    case 5:
      DeleteUserScreen(UserList);
    case 6:
      break;
    default:
      printf("\nError!");
      break;
    }
  } while (Choice != 6);
}

// =========================================================================== Room Management ===========================================================================

// === RE5 BEGIN ===
// CONTRACT
// @purpose 会議室の詳細情報を表示する
// @pre     Roomポインタが有効であること
// @post    会議室の情報がコンソールに表示される
// @errors  無効なRoomポインタの場合は未定義の動作
// DATAFLOW
// @reads   Roomのプロパティ（ID, Name, IsReserved, Date, Start_Time, End_Time）
// @writes  コンソールへの出力
// @calls   printf関数
// ENTITIES & ER
// @entity  Room
// @er      Reservation.roomId→Room.id
// UI & FLOW
// @screen  会議室管理画面
// @flow    会議室の詳細情報を表示する
// === RE5 END ===
void DisplayRoomCard(RoomPtr Room)
{
  printf("\n______________________________\n");
  printf(" - ID: %d", Room->ID);
  printf("\n - Name: %s", Room->Name);
  if (Room->IsReserved == true)
  {
    printf("\n - Reserved : Yes");
    printf("\n    => Date: %02d/%02d/%d",
           Room->Date.Day, Room->Date.Month, Room->Date.Year);
    printf("\n    => Time: %02d:%02d - %02d:%02d",
           Room->Start_Time.Hour, Room->Start_Time.Minute,
           Room->End_Time.Hour, Room->End_Time.Minute);
  }
  else
  {
    printf("\n - Reserved : No");
  }
  printf("\n______________________________\n");
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 会議室情報を読み込む
// @pre     RoomPtrが有効であること
// @post    RoomInfoに部屋のIDと名前が設定される
// @errors  無効な入力やメモリの問題
// DATAFLOW
// @reads   ユーザーからの入力（ID、名前）
// @writes  RoomInfoのID、名前、IsReserved、Next
// @calls   ReadIntBetween
// ENTITIES & ER
// @entity  Room(id, 部屋名, 収容人数, 説明, 作成日時)
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  会議室登録画面
// @flow    ユーザーが会議室情報を入力し、保存する
// === RE5 END ===
void ReadRoomInfo(RoomPtr *RoomInfo)
{
  printf("\nEnter Room Info:\n--------------------------");
  printf("\nID: ");
  (*RoomInfo)->ID = ReadIntBetween(0, 9999);
  printf("\nName: ");
  scanf("%s", (*RoomInfo)->Name);

  (*RoomInfo)->IsReserved = false;
  (*RoomInfo)->Next = NULL;
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約管理システムの予約追加、検索、更新機能を提供する。
// @pre     ユーザーが予約情報を入力する。
// @post    予約が追加、検索、更新される。
// @errors  ユーザーIDが存在しません、会議室IDが存在しません、予約が重複しています
// DATAFLOW
// @reads   ユーザーID、会議室ID、予約情報
// @writes  新しい予約情報
// @calls   SearchRoomByID
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約追加画面、予約検索画面、予約更新画面
// @flow    ユーザーが情報を入力し、システムが処理結果を表示する
// === RE5 END ===
RoomPtr SearchRoomByID(RoomPtr RoomList, int ID)
{
  RoomPtr current = RoomList;
  while (current != NULL)
  {
    if (current->ID == ID)
      return current;

    current = current->Next;
  }
  return NULL;
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 新しい会議室を追加する
// @pre     RoomListがNULLまたは新しい部屋が提供されること
// @post    新しい部屋がリストに追加される
// @errors  RoomListがNULLの場合、新しい部屋を返す
// DATAFLOW
// @reads   RoomListの現在の状態を読み取る
// @writes  新しい部屋をRoomListに追加する
// @calls   この関数は他の関数を呼び出さない
// ENTITIES & ER
// @entity  Room
// @er      Reservation.roomId→Room.id
// UI & FLOW
// @screen  会議室管理画面
// @flow    会議室追加リクエストを送信
// === RE5 END ===
RoomPtr AddNewRoom(RoomPtr RoomList, RoomPtr NewRoom)
{
  if (RoomList == NULL)
  {
    return NewRoom;
  }

  RoomPtr current = RoomList;
  while (current->Next != NULL)
  {
    current = current->Next;
  }
  current->Next = NewRoom;
  return RoomList;
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 部屋名で会議室を検索し、存在する場合は詳細を表示する。
// @pre     RoomListがNULLでないこと。
// @post    IsExistがtrueに設定され、該当する会議室の詳細が表示される。
// @errors  該当する会議室が存在しない場合、IsExistはfalseのまま。
// DATAFLOW
// @reads   RoomListから会議室情報を読み取る。
// @writes  IsExistの値を更新する。
// @calls   DisplayRoomCard関数を呼び出して会議室の詳細を表示する。
// ENTITIES & ER
// @entity  Room
// @er      Reservation.roomId→Room.id
// UI & FLOW
// @screen  会議室管理画面
// @flow    ユーザーが会議室名を入力し、検索リクエストを送信する。
// === RE5 END ===
void SearchRoomByName(RoomPtr RoomList, char Name[MAX_STRING_SIZE], bool *IsExist)
{
  RoomPtr current = RoomList;
  while (current != NULL)
  {
    if (strcmp(current->Name, Name) == 0)
    {
      DisplayRoomCard(current);
      *IsExist = true;
    }
    current = current->Next;
  }
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

// === RE5 BEGIN ===
// CONTRACT
// @purpose 新しい部屋を追加するための画面を表示し、部屋情報を入力させる。
// @pre     部屋リストが存在すること。
// @post    新しい部屋が部屋リストに追加される。
// @errors  部屋IDが既に存在する場合、エラーメッセージを表示する。
// DATAFLOW
// @reads   部屋情報を読み込む。
// @writes  新しい部屋を部屋リストに追加する。
// @calls   部屋情報を読み込む関数、部屋検索関数、部屋追加関数、プログラム一時停止関数。
// ENTITIES & ER
// @entity  Room(id, 部屋名, 収容人数, 説明, 作成日時)
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  会議室登録画面
// @flow    ユーザーが部屋情報を入力し、追加ボタンを押す。
// === RE5 END ===
void AddNewRoomScreen(RoomPtr *RoomList)
{
  ScreenHeader("\tAdd New Room");

  RoomPtr NewRoom = (Room *)malloc(sizeof(Room));
  ReadRoomInfo(&NewRoom);

  if (SearchRoomByID(*RoomList, NewRoom->ID) == NULL)
  {
    *RoomList = AddNewRoom(*RoomList, NewRoom);
    printf("\nRoom Added Successfully!\n");
  }
  else
    printf("\nRoom with ID [%d] already exist!\n", NewRoom->ID);

  PauseProgram();
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 会議室を名前で検索する
// @pre     ユーザーが検索したい会議室の名前を入力する
// @post    会議室が見つからない場合、エラーメッセージを表示する
// @errors  会議室が見つからない場合
// DATAFLOW
// @reads   ユーザーが入力した会議室名
// @writes  検索結果の表示
// @calls   SearchRoomByName関数
// ENTITIES & ER
// @entity  Room
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  会議室検索画面
// @flow    ユーザーが名前を入力し、検索結果を表示する
// === RE5 END ===
void SearchRoomByNameScreen(RoomPtr RoomList)
{
  ScreenHeader("\tSearch by Name");

  bool IsExist = false;

  char Name[MAX_STRING_SIZE];
  printf("\nPlease enter name: ");
  scanf("%s", Name);

  SearchRoomByName(RoomList, Name, &IsExist);

  if (IsExist == false)
    printf("\nRoom not Found!\n");

  PauseProgram();
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約情報を追加するための機能
// @pre     ユーザーID、会議室ID、予約開始日時、予約終了日時を入力させる
// @post    予約追加成功メッセージまたはエラーメッセージを表示する
// @errors  ユーザーIDが存在しません、会議室IDが存在しません、予約が重複しています
// DATAFLOW
// @reads   ユーザーID、会議室ID、予約開始日時、予約終了日時
// @writes  新しい予約情報
// @calls   会議室IDの存在チェック
// ENTITIES & ER
// @entity  Reservation, User, Room
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約追加画面
// @flow    ユーザーが情報を入力し、システムが予約を追加する
// === RE5 END ===
void SearchRoomByIDScreen(RoomPtr RoomList)
{
  ScreenHeader("\tSearch By ID");
  int ID;
  printf("\nPlease enter ID: ");
  ID = ReadIntBetween(0, 9999);

  RoomPtr Room = SearchRoomByID(RoomList, ID);

  if (Room == NULL)
    printf("\nRoom not Found!\n");
  else
    DisplayRoomCard(Room);

  PauseProgram();
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 会議室リストを表示する
// @pre     RoomListがNULLでないこと
// @post    全ての会議室情報が表示される
// @errors  リストが空の場合はメッセージを表示
// DATAFLOW
// @reads   RoomListから各会議室情報を読み取る
// @writes  表示用にコンソールに出力
// @calls   DisplayRoomCard関数を呼び出す
// ENTITIES & ER
// @entity  Room
// @er      Reservation.roomId→Room.id
// UI & FLOW
// @screen  会議室管理画面
// @flow    会議室リストを取得して表示
// === RE5 END ===
void DisplayRoomListScreen(RoomPtr RoomList)
{
  ScreenHeader("\t Room List");

  if (RoomList == NULL)
  {
    printf("\nList is empty!");
  }
  else
  {
    RoomPtr current = RoomList;
    int i = 0;
    while (current != NULL)
    {
      printf("\n - Room [%d]:", ++i);
      DisplayRoomCard(current);
      current = current->Next;
    }
  }
  PauseProgram();
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 会議室を削除するための機能
// @pre     削除する会議室のIDを入力する
// @post    指定されたIDの会議室が削除される
// @errors  指定されたIDの会議室が見つからない場合、エラーメッセージを表示
// DATAFLOW
// @reads   会議室のID、ユーザーの選択
// @writes  会議室リストの更新
// @calls   SearchRoomByID, DisplayRoomCard, DeleteRoomByID, PauseProgram
// ENTITIES & ER
// @entity  Room(id, 部屋名, 収容人数, 説明, 作成日時)
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  会議室削除画面
// @flow    ID入力 -> 確認メッセージ -> 削除処理
// === RE5 END ===
void DeleteRoomScreen(RoomPtr *RoomList)
{
  ScreenHeader("\tDelete Room");
  int ID;
  printf("\nPlease enter ID: ");
  ID = ReadIntBetween(0, 9999);
  RoomPtr Room = SearchRoomByID(*RoomList, ID);
  if (Room != NULL)
  {
    DisplayRoomCard(Room);
    char Choice = 'n';
    printf("\n\nAre you sure you want to delete this room? y/n? ");
    scanf(" %c", &Choice);
    if (Choice == 'y')
    {
      *RoomList = DeleteRoomByID(*RoomList, ID);
      printf("\nRoom with ID [%d] deleted successfully!\n", ID);
    }
  }
  else
    printf("\nRoom with ID [%d] not found!\n", ID);

  PauseProgram();
}

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
// @purpose 会議室管理メニューの表示と操作
// @pre     ユーザーがメニューを開くと、選択肢が表示される。
// @post    ユーザーの選択に応じた操作が実行される。
// @errors  無効な選択肢が入力された場合、エラーメッセージを表示する。
// DATAFLOW
// @reads   ユーザーの選択肢を読み取る。
// @writes  選択された操作に基づいて、会議室情報を更新または表示する。
// @calls   各操作に対応する関数を呼び出す。
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  会議室管理画面
// @flow    ユーザーがメニューから操作を選択し、対応する画面に遷移する。
// === RE5 END ===
void RoomManagementMenu(RoomPtr *RoomList)
{
  int Choice = 0;
  do
  {
    system("cls");
    printf("\n---------------------------------------------\n");
    printf("\t   Room Management Menu");
    printf("\n---------------------------------------------\n");
    printf("  [1] Add New Room.");
    printf("\n  [2] Search by Name.");
    printf("\n  [3] Search by ID.");
    printf("\n  [4] Display Rooms.");
    printf("\n  [5] Delete Room.");
    printf("\n  [6] Check is Room Availabe.");
    printf("\n  [7] Main Menu.");
    printf("\n---------------------------------------------\n");
    printf(" - Please enter your choice: ");
    Choice = ReadIntBetween(1, 7);
    switch (Choice)
    {
    case 1:
      AddNewRoomScreen(RoomList);
      break;
    case 2:
      SearchRoomByNameScreen(*RoomList);
      break;
    case 3:
      SearchRoomByIDScreen(*RoomList);
      break;
    case 4:
      DisplayRoomListScreen(*RoomList);
      break;
    case 5:
      DeleteRoomScreen(RoomList);
      break;
    case 6:
      CheckRoomAvailabilityScreen(*RoomList);
      break;
    case 7:
      break;
    default:
      printf("\nError!");
      break;
    }
  } while (Choice != 7);
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
// @purpose 予約情報の検索機能を提供する。
// @pre     予約IDを入力する。
// @post    該当する予約情報を返す。
// @errors  予約が見つからない場合はエラーメッセージを返す。
// DATAFLOW
// @reads   予約リストから予約情報を読み取る。
// @writes  該当する予約情報を返す。
// @calls   SearchReservationByID関数を呼び出す。
// ENTITIES & ER
// @entity  Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約検索画面
// @flow    ユーザーが予約IDを入力し、検索ボタンを押す。
// === RE5 END ===
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
// @purpose 予約期間を読み取り、部屋の予約情報を設定する。
// @pre     ReservationPtrとRoomPtrが有効なポインタであること。
// @post    ReservationInfoとRoomに予約情報が設定される。
// @errors  無効な入力やメモリの問題が発生する可能性がある。
// DATAFLOW
// @reads   ユーザーからの予約日付と時間の入力。
// @writes  ReservationInfoとRoomに予約情報を設定。
// @calls   ReadDateとReadTime関数を呼び出す。
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約画面
// @flow    ユーザーが予約情報を入力し、予約を追加する。
// === RE5 END ===
void ReadReservationPeriod(ReservationPtr *ReservationInfo, RoomPtr *Room)
{
  printf("\nEnter Reservation Date & Time:");
  (*ReservationInfo)->Date = ReadDate(" => Date [Day Month Year]: ");
  (*ReservationInfo)->Start_Time = ReadTime(" => Start Time [Hour Minute]: ");
  (*ReservationInfo)->End_Time = ReadTime(" => End Time [Hour Minute]: ");

  (*Room)->Date = (*ReservationInfo)->Date;
  (*Room)->Start_Time = (*ReservationInfo)->Start_Time;
  (*Room)->End_Time = (*ReservationInfo)->End_Time;
  (*Room)->IsReserved = true;
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約情報を読み取る
// @pre     予約リストとユーザーリストが存在すること
// @post    新しい予約情報がReservationInfoに格納される
// @errors  IDの重複、ユーザーIDの不正、部屋の予約状況
// DATAFLOW
// @reads   ReservationList, UserList, RoomList
// @writes  ReservationInfo
// @calls   ReadIntBetween, SearchReservationByID, SearchUserByID, SearchRoomByID, ReadReservationPeriod
// ENTITIES & ER
// @entity  User, Room, Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約情報入力画面
// @flow    ユーザーID、部屋ID、予約期間を入力し、予約を追加
// === RE5 END ===
void ReadReservationInfo(ReservationPtr ReservationList, ReservationPtr *ReservationInfo, UserPtr UserList, RoomPtr *RoomList)
{
  printf("\nEnter Reservation Info:\n--------------------------");

  printf("\n - ID: ");
  (*ReservationInfo)->ID = ReadIntBetween(0, 9999);
  while (SearchReservationByID(ReservationList, (*ReservationInfo)->ID) != NULL)
  {
    printf("\nReservation with ID [%d] already exist!", (*ReservationInfo)->ID);
    printf("\n - Please enter another ID: ");
    (*ReservationInfo)->ID = ReadIntBetween(0, 9999);
  }

  printf("\n - User ID: ");
  (*ReservationInfo)->UserID = ReadIntBetween(0, 9999);
  while (SearchUserByID(UserList, (*ReservationInfo)->UserID) == NULL)
  {
    printf("\nUser with ID [%d] not found!", (*ReservationInfo)->UserID);
    printf("\n - Please enter a valid User ID: ");
    (*ReservationInfo)->UserID = ReadIntBetween(0, 9999);
  }

  RoomPtr Room;
  printf("\n - Room ID: ");
  (*ReservationInfo)->RoomID = ReadIntBetween(0, 9999);
  while ((Room = SearchRoomByID(*RoomList, (*ReservationInfo)->RoomID)) == NULL || Room->IsReserved == true)
  {
    printf("\nRoom with ID [%d] not available!", (*ReservationInfo)->RoomID);
    printf("\n - Please enter another Room ID: ");
    (*ReservationInfo)->RoomID = ReadIntBetween(0, 9999);
  }

  ReadReservationPeriod(ReservationInfo, &Room);

  (*ReservationInfo)->Next = NULL;
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 新しい予約を追加する
// @pre     ReservationListがNULLでない場合、リストの最後にNewReservationを追加する
// @post    ReservationListが返される
// @errors  ReservationListがNULLの場合、NewReservationを返す
// DATAFLOW
// @reads   ReservationListとNewReservationの情報を読み取る
// @writes  NewReservationをReservationListに追加する
// @calls   この関数は他の関数から呼び出される
// ENTITIES & ER
// @entity  Reservation, User, Room
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約管理画面
// @flow    予約追加リクエストを送信し、全予約を取得する
// === RE5 END ===
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

// === RE5 BEGIN ===
// CONTRACT
// @purpose 新しい予約を追加するための画面を表示し、ユーザーからの入力を受け取る。
// @pre     予約リスト、ユーザーリスト、部屋リストが必要。
// @post    新しい予約が追加され、成功メッセージが表示される。
// @errors  メモリ割り当て失敗や入力エラーが発生する可能性がある。
// DATAFLOW
// @reads   予約リスト、ユーザーリスト、部屋リストから情報を読み取る。
// @writes  新しい予約を予約リストに追加する。
// @calls   ReadReservationInfo、AddNewReservation、PauseProgramを呼び出す。
// ENTITIES & ER
// @entity  Reservation, User, Room
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  新しい予約追加画面
// @flow    ユーザーが予約情報を入力し、送信すると予約が追加される。
// === RE5 END ===
void AddNewReservationScreen(ReservationPtr *ReservationList, UserPtr UserList, RoomPtr *RoomList)
{
  ScreenHeader("\tAdd New Reservation");

  ReservationPtr NewReservation = (Reservation *)malloc(sizeof(Reservation));
  ReadReservationInfo(*ReservationList, &NewReservation, UserList, RoomList);

  *ReservationList = AddNewReservation(*ReservationList, NewReservation);
  printf("\nReservation Added Successfully!\n");

  PauseProgram();
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約の削除を行う
// @pre     予約IDを入力する
// @post    予約が削除されたことを通知する
// @errors  予約が見つからない場合、エラーメッセージを表示する
// DATAFLOW
// @reads   予約リストと部屋リストからデータを読み取る
// @writes  予約リストを更新し、部屋の予約状況を変更する
// @calls   予約を検索し、削除する関数を呼び出す
// ENTITIES & ER
// @entity  Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  削除予約画面
// @flow    ユーザーがIDを入力し、確認後に予約を削除する
// === RE5 END ===
void DeleteReservationScreen(ReservationPtr *ReservationList, RoomPtr *RoomList)
{
  ScreenHeader("\tDelete Reservation");

  int ID;
  ReservationPtr Reservation;
  printf("\nPlease enter ID: ");
  ID = ReadIntBetween(0, 9999);

  if ((Reservation = SearchReservationByID(*ReservationList, ID)) != NULL)
  {
    DisplayReservationCard(Reservation);
    char Choice = 'n';
    printf("\nAre you sure you want to delete this reservation? y/n? ");
    scanf(" %c", &Choice);
    if (Choice == 'y')
    {
      // Delete Room Reseration
      RoomPtr Room = SearchRoomByID(*RoomList, Reservation->RoomID);
      Room->IsReserved = false;
      Room->Date = (stDate){0, 0, 0};
      Room->Start_Time = (stTime){0, 0};
      Room->End_Time = (stTime){0, 0};

      *ReservationList = DeleteReservationByID(*ReservationList, ID);
      printf("\nReservation with ID [%d] deleted successfully!\n", ID);
    }
  }
  else
    printf("\nReservation with ID [%d] not found!\n", ID);

  PauseProgram();
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約リストを表示する
// @pre     予約リストが存在するか確認する
// @post    予約リストを画面に表示する
// @errors  リストが空の場合はメッセージを表示
// DATAFLOW
// @reads   ReservationList
// @writes  画面出力
// @calls   ScreenHeader, DisplayReservationCard, PauseProgram
// ENTITIES & ER
// @entity  Reservation, User, Room
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約一覧画面
// @flow    予約がある場合はリストを表示し、ない場合は空メッセージを表示
// === RE5 END ===
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

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約データを2つの予約間でスワップする。
// @pre     Reservation1とReservation2は有効なポインタである必要がある。
// @post    Reservation1とReservation2のデータが入れ替わる。
// @errors  無効なポインタが渡された場合、未定義の動作が発生する可能性がある。
// DATAFLOW
// @reads   Reservation1とReservation2の各フィールド。
// @writes  Reservation1とReservation2の各フィールド。
// @calls   この関数は他の関数を呼び出さない。
// ENTITIES & ER
// @entity  Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約管理画面
// @flow    予約のスワップ操作が行われる。
// === RE5 END ===
void SwapReservationData(ReservationPtr Reservation1, ReservationPtr Reservation2)
{
  int tmpID = Reservation1->ID;
  int tmpUserID = Reservation1->UserID;
  int tmpRoomID = Reservation1->RoomID;

  stDate tmpDate = Reservation1->Date;
  stTime tmpStart_Time = Reservation1->Start_Time;
  stTime tmpEnd_Time = Reservation1->End_Time;

  Reservation1->ID = Reservation2->ID;
  Reservation1->UserID = Reservation2->UserID;
  Reservation1->RoomID = Reservation2->RoomID;

  Reservation1->Date = Reservation2->Date;
  Reservation1->Start_Time = Reservation2->Start_Time;
  Reservation1->End_Time = Reservation2->End_Time;

  Reservation2->ID = tmpID;
  Reservation2->UserID = tmpUserID;
  Reservation2->RoomID = tmpRoomID;

  Reservation2->Date = tmpDate;
  Reservation2->Start_Time = tmpStart_Time;
  Reservation2->End_Time = tmpEnd_Time;
}

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約を日付でソートする
// @pre     予約リストが存在すること
// @post    日付でソートされた予約リストを返す
// @errors  予約リストがNULLの場合はエラーを返す
// DATAFLOW
// @reads   予約リストの各予約データ
// @writes  ソートされた予約リスト
// @calls   IsDate1BeforeDate2, SwapReservationData
// ENTITIES & ER
// @entity  Reservation, User, Room
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約一覧画面
// @flow    日付でソートされた予約を表示
// === RE5 END ===
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

// === RE5 BEGIN ===
// CONTRACT
// @purpose 予約を日付でソートする
// @pre     予約リストが空でないことを確認する
// @post    予約が正常にソートされ、メッセージが表示される
// @errors  リストが空の場合、エラーメッセージを表示
// DATAFLOW
// @reads   予約リストの内容
// @writes  ソートされた予約リスト
// @calls   SortReservationsByDate関数
// ENTITIES & ER
// @entity  Reservation
// @er      Reservation.userId→User.id; Reservation.roomId→Room.id
// UI & FLOW
// @screen  予約日付ソート画面
// @flow    ユーザーが予約を日付でソートするリクエストを送信
// === RE5 END ===
void SortReservationsByDateScreen(ReservationPtr *ReservationList)
{
  ScreenHeader("  Sort Reservations By Date");

  if (*ReservationList == NULL)
  {
    printf("\nList is empty!");
  }
  else
  {
    *ReservationList = SortReservationsByDate(*ReservationList);
    printf("\nReservations Sorted Successfully!\n");
  }
  PauseProgram();
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