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
