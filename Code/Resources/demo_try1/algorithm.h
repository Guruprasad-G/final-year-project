/*int check_if_in_array(int num, int arr, int len)
{
    int i;
    for(i=0;i<len;i++)
    {
        if(arr[i]==num)
            return 1;
    }
    return 0;
}*/

int dfs(int current_node)
{
    int i,backtrace_node;
    visited_nodes[current_node] = 1;
    for(i=0;i<8;i++)
    {
        if(visited_nodes[graph[current_node][i]] == 0)
        {
            stack[stack_pointer++] = graph[current_node][i];
            return graph[current_node][i];
        }
    }
    backtrace_node = stack[stack_pointer];
    stack[stack_pointer--] = 0;
    return backtrace_node;
}

void bot_movement(int direction)
{
    switch(direction)
    {
        case u:
            moveCar(UP);
        case ur:
            moveCar(RIGHT);
            moveCar(UP);
        case r:
            moveCar(RIGHT);
            moveCar(RIGHT);
            moveCar(UP);
        case dr:
            moveCar(RIGHT);
            moveCar(RIGHT);
            moveCar(RIGHT);
            moveCar(UP);
        case d:
            moveCar(RIGHT);
            moveCar(RIGHT);
            moveCar(RIGHT);
            moveCar(RIGHT);
            moveCar(UP);
        case dl:
            moveCar(LEFT);
            moveCar(LEFT);
            moveCar(LEFT);
            moveCar(UP);
        case l:
            moveCar(LEFT);
            moveCar(LEFT);
            moveCar(UP);
        case ul:
            moveCar(LEFT);
            moveCar(UP);
    }
}

void next_node_movement(int current_node, int orientation)
{
    int next_node = dfs(current_node);
    int directions_arr[] = {u,ur,r,dr,d,dl,l,ul};
    int node_location,where_to_go;
    int i;
    for(i=0;i!=orientation;i++)
    {
        int temp = directions_arr[7];
        for (i=7;i!=0;i--)
            directions_arr[i] = directions_arr[i-1];
        directions_arr[0] = temp;
    }
    if(next_node == current_node-n)
    node_location = u;
    else if(next_node == current_node-n+1)
    node_location = ur;
    else if(next_node == current_node+1)
    node_location = r;
    else if(next_node == current_node+n+1)
    node_location = dr;
    else if(next_node == current_node+n)
    node_location = d;
    else if(next_node == current_node+n-1)
    node_location = dl;
    else if(next_node == current_node-1)
    node_location = l;
    else if(next_node == current_node-n-1)
    node_location = ul;
    else
    {
        //printf("\nUnable to locate next node\n");
        node_location = -1;
    }
    where_to_go = directions_arr[node_location];
    bot_movement(where_to_go);
    int msg[2] = {next_node,node_location};
    return msg;
}
