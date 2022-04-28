void initialize_graph()
{
    int i,j;
    for(i=0;i<n*n+1;i++)
    {
        for(j=0;j<8;j++)
        {
            graph[i][j] = 0;
        }
    }
    for(i=0;i<n+3;i++)
    {
        for(j=0;j<n*n;j++)
        {
            print_str[i][j] = ' ';
        }
    }
}

void print_graph()
{
    int i,j;
    for(i=0;i<n*n+1;i++)
    {
        Serial.print(i);
        Serial.print(" - \t");
        for(j=0;j<8;j++)
        {
            Serial.print(graph[i][j]);
            Serial.print("\t");
        }
        printf("\n");
    }
} 

void rotate_array(int n1, int orientation)
{
  if(previous_node == n1)
    return;
  else
  {
    for(i=0;i<orientation;i++)
    {
        int temp = graph[n1][0];
        int j;
        for (j=0;j<7;j++)
            graph[n1][j] = graph[n1][j+1];
        graph[n1][8-1] = temp;
    }
    Serial.print("Graph array upon rotation =>   ");
    for(i=0;i<8;i++)
    {
      Serial.print(graph[n1][i]);
      Serial.print("  ");
    }
    Serial.print("\n");
  }
  previous_node = n1;
}

int check_if_val_in_array(int n1, int n2)
{
  int i;
  for(i=0;i<8;i++)
  {
    if(graph[n1][i] == n2)
      return 0;
  }
  return 1;
}

void connect_node(int n1, int direct, int sensor_val, int orientation)
{
    int i,j,n2;
    if(sensor_val < threshold_val)
    {
        return;
    }
    //rotate_array( n1, orientation);
    direct = (direct + orientation)%8;
    switch(direct)
    {
        case u:
            n2 = n1-n;
            if(n2 < 1)
            return;
            //printf("%d U %d -> %d D %d\n",n1,n2,n2,n1);
            Serial.print("\n");
            Serial.print(n1);
            Serial.print(" U ");
            Serial.print(n2);
            Serial.print(" -> ");
            Serial.print(n2);
            Serial.print(" D ");
            Serial.print(n1);
            Serial.print("\n");
            if(check_if_val_in_array(n2,n1))
              graph[n2][d] = n1;
            break;
        case ur:
            n2 = n1-n+1;
            if(n2 < 1)
            return;
            //printf("%d UR %d -> %d DL %d\n",n1,n2,n2,n1);
            Serial.print("\n");
            Serial.print(n1);
            Serial.print(" UR ");
            Serial.print(n2);
            Serial.print(" -> ");
            Serial.print(n2);
            Serial.print(" DL ");
            Serial.print(n1);
            Serial.print("\n");
            if(check_if_val_in_array(n2,n1))
              graph[n2][dl] = n1;
            break;
        case r:
            n2 = n1+1;
            if(n2 < 1)
            return;
            //printf("%d R %d -> %d L %d\n",n1,n2,n2,n1);
            Serial.print("\n");
            Serial.print(n1);
            Serial.print(" R ");
            Serial.print(n2);
            Serial.print(" -> ");
            Serial.print(n2);
            Serial.print(" L ");
            Serial.print(n1);
            Serial.print("\n");
            if(check_if_val_in_array(n2,n1))
              graph[n2][l] = n1;
            break;
        case dr:
            n2 = n1+n+1;
            if(n2 < 1)
            return;
            //printf("%d DR %d -> %d UL %d\n",n1,n2,n2,n1);
            Serial.print("\n");
            Serial.print(n1);
            Serial.print(" DR ");
            Serial.print(n2);
            Serial.print(" -> ");
            Serial.print(n2);
            Serial.print(" UL ");
            Serial.print(n1);
            Serial.print("\n");
            if(check_if_val_in_array(n2,n1))
              graph[n2][ul] = n1;
            break;
        case d:
            n2 = n1+n;
            if(n2 < 1)
            return;
            //printf("%d D %d -> %d U %d\n",n1,n2,n2,n1);
            Serial.print("\n");
            Serial.print(n1);
            Serial.print(" D ");
            Serial.print(n2);
            Serial.print(" -> ");
            Serial.print(n2);
            Serial.print(" U ");
            Serial.print(n1);
            Serial.print("\n");
            if(check_if_val_in_array(n2,n1))
              graph[n2][u] = n1;
            break;
        case dl:
            n2 = n1+n-1;
            if(n2 < 1)
            return;
            //printf("%d DL %d -> %d UR %d\n",n1,n2,n2,n1);
            Serial.print("\n");
            Serial.print(n1);
            Serial.print(" DL ");
            Serial.print(n2);
            Serial.print(" -> ");
            Serial.print(n2);
            Serial.print(" UR ");
            Serial.print(n1);
            Serial.print("\n");
            if(check_if_val_in_array(n2,n1))
              graph[n2][ur] = n1;
            break;
        case l:
            n2 = n1-1;
            if(n2 < 1)
            return;
            //printf("%d L %d -> %d R %d\n",n1,n2,n2,n1);
            Serial.print("\n");
            Serial.print(n1);
            Serial.print(" L ");
            Serial.print(n2);
            Serial.print(" -> ");
            Serial.print(n2);
            Serial.print(" R ");
            Serial.print(n1);
            Serial.print("\n");
            if(check_if_val_in_array(n2,n1))
              graph[n2][r] = n1;
            break;
        case ul:
            n2 = n1-n-1;
            if(n2 < 1)
            return;
            //printf("%d UL %d -> %d DR %d\n",n1,n2,n2,n1);
            Serial.print("\n");
            Serial.print(n1);
            Serial.print(" UL ");
            Serial.print(n2);
            Serial.print(" -> ");
            Serial.print(n2);
            Serial.print(" DR ");
            Serial.print(n1);
            Serial.print("\n");
            if(check_if_val_in_array(n2,n1))
              graph[n2][dr] = n1;
            break;
    }
    if(check_if_val_in_array(n1,n2))
      graph[n1][direct] = n2;
    //printf("\n");
    /*for(i=0;i!=orientation;i++)
    {
        int temp = graph[n1][7];
        for (j=7;j!=0;j--)
            graph[n1][j] = graph[n1][j-1];
        graph[n1][0] = temp;
    }
    Serial.print("Graph array upon reset rotation =>   ");
    for(i=0;i<8;i++)
    {
      Serial.print(graph[n1][i]);
      Serial.print("  ");
    }
    Serial.print("\n");*/
}

void generate_graph()
{
    int node;
    char backslash = '\\';
    int i,j,k=1,m;
    for(i=0;i<n+3;i+=2)
    {
        //printf("Moving Row by row :%d\n",i);
        for(j=0;j<n*n;j++)
        {
            //printf("Moving Column by column : %d\n",j);
            if(j%(n+1)==0)
            {
                //printf("Encountered number position : %d\n",k);
                print_str[i][j] = k+'0';
                //printf("Here => %c\n",print_str[i][j]);
            }
            else
            {
                continue;
            }
            for(m=0;m<8;m++)
            {
                node = graph[k][m];
                if(node!=0)
                {
                    //printf("Node = %d for k = %d, m = %d\n",node,k,m);
                    switch(m)
                    {
                        case 0:
                            //printf("Top\n");
                            print_str[i-1][j] = '|';
                            //pretty_print_graph();
                            break;
                        case 1:
                            //printf("Top Right\n");
                            if(print_str[i-1][j+2] == backslash)
                            {
                                //printf("Updating connection to x\n");
                                print_str[i-1][j+2] = 'x';
                            }
                            else if(print_str[i-1][j+2] != 'x')
                                print_str[i-1][j+2] = '/';
                            //pretty_print_graph();
                            break;
                        case 2:
                            //printf("Right\n");
                            print_str[i][j+2] = '-';
                            //pretty_print_graph();
                            break;
                        case 3:
                            //printf("Bottom Right\n");
                            if(print_str[i+1][j+2] == '/')
                            {
                                //printf("Updating connection to x\n");
                                print_str[i+1][j+2] = 'x';
                            }
                            else if(print_str[i-1][j+2] != 'x')
                                print_str[i+1][j+2] = backslash;
                            //pretty_print_graph();
                            break;
                        case 4:
                            //printf("Bottom\n");
                            print_str[i+1][j] = '|';
                            //pretty_print_graph();
                            break;
                        case 5:
                            //printf("Bottom Left\n");
                            if(print_str[i+1][j-2] == backslash)
                            {
                                //printf("Updating connection to x\n");
                                print_str[i+1][j-2] = 'x';
                            }
                            else if(print_str[i-1][j+2] != 'x')
                                print_str[i+1][j-2] = '/';
                            //pretty_print_graph();
                            break;
                        case 6:
                            //printf("Left\n");
                            print_str[i][j-2] = '-';
                            //pretty_print_graph();
                            break;
                        case 7:
                            //printf("Top Left\n");
                            if(print_str[i-1][j-2] == '/')
                            {
                                //printf("Updating connection to x\n");
                                print_str[i-1][j-2] = 'x';
                            }
                            else if(print_str[i-1][j-2] != 'x')
                                print_str[i-1][j-2] = backslash;
                            //pretty_print_graph();
                            break;
                    }
                }
            }
            k++;
            if(k>n*n)
            {
                return;
            }
        }
    }
}

void pretty_print_graph()
{
    int i,j;
    for(i=0;i<n+3;i++)
    {
        for(j=0;j<n*n;j++)
        {
            Serial.print(print_str[i][j]);
        }
        Serial.print("\n");
    }
}
