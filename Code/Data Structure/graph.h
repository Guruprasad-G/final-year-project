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

void connect_node(int n1, int direction, int sensor_val, int orientation)
{
    int i,j,n2;
    if(sensor_val > threshold_val)
    {
        return;
    }
    for(i=0;i!=orientation;i++)
    {
        int temp = graph[n1][0];
        for (i=0;i<7;i++)
            graph[n1][i] = graph[n1][i+1];
        graph[n1][n-1] = temp;
    }
    switch(direction)
    {
        case u:
            n2 = n1-n;
            //printf("%d U %d -> %d D %d\n",n1,n2,n2,n1);
            graph[n2][d] = n1;
            break;
        case ur:
            n2 = n1-n+1;
            //printf("%d UR %d -> %d DL %d\n",n1,n2,n2,n1);
            graph[n2][dl] = n1;
            break;
        case r:
            n2 = n1+1;
            //printf("%d R %d -> %d L %d\n",n1,n2,n2,n1);
            graph[n2][l] = n1;
            break;
        case dr:
            n2 = n1+n+1;
            //printf("%d DR %d -> %d UL %d\n",n1,n2,n2,n1);
            graph[n2][ul] = n1;
            break;
        case d:
            n2 = n1+n;
            //printf("%d D %d -> %d U %d\n",n1,n2,n2,n1);
            graph[n2][u] = n1;
            break;
        case dl:
            n2 = n1+n-1;
            //printf("%d DL %d -> %d UR %d\n",n1,n2,n2,n1);
            graph[n2][ur] = n1;
            break;
        case l:
            n2 = n1-1;
            //printf("%d L %d -> %d R %d\n",n1,n2,n2,n1);
            graph[n2][r] = n1;
            break;
        case ul:
            n2 = n1-n-1;
            //printf("%d UL %d -> %d DR %d\n",n1,n2,n2,n1);
            graph[n2][dr] = n1;
            break;
    }
    graph[n1][direction] = n2;
    //printf("\n");
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

void print_graph()
{
    int i,j;
    for(i=0;i<n*n+1;i++)
    {
        printf("%d - \t",i);
        for(j=0;j<8;j++)
        {
            printf("%d\t",graph[i][j]);
        }
        printf("\n");
    }
}

void pretty_print_graph()
{
    int i,j;
    for(i=0;i<n+3;i++)
    {
        for(j=0;j<n*n;j++)
        {
            printf("%c",print_str[i][j]);
        }
        printf("\n");
    }
}

