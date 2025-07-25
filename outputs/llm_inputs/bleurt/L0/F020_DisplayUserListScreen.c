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
