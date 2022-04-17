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
