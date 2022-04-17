void connect_node(int n2, int direction, int n1, int sensor_val)
{
    if(sensor_val > threshold_val)
    {
        return;
    }
    graph[n1][direction] = n2;
    switch(direction)
    {
        case u:
            //printf("%d U %d -> %d D %d\n",n1,n2,n2,n1);
            graph[n2][d] = n1;
            break;
        case ur:
            //printf("%d UR %d -> %d DL %d\n",n1,n2,n2,n1);
            graph[n2][dl] = n1;
            break;
        case r:
            //printf("%d R %d -> %d L %d\n",n1,n2,n2,n1);
            graph[n2][l] = n1;
            break;
        case dr:
            //printf("%d DR %d -> %d UL %d\n",n1,n2,n2,n1);
            graph[n2][ul] = n1;
            break;
        case d:
            //printf("%d D %d -> %d U %d\n",n1,n2,n2,n1);
            graph[n2][u] = n1;
            break;
        case dl:
            //printf("%d DL %d -> %d UR %d\n",n1,n2,n2,n1);
            graph[n2][ur] = n1;
            break;
        case l:
            //printf("%d L %d -> %d R %d\n",n1,n2,n2,n1);
            graph[n2][r] = n1;
            break;
        case ul:
            //printf("%d UL %d -> %d DR %d\n",n1,n2,n2,n1);
            graph[n2][dr] = n1;
            break;
    }
    //printf("\n");
}
