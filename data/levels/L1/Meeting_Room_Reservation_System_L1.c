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

// [Logical] この関数は、ユーザーがEnterキーを押すまでプログラムを一時停止し、メインメニューに戻るよう促します。
void PauseProgram()
{
  printf("\n\nPress Enter to Go To Main Menu...");
  char enter;
  scanf("%s", &enter);
}

// [Logical] この関数は、指定されたタイトルを画面のヘッダーとして表示します。
void ScreenHeader(char Title[MAX_STRING_SIZE])
{
  system("cls");
  printf("\n******************************\n");
  printf("%s", Title);
  printf("\n******************************\n");
}

// [Logical] この関数は、指定された範囲内の整数をユーザーから入力として取得します。
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

// [Logical] この関数は、ユーザーから日付を入力として受け取り、それを構造体として返します。
stDate ReadDate(char Message[MAX_STRING_SIZE])
{
  stDate Date;
  printf("\n%s", Message);
  scanf("%d %d %d", &Date.Day, &Date.Month, &Date.Year);
  return Date;
}

// [Logical] この関数は、ユーザーから時間を入力として受け取り、stTime構造体として返します。
stTime ReadTime(char Message[MAX_STRING_SIZE])
{
  stTime Time;
  printf("\n%s", Message);
  scanf("%d %d", &Time.Hour, &Time.Minute);
  return Time;
}

// [Logical] この関数は、2つの日付を比較し、最初の日付が2番目の日付よりも前であるかを判定します。
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

// [Logical] この関数は、Time1がTime2よりも前の時間かどうかを判定します。
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

// [Logical] この関数は、終了画面を表示するためのものです。
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

// [Logical] この関数は、ユーザーにメインメニューを表示し、選択に応じて異なる管理メニューを呼び出す役割を持ちます。
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

// [Logical] この関数はプログラムのエントリーポイントとして、メインメニューを表示するために呼び出されます。
int main()
{
  MainMenu();
  return 0;
}

// =========================================================================== User Management ===========================================================================
// [Logical] この関数は、ユーザーの情報をフォーマットしてコンソールに表示します。
void DisplayUserCard(UserPtr User)
{
  printf("\n______________________________\n");
  printf(" - ID: %d", User->ID);
  printf("\n - First Name: %s", User->FirstName);
  printf("\n - Last Name : %s", User->LastName);
  printf("\n______________________________\n");
}

// [Logical] この関数はユーザー情報を入力し、指定された構造体に格納します。
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

// [Logical] この関数は、ユーザーリストから指定された名前を持つユーザーを検索し、見つかった場合にその情報を表示します。
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

// [Logical] この関数は、ユーザーリストから指定されたIDを持つユーザーを検索する。
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

// [Logical] この関数は、新しいユーザーを既存のユーザーリストの末尾に追加します。
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

// [Logical] この関数は、指定されたIDを持つユーザーをリンクリストから削除します。
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

// [Logical] この関数は、新しいユーザーをユーザーリストに追加するための画面を表示し、ユーザー情報を入力してリストに追加します。
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

// [Logical] この関数は、ユーザーのリストから指定された名前を持つユーザーを検索し、結果を表示します。
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

// [Logical] この関数は、ユーザーIDを入力して該当するユーザーを検索し、その結果を表示する。
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

// [Logical] ユーザーリストを画面に表示する関数です。
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

// [Logical] この関数は、ユーザーリストから指定されたIDのユーザーを削除するための画面を表示します。
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

// [Logical] この関数は、ユーザー管理メニューを表示し、ユーザーに関連する操作を選択して実行します。
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

// [Logical] この関数は、指定された部屋の情報をフォーマットして表示します。
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

// [Logical] この関数は、部屋情報を入力し、構造体に格納するためのものです。
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

// [Logical] この関数は、リンクリストから指定されたIDを持つ部屋を検索して返します。
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

// [Logical] この関数は、新しい部屋を既存の部屋リストの末尾に追加します。
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

// [Logical] この関数は、指定された名前に一致する部屋をリストから検索し、見つかった場合にその情報を表示します。
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

// [Logical] この関数は、リンクリストから指定されたIDの部屋を削除します。
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

// [Logical] この関数は、新しい部屋をリストに追加するための画面を表示し、部屋情報を入力してリストに追加します。
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

// [Logical] この関数は、ユーザーが入力した名前に基づいて部屋を検索し、結果を表示します。
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

// [Logical] この関数は、ユーザーが入力したIDに基づいて部屋を検索し、その結果を表示します。
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

// [Logical] この関数は、部屋のリストを画面に表示する機能を持つ。
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

// [Logical] この関数は、指定されたIDの部屋をリストから削除するための画面を表示し、削除操作を実行します。
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

// [Logical] この関数は、部屋の空き状況を名前またはIDで確認するための画面を表示します。
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

// [Logical] この関数は、部屋管理メニューを表示し、ユーザーの選択に応じて適切な処理を行います。
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

// [Logical] この関数は予約情報をフォーマットして表示します。
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

// [Logical] この関数は、予約リストから指定されたIDの予約を検索して返します。
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

// [Logical] この関数は、予約リストから指定されたIDの予約を削除します。
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

// [Logical] この関数は予約情報と部屋情報に予約期間を読み込み、設定します。
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

// [Logical] この関数は予約情報を入力し、予約リストに追加するためのデータを取得します。
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

// [Logical] この関数は、新しい予約を既存の予約リストの末尾に追加します。
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

// [Logical] この関数は、新しい予約を追加するための画面を表示し、予約情報を取得してリストに追加します。
void AddNewReservationScreen(ReservationPtr *ReservationList, UserPtr UserList, RoomPtr *RoomList)
{
  ScreenHeader("\tAdd New Reservation");

  ReservationPtr NewReservation = (Reservation *)malloc(sizeof(Reservation));
  ReadReservationInfo(*ReservationList, &NewReservation, UserList, RoomList);

  *ReservationList = AddNewReservation(*ReservationList, NewReservation);
  printf("\nReservation Added Successfully!\n");

  PauseProgram();
}

// [Logical] この関数は、予約リストから指定されたIDの予約を削除するためのものです。
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

// [Logical] この関数は、予約リストを表示するための画面を出力します。
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

// [Logical] この関数は、2つの予約データの内容を入れ替える。
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

// [Logical] この関数は、予約リストを日付順にソートします。
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

// [Logical] この関数は、予約リストを日付順にソートするための画面を表示します。
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

// [Logical] この関数は予約管理メニューを表示し、ユーザーの選択に応じた操作を実行します。
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