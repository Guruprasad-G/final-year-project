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
