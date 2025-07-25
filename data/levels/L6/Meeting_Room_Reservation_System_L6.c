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
// [Precise] 標準出力にメッセージを表示し、ユーザーの入力を待機してEnterキーが押されると次の処理に進みます。
// [Unambiguous] ユーザーがEnterキーを押すまでプログラムの進行を停止するための関数です。
// [Exhaustive] Enterキー以外の入力を受け取ると意図した動作をしない可能性がありますが、エラー処理は実装されていません。
// [Troubleshooting] 不正な入力があった場合のエラー処理は実装されていないため、入力がEnterキーであることを確認する必要があります。
// [Contextualizing] ユーザーがメインメニューに戻る前に一時停止が必要な場面で使用されます。
void PauseProgram()
{
  printf("\n\nPress Enter to Go To Main Menu...");
  char enter;
  scanf("%s", &enter);
}

// [Logical] この関数は、指定されたタイトルを画面のヘッダーとして表示します。
// [Precise] 画面をクリアした後、タイトルを囲む装飾ラインと共に標準出力に表示します。
// [Unambiguous] 関数は、画面をクリアし、タイトルを装飾ラインで囲んで表示します。
// [Exhaustive] 戻り値はなく、標準出力に影響を与えますが、エラー処理は行っていません。
// [Troubleshooting] エラー処理は実装されておらず、`system("cls")`が失敗した場合の対処はありません。
// [Contextualizing] この関数は、コンソールアプリケーションで画面のヘッダーを表示する際に使用されます。
void ScreenHeader(char Title[MAX_STRING_SIZE])
{
  system("cls");
  printf("\n******************************\n");
  printf("%s", Title);
  printf("\n******************************\n");
}

// [Logical] この関数は、指定された範囲内の整数をユーザーから入力として取得します。
// [Precise] ユーザーから整数を入力させ、その値が指定された範囲内に収まるまで再入力を促します。
// [Unambiguous] ユーザーが入力した整数が範囲外の場合、範囲内の整数が入力されるまで再度入力を求めます。
// [Exhaustive] 範囲内の整数が入力されると、その整数を返します。範囲外の入力があった場合は再入力を促します。
// [Troubleshooting] 範囲外の入力があった場合、範囲内の整数が入力されるまでプロンプトを表示し続けます。
// [Contextualizing] この関数は、ユーザーから特定の範囲内の整数入力を必要とする場面で使用されます。
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
// [Precise] 関数は、指定されたメッセージを表示し、ユーザーから日付の3つの整数（年、月、日）を入力として受け取り、それを構造体stDateに格納して返します。
// [Unambiguous] 関数は標準入力から日付を取得し、stDate構造体に格納して返します。
// [Exhaustive] 関数はユーザーの入力に基づいてstDate構造体を返しますが、入力が不正な場合のエラーハンドリングはありません。
// [Troubleshooting] 不正な入力があった場合、scanfは期待通りに動作しない可能性があるため、入力の検証やエラーチェックを追加する必要があります。
// [Contextualizing] この関数は、ユーザーから日付情報を取得する必要があるプログラム内で使用されます。
stDate ReadDate(char Message[MAX_STRING_SIZE])
{
  stDate Date;
  printf("\n%s", Message);
  scanf("%d %d %d", &Date.Day, &Date.Month, &Date.Year);
  return Date;
}

// [Logical] この関数は、ユーザーから時間を入力として受け取り、stTime構造体として返します。
// [Precise] 関数はメッセージを表示し、ユーザーから時間（時と分）を標準入力から取得してstTime構造体に格納します。
// [Unambiguous] ユーザーにメッセージを表示し、整数として時と分を入力させ、それをstTime構造体に保存して返します。
// [Exhaustive] 関数はユーザー入力に基づいてstTime構造体を返しますが、入力が不正な場合のエラーチェックは行っていません。
// [Troubleshooting] 入力が不正な場合、関数はエラーを処理せず、不正なデータがstTime構造体に格納される可能性があります。
// [Contextualizing] この関数は、ユーザーから時間情報を取得する必要があるプログラム内で使用されます。
stTime ReadTime(char Message[MAX_STRING_SIZE])
{
  stTime Time;
  printf("\n%s", Message);
  scanf("%d %d", &Time.Hour, &Time.Minute);
  return Time;
}

// [Logical] この関数は、2つの日付を比較し、最初の日付が2番目の日付よりも前であるかを判定します。
// [Precise] 年、月、日の順に比較し、最初に異なる値を持つ単位で判定を行います。
// [Unambiguous] 年が異なる場合は年を、年が同じ場合は月を、月も同じ場合は日を比較して判定します。
// [Exhaustive] 最初の日付が2番目の日付より前の場合はtrueを返し、それ以外の場合はfalseを返します。
// [Troubleshooting] この関数はエラー処理を行わないため、入力データの妥当性は呼び出し元で確認する必要があります。
// [Contextualizing] 日付の順序を確認する必要がある場面で使用されます。
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
// [Precise] Time1とTime2の時間（Hour）を比較し、同じ場合は分（Minute）を比較して判定します。
// [Unambiguous] Time1の時間がTime2よりも早い場合はtrueを返し、そうでない場合はfalseを返します。
// [Exhaustive] Time1がTime2よりも早い場合にtrueを返し、同時または遅い場合にはfalseを返します。
// [Troubleshooting] この関数はエラーや例外を考慮していないため、入力データの整合性は事前に確認する必要があります。
// [Contextualizing] この関数は、時間の順序を確認する必要があるスケジューリングや時間管理の場面で使用されます。
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
// [Precise] 画面をクリアした後、プロジェクトの終了メッセージと作成者の情報を表示します。
// [Unambiguous] 画面をクリアし、特定のフォーマットで終了メッセージを表示します。
// [Exhaustive] この関数は画面をクリアし、固定のメッセージを表示するだけで、戻り値や他の影響はありません。
// [Troubleshooting] この関数自体にはエラー処理は含まれておらず、`system("cls")`が失敗した場合の対処はありません。
// [Contextualizing] プログラムの終了時に、ユーザーにプロジェクトの完了を知らせるために使用されます。
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
// [Precise] メニューを表示し、ユーザーの選択を受け取り、その選択に基づいて適切な管理メニュー関数を呼び出します。
// [Unambiguous] ユーザーの選択に基づいて、ユーザー管理、部屋管理、予約管理の各メニューを呼び出し、選択が4の場合はプログラムを終了します。
// [Exhaustive] 選択が1〜3の場合は対応する管理メニューを呼び出し、4の場合は終了し、それ以外はエラーメッセージを表示します。
// [Troubleshooting] 無効な選択をした場合はエラーメッセージを表示し、再度選択を促します。
// [Contextualizing] この関数は、ユーザーがシステムの異なる管理機能にアクセスするためのエントリーポイントとして使用されます。
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
// [Precise] MainMenu関数を呼び出し、その後プログラムを終了します。
// [Unambiguous] プログラム開始時にMainMenu関数を実行し、終了コード0を返します。
// [Exhaustive] MainMenu関数の実行後、正常終了を示す0を返します。
// [Troubleshooting] この関数自体はエラー処理を行いませんが、MainMenu関数内でのエラー処理が必要です。
// [Contextualizing] この関数はC言語プログラムの開始地点として、ユーザーにメインメニューを提示する際に使用されます。
int main()
{
  MainMenu();
  return 0;
}

// =========================================================================== User Management ===========================================================================
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

// [Logical] この関数はユーザー情報を入力し、指定された構造体に格納します。
// [Precise] ユーザーにID、名前、苗字を入力させ、それらをUserPtr構造体のメンバーに格納します。
// [Unambiguous] ユーザーからIDと名前を取得し、構造体の対応するフィールドに保存します。
// [Exhaustive] IDは0から9999の範囲で入力され、NextポインタはNULLに設定されます。
// [Troubleshooting] IDの入力が範囲外の場合、再入力を促す処理が必要です。名前の入力でバッファオーバーフローが発生しないように注意が必要です。
// [Contextualizing] この関数はユーザー情報を管理するシステムで、ユーザー情報を初期化または更新する際に使用されます。
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
// [Precise] ユーザーリストを線形探索し、指定された名前と一致するユーザーを見つけた場合にその情報を表示し、フラグをtrueに設定します。
// [Unambiguous] ユーザーリストを先頭から末尾まで順に確認し、一致する名前が見つかるとそのユーザー情報を表示し、フラグを更新します。
// [Exhaustive] 一致するユーザーが見つかるとその情報を表示し、フラグをtrueに設定しますが、見つからない場合は何も表示されず、フラグは変更されません。
// [Troubleshooting] 関数内でエラー処理は行われておらず、NULLポインタや不正な入力に対する対処は呼び出し元で行う必要があります。
// [Contextualizing] この関数は、ユーザー情報を管理するシステムで、特定の名前を持つユーザーを検索する際に使用されます。
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
// [Precise] リストを先頭から順に走査し、各ユーザーのIDを比較して一致した場合にそのユーザーを返す。
// [Unambiguous] ユーザーIDが一致した場合はそのユーザーへのポインタを返し、一致しない場合は次のユーザーを確認し、リストの終端まで一致しない場合はNULLを返す。
// [Exhaustive] 一致するユーザーが見つかればそのポインタを返し、見つからなければNULLを返す。
// [Troubleshooting] リストがNULLの場合やIDが見つからない場合はNULLを返し、エラーとして扱うことができる。
// [Contextualizing] この関数は、ユーザー管理システムやデータベース操作で特定のユーザーを検索する際に使用される。
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
// [Precise] ユーザーリストが空の場合は新しいユーザーを返し、そうでない場合はリストの末尾に新しいユーザーを追加します。
// [Unambiguous] ユーザーリストがNULLの場合、新しいユーザーがリストの先頭となり、そうでない場合はリストの最後に新しいユーザーが追加されます。
// [Exhaustive] ユーザーリストがNULLの場合は新しいユーザーを返し、正常に追加された場合は更新されたユーザーリストを返します。
// [Troubleshooting] ユーザーリストや新しいユーザーがNULLであることを考慮しており、NULLチェックを行うことでエラーを回避します。
// [Contextualizing] この関数は、ユーザー管理システムで新しいユーザーをリストに追加する際に使用されます。
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
// [Precise] リンクリストを線形探索し、指定されたIDを持つノードを見つけて削除し、リストを更新します。
// [Unambiguous] 指定されたIDを持つユーザーが見つかると、そのノードをリストから削除し、メモリを解放します。
// [Exhaustive] ユーザーが見つからない場合はリストに変更はなく、見つかった場合はそのユーザーを削除して更新されたリストを返します。
// [Troubleshooting] 削除対象のユーザーが見つからない場合は何も削除せず、メモリ解放時にNULLポインタを渡さないように注意が必要です。
// [Contextualizing] この関数は、ユーザー管理システムなどでユーザー情報を動的に管理する際に使用されます。
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
// [Precise] 関数は、新しいユーザーの情報を読み込み、既存のユーザーリストに同じIDのユーザーが存在しない場合にリストに追加します。
// [Unambiguous] ユーザーIDが既存のリストに存在しない場合にのみ、新しいユーザーをリストに追加します。
// [Exhaustive] ユーザーが正常に追加された場合は成功メッセージを表示し、既に存在する場合は警告メッセージを表示します。
// [Troubleshooting] ユーザーIDの重複がある場合は、ユーザーが追加されず、重複の警告メッセージが表示されます。
// [Contextualizing] この関数は、ユーザー管理システムにおいて新しいユーザーを追加する際に使用されます。
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
// [Precise] ユーザーに名前の入力を求め、その名前でユーザーリストを検索し、結果を表示します。
// [Unambiguous] 入力された名前でユーザーリストを検索し、見つからなければメッセージを表示します。
// [Exhaustive] ユーザーが見つかった場合は何も表示せず、見つからなければ「User not Found!」と表示します。
// [Troubleshooting] 入力エラーや検索エラーに対する具体的な対処はありませんが、ユーザーが見つからない場合にメッセージを表示します。
// [Contextualizing] ユーザー情報を管理するシステム内で、特定のユーザーを名前で検索するために使用されます。
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
// [Precise] ユーザーにIDを入力させ、そのIDを用いてユーザーリストからユーザーを検索し、見つかった場合はユーザー情報を表示する。
// [Unambiguous] ユーザーIDを入力し、該当するユーザーが見つかればその情報を表示し、見つからなければ見つからなかった旨を表示する。
// [Exhaustive] ユーザーが見つからない場合はエラーメッセージを表示し、見つかった場合はユーザー情報を表示する。
// [Troubleshooting] ユーザーが見つからない場合は"User not Found!"と表示し、プログラムは続行される。
// [Contextualizing] ユーザー情報をIDで検索する機能が必要な場面で使用される。
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
// [Precise] ユーザーリストが空でない場合、リスト内の各ユーザーを順に表示し、空の場合はその旨を表示します。
// [Unambiguous] ユーザーリストがNULLの場合は空であると表示し、そうでない場合は各ユーザーを順に表示します。
// [Exhaustive] ユーザーリストがNULLの場合は"List is empty!"を表示し、各ユーザーを表示した後にプログラムを一時停止します。
// [Troubleshooting] ユーザーリストがNULLの場合は"List is empty!"と表示されるため、リストが空であることが明示されます。
// [Contextualizing] ユーザー情報を一覧表示する画面で使用される関数です。
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
// [Precise] ユーザーIDを入力し、該当ユーザーを検索して表示、削除の確認を行い、削除が承認された場合はユーザーリストから削除します。
// [Unambiguous] ユーザーIDを入力し、存在する場合は削除確認を行い、承認されれば削除、存在しない場合はエラーメッセージを表示します。
// [Exhaustive] ユーザーが存在し削除された場合は成功メッセージを表示し、存在しない場合はエラーメッセージを表示します。
// [Troubleshooting] ユーザーが見つからない場合はエラーメッセージを表示し、削除は行われません。
// [Contextualizing] この関数は、ユーザー管理システムの一部として、ユーザーを削除する際に使用されます。
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
// [Precise] ユーザーはメニューから選択し、選択に応じたユーザー管理操作が実行されます。
// [Unambiguous] ユーザーは1から6の選択肢を入力し、それに応じた機能が呼び出されます。
// [Exhaustive] 選択肢に応じてユーザー追加、検索、表示、削除が行われ、6を選ぶとメニューを終了します。
// [Troubleshooting] 無効な選択肢が入力された場合にはエラーメッセージが表示されます。
// [Contextualizing] この関数はユーザー管理システムの一部として、ユーザー情報の操作を行う際に使用されます。
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
// [Precise] 部屋のID、名前、予約状況、および予約されている場合はその日時をprintf関数で出力します。
// [Unambiguous] 部屋が予約されているかどうかに応じて、予約の詳細を含むかどうかが決まります。
// [Exhaustive] 部屋が予約されている場合は、予約の日付と時間を表示し、予約されていない場合はその旨を表示します。
// [Troubleshooting] この関数にはエラー処理が含まれておらず、NULLポインタが渡された場合に未定義の動作を引き起こす可能性があります。
// [Contextualizing] この関数は、部屋の詳細をユーザーに表示するために、ユーザーインターフェースやデバッグの場面で使用されます。
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
// [Precise] ユーザーから部屋のIDと名前を入力し、構造体の対応するフィールドに格納します。また、予約状態を初期化し、次の部屋へのポインタをNULLに設定します。
// [Unambiguous] 関数は、部屋のIDを0から9999の範囲で取得し、名前を標準入力から読み取ります。予約状態をfalseに設定し、次のポインタをNULLにします。
// [Exhaustive] 関数は戻り値を持たず、入力されたデータを直接構造体に格納します。エラー処理は特に行っていません。
// [Troubleshooting] IDの入力が範囲外の場合や名前入力でエラーが発生した場合の処理は実装されていません。
// [Contextualizing] この関数は、部屋情報を初期化する際に使用され、ホテル管理システムなどで利用される可能性があります。
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
// [Precise] リストの各要素を順にチェックし、IDが一致する要素を見つけたらそのポインタを返します。
// [Unambiguous] IDが一致する部屋が見つかった場合はそのポインタを、見つからなかった場合はNULLを返します。
// [Exhaustive] 一致するIDが見つかればその部屋のポインタを返し、見つからなければNULLを返します。
// [Troubleshooting] IDが見つからない場合はNULLが返されるため、呼び出し側でNULLチェックを行う必要があります。
// [Contextualizing] この関数は、部屋の情報を管理するシステムで特定の部屋を検索する際に使用されます。
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
// [Precise] 関数は、まず部屋リストが空かどうかを確認し、空であれば新しい部屋を返し、そうでなければリストの末尾まで移動して新しい部屋を追加します。
// [Unambiguous] 部屋リストがNULLの場合、新しい部屋をそのまま返し、そうでない場合はリストの最後に新しい部屋を接続してリスト全体を返します。
// [Exhaustive] 部屋リストがNULLの場合は新しい部屋を返し、そうでない場合はリストの末尾に新しい部屋を追加してからリスト全体を返します。
// [Troubleshooting] 関数内でエラー処理は行われておらず、NULLポインタが渡された場合の動作は未定義です。
// [Contextualizing] この関数は、部屋を管理するシステムにおいて、新しい部屋をリストに追加する際に使用されます。
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
// [Precise] 部屋のリストを順に走査し、名前が一致する部屋を見つけると、その部屋の情報を表示し、存在フラグを真に設定します。
// [Unambiguous] 部屋の名前が一致した場合にのみ情報を表示し、フラグを更新します。
// [Exhaustive] 一致する部屋が見つかるとその情報を表示し、フラグを真に設定しますが、見つからない場合は何も表示せず、フラグは変更されません。
// [Troubleshooting] エラー処理は実装されておらず、部屋のリストがNULLの場合は何も行いません。
// [Contextualizing] この関数は、部屋の情報を管理するシステムなどで、特定の部屋を検索する際に使用されます。
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
// [Precise] リストを先頭から順に探索し、指定されたIDの部屋を見つけたら、そのノードをリストから削除し、メモリを解放します。
// [Unambiguous] 関数は、指定されたIDを持つノードを見つけて削除し、リストの先頭が削除された場合も考慮しています。
// [Exhaustive] 削除されたノードのメモリを解放し、更新されたリストの先頭を返しますが、IDが見つからない場合の処理はありません。
// [Troubleshooting] IDが見つからない場合、関数は何も削除せず、リストの先頭をそのまま返します。
// [Contextualizing] この関数は、部屋の情報を管理するアプリケーションで、特定の部屋をリストから削除する際に使用されます。
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
// [Precise] 新しい部屋の情報をメモリに確保し、入力された情報を元に部屋IDを検索し、存在しなければリストに追加します。
// [Unambiguous] 部屋IDが既に存在する場合は追加せず、存在しない場合のみ新しい部屋をリストに追加します。
// [Exhaustive] 部屋が追加された場合は成功メッセージを表示し、既存の場合はエラーメッセージを表示します。メモリ確保や入力時のエラー処理は行っていません。
// [Troubleshooting] 部屋IDの重複時にはエラーメッセージを表示しますが、メモリ確保や入力エラー時の対処はありません。
// [Contextualizing] この関数は、部屋管理システムの一部として、新しい部屋を追加する際に使用されます。
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
// [Precise] ユーザーに部屋の名前を入力させ、その名前を用いてリスト内の部屋を検索し、見つからなければメッセージを表示します。
// [Unambiguous] ユーザーが入力した名前で部屋を検索し、見つからない場合は「Room not Found!」と表示します。
// [Exhaustive] 部屋が見つかった場合は何も表示せず、見つからない場合はエラーメッセージを表示し、プログラムを一時停止します。
// [Troubleshooting] 部屋が見つからない場合は、エラーメッセージを表示し、ユーザーに再試行の機会を与えるためにプログラムを一時停止します。
// [Contextualizing] この関数は、部屋の管理システムにおいて、ユーザーが特定の部屋を名前で検索する際に使用されます。
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
// [Precise] ユーザーからIDを入力させ、そのIDを用いて部屋リストから部屋を検索し、見つかった場合は部屋情報を表示します。
// [Unambiguous] 関数は指定されたIDで部屋を検索し、見つからなければエラーメッセージを表示し、見つかれば部屋の詳細を表示します。
// [Exhaustive] 部屋が見つからない場合はエラーメッセージを表示し、見つかった場合は部屋情報を表示します。どちらの場合もプログラムを一時停止します。
// [Troubleshooting] 部屋が見つからない場合は、エラーメッセージを表示してユーザーに通知します。
// [Contextualizing] この関数は、ユーザーが部屋の詳細を確認したいときに使用されます。
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
// [Precise] 部屋リストが空であるかを確認し、空であればメッセージを表示し、そうでなければ各部屋情報を順に表示する。
// [Unambiguous] 部屋リストがNULLの場合は空リストのメッセージを表示し、そうでない場合はリスト内の各部屋を順に表示する。
// [Exhaustive] 部屋リストがNULLの場合は"List is empty!"を表示し、リストが存在する場合は各部屋を順に表示する。エラー処理は特に行っていない。
// [Troubleshooting] 部屋リストがNULLの場合に"List is empty!"と表示するが、その他のエラー処理は実装されていない。
// [Contextualizing] この関数は、ユーザーインターフェースの一部として、部屋情報を表示する場面で使用される。
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
// [Precise] ユーザーに部屋IDを入力させ、そのIDの部屋を検索し、見つかった場合は削除の確認を求め、確認後に削除を実行します。
// [Unambiguous] 部屋IDを入力し、存在する場合は削除確認を行い、ユーザーが承認した場合にのみ削除を実行します。
// [Exhaustive] 部屋が見つかれば削除し、成功メッセージを表示し、見つからなければエラーメッセージを表示します。
// [Troubleshooting] 部屋が見つからない場合はエラーメッセージを表示し、削除操作は行われません。
// [Contextualizing] この関数は、部屋管理システムで部屋を削除する際に使用されます。
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
// [Precise] ユーザーに名前またはIDでの検索方法を選択させ、その選択に基づいて対応する検索関数を呼び出します。
// [Unambiguous] ユーザーの選択に応じて、名前での検索またはIDでの検索を行うことができるように分岐処理を行います。
// [Exhaustive] 選択肢が1または2の場合にそれぞれの検索関数を呼び出し、それ以外の選択肢ではエラーメッセージを表示します。
// [Troubleshooting] 無効な選択肢が入力された場合にエラーメッセージを表示し、再入力を促します。
// [Contextualizing] この関数は、ホテルや施設の予約システムにおいて、ユーザーが部屋の空き状況を確認する際に使用されます。
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
// [Precise] ユーザーに1から7の選択肢を提示し、選択に応じて部屋の追加、検索、表示、削除、空室確認、またはメインメニューへの移行を行います。
// [Unambiguous] 選択肢に基づいて、対応する部屋管理機能を呼び出すか、メインメニューに戻ります。
// [Exhaustive] 選択肢が1から6の場合は対応する処理を行い、7の場合はメニューを終了します。無効な選択肢にはエラーメッセージを表示します。
// [Troubleshooting] 無効な選択肢を入力した場合、エラーメッセージを表示しますが、それ以外のエラー処理は実装されていません。
// [Contextualizing] この関数は、部屋管理システムのユーザーインターフェースとして使用され、ユーザーが部屋情報を管理する際に呼び出されます。
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
// [Precise] 予約情報の各フィールドを取得し、フォーマットされた文字列として標準出力に表示します。
// [Unambiguous] Reservation構造体のメンバーを使用して、予約の詳細を明確に表示します。
// [Exhaustive] この関数は出力を行うだけで、戻り値はなく、他のシステムへの影響もありません。
// [Troubleshooting] ReservationがNULLの場合、プログラムがクラッシュする可能性があるため、NULLチェックを追加することが推奨されます。
// [Contextualizing] この関数は、予約管理システムで予約の詳細をユーザーに表示する際に使用されます。
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
// [Precise] リストの先頭から順に各予約のIDを比較し、一致する予約が見つかればそのポインタを返します。
// [Unambiguous] リストを線形探索し、IDが一致する最初の予約を見つけた時点でそのポインタを返し、見つからない場合はNULLを返します。
// [Exhaustive] 一致する予約が見つかればその予約のポインタを返し、見つからなければNULLを返します。
// [Troubleshooting] 予約リストがNULLの場合やIDが見つからない場合はNULLを返しますが、エラー処理は特に行っていません。
// [Contextualizing] この関数は、予約管理システムなどで特定の予約を検索する際に使用されます。
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
// [Precise] リストを順に走査し、指定されたIDと一致する予約を見つけたら、そのノードをリストから削除します。
// [Unambiguous] 指定されたIDの予約を見つけた場合、そのノードをリストから削除し、メモリを解放します。
// [Exhaustive] 指定されたIDの予約が見つかると削除され、リストの先頭が変更される可能性があります。見つからない場合はリストは変更されません。
// [Troubleshooting] 削除する予約が見つからない場合、リストはそのまま返され、特にエラー処理は行われません。
// [Contextualizing] この関数は、予約管理システムなどで、特定の予約を削除する際に使用されます。
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
// [Precise] ユーザーから予約の日付と開始・終了時間を入力させ、それを予約情報と部屋情報に格納します。
// [Unambiguous] ユーザー入力を通じて予約の日付と時間を取得し、予約情報と部屋情報の両方に同じ値を設定します。
// [Exhaustive] 関数は予約情報と部屋情報のDate、Start_Time、End_Timeを設定し、部屋の予約状態をtrueにします。
// [Troubleshooting] ユーザー入力が不適切な場合、ReadDateやReadTime関数でエラー処理が行われる必要がありますが、この関数内では特にエラー処理は行われていません。
// [Contextualizing] この関数は、予約システムにおいてユーザーから予約情報を取得し、部屋の予約状態を更新する際に使用されます。
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
// [Precise] ユーザーから予約ID、ユーザーID、部屋IDを入力させ、既存のデータと照合して有効な情報を取得します。
// [Unambiguous] 予約IDは既存の予約と重複しないこと、ユーザーIDは存在すること、部屋IDは予約可能であることを確認します。
// [Exhaustive] 予約ID、ユーザーID、部屋IDの入力が完了し、部屋が予約可能であれば、予約期間を設定し、予約情報のリストに追加します。
// [Troubleshooting] 入力されたIDが無効または重複している場合、再入力を促すメッセージを表示します。
// [Contextualizing] この関数は予約管理システムの一部として、新しい予約情報を追加する際に使用されます。
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
// [Precise] 予約リストが空の場合は新しい予約をそのまま返し、そうでない場合はリストの末尾を探索して新しい予約を追加します。
// [Unambiguous] 予約リストがNULLの場合、新しい予約を返し、そうでない場合はリストの最後に新しい予約を接続します。
// [Exhaustive] 予約リストがNULLなら新しい予約を返し、そうでない場合は更新された予約リストを返します。
// [Troubleshooting] エラー処理は特に行われておらず、入力がNULLである場合の動作のみを考慮しています。
// [Contextualizing] この関数は、予約管理システムで新しい予約をリストに追加する際に使用されます。
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
// [Precise] 関数は、まず画面ヘッダーを表示し、次に新しい予約のメモリを確保し、ユーザーと部屋のリストを基に予約情報を読み込み、予約リストに新しい予約を追加します。
// [Unambiguous] 関数は、予約情報を読み込み、予約リストに追加し、成功メッセージを表示してプログラムを一時停止します。
// [Exhaustive] 関数は、新しい予約が正常に追加されたことを示すメッセージを表示し、プログラムを一時停止しますが、メモリ確保や予約追加が失敗した場合の処理は含まれていません。
// [Troubleshooting] メモリ確保や予約追加が失敗した場合のエラーハンドリングが実装されていないため、これらのケースに対する対策が必要です。
// [Contextualizing] この関数は、予約管理システムの一部として、新しい予約を追加する際に使用されます。
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
// [Precise] ユーザーに予約IDを入力させ、そのIDに基づいて予約を検索し、確認後に削除します。
// [Unambiguous] 予約IDを入力し、存在する場合は削除確認を行い、ユーザーが承認した場合に削除を実行します。
// [Exhaustive] 予約が見つかった場合は削除し、成功メッセージを表示しますが、見つからない場合はエラーメッセージを表示します。
// [Troubleshooting] 予約が見つからない場合は、エラーメッセージを表示して処理を終了します。
// [Contextualizing] この関数は、予約管理システム内で予約を削除する際に使用されます。
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
// [Precise] 予約リストが空かどうかをチェックし、空でない場合は各予約を順に表示します。
// [Unambiguous] 予約リストがNULLの場合は'List is empty!'と表示し、そうでない場合は各予約を順に表示します。
// [Exhaustive] 予約リストがNULLの場合は'List is empty!'を表示し、リストが存在する場合は各予約を番号付きで表示します。
// [Troubleshooting] 予約リストがNULLの場合は適切に'List is empty!'と表示されるため、エラーは発生しません。
// [Contextualizing] この関数は、予約情報をユーザーに表示する必要がある場面で使用されます。
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
// [Precise] 一時変数を用いて、2つの予約構造体の各フィールドの値を交換している。
// [Unambiguous] Reservation1とReservation2の各フィールド（ID、UserID、RoomID、Date、Start_Time、End_Time）を正確に入れ替える。
// [Exhaustive] 関数はvoid型で戻り値はなく、直接渡された予約データの内容を変更する。
// [Troubleshooting] エラー処理は含まれておらず、無効なポインタが渡された場合の動作は未定義。
// [Contextualizing] 予約データを管理するシステム内で、予約情報の入れ替えが必要な場面で使用される。
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
// [Precise] リスト内の各予約を二重ループで比較し、必要に応じてデータを入れ替えることでソートを行います。
// [Unambiguous] 各予約のデータを比較し、日付が前のものを先にするようにデータを交換します。
// [Exhaustive] 関数はソートされた予約リストを返し、リスト内のデータを直接変更します。
// [Troubleshooting] 入力リストがNULLの場合や、日付比較関数が正しく動作しない場合に注意が必要です。
// [Contextualizing] この関数は、予約管理システムで予約を日付順に整理する際に使用されます。
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
// [Precise] 予約リストが空でない場合、`SortReservationsByDate`関数を呼び出してリストをソートし、結果を表示します。
// [Unambiguous] 予約リストがNULLでない場合にのみソートを実行し、成功メッセージを表示します。
// [Exhaustive] リストが空の場合はメッセージを表示し、リストが空でない場合はソートを実行して成功メッセージを表示します。
// [Troubleshooting] リストがNULLの場合、ソートは行われず、空のリストであることを通知するメッセージが表示されます。
// [Contextualizing] この関数は、ユーザーが予約リストを日付順に整理したいときに使用されます。
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
// [Precise] メニューを表示し、ユーザーの選択を受け取り、選択に応じて予約の追加、削除、表示、日付でのソートを行います。
// [Unambiguous] ユーザーが1から5の選択肢を入力し、その選択に基づいて対応する予約管理操作を実行します。
// [Exhaustive] 1から4の選択肢はそれぞれ予約追加、削除、表示、ソートを行い、5はメニューを終了します。範囲外の選択はエラーメッセージを表示します。
// [Troubleshooting] 無効な選択肢を入力した場合はエラーメッセージを表示し、再度選択を促します。
// [Contextualizing] この関数は予約管理システムの一部として、ユーザーが予約を管理する際に使用されます。
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