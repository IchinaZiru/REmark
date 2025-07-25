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
