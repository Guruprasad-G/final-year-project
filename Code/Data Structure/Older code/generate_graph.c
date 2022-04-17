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
