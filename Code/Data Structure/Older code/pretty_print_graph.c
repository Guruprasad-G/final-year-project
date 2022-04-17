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
