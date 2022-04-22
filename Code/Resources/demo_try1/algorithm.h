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
    //Serial.print("CURRENT NODE ----------------- %d\n",current_node);
    visited_nodes[current_node] = 1;
    for(i=0;i<8;i++)
    {
        if(visited_nodes[graph[current_node][i]] == 0 && graph[current_node][i]!=0)
        {
            stack[stack_pointer++] = graph[current_node][i];
            //Serial.print("Returning           -       Current node to be %d\n",graph[current_node][i]);
            return graph[current_node][i];
        }
    }
    backtrace_node = stack[stack_pointer];
    stack[stack_pointer--] = 0;
    if(backtrace_node !=0)
    return backtrace_node;
    else
    return 47;
}

void bot_movement(int direction)
{
    int counter = 0;
    if(direction==47)
    moveCar(STOP);
    switch(direction)
    {
        case u:
            moveCar(UP);
            Serial.print("Moving UP\n");
        case ur:
            duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            while(distance<15 && counter!= 10)
            {
              moveCar(RIGHT);
              Serial.print("Moving RIGHT for UR\n");
              counter++;
            }
              
            //moveCar(UP);
        case r:
            duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            while(distance<15 && counter!= 10)
            {
              moveCar(RIGHT);
              Serial.print("Moving RIGHT for R\n");
              counter++;
            }
              
            //moveCar(UP);
        case dr:
            duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            while(distance<15 && counter!= 10)
            {
              moveCar(RIGHT);
              Serial.print("Moving RIGHT for DR\n");
              counter++;
            }
              
            //moveCar(UP);
        case d:
            duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            while(distance<15 && counter!= 10)
            {
              moveCar(RIGHT);
              Serial.print("Moving RIGHT for D\n");
              counter++;
            }
              
            //moveCar(UP);
        case dl:
            duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            while(distance<15 && counter!= 10)
            {
              moveCar(LEFT);
              Serial.print("Moving LEFT for DL\n");
              counter++;
            }
              
            //moveCar(UP);
        case l:
            duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            while(distance<15 && counter!= 10)
            {
              moveCar(LEFT);
              Serial.print("Moving LEFT for L\n");
              counter++;
            }
              
            //moveCar(UP);
        case ul:
            duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            while(distance<15 && counter!= 10)
            {
              moveCar(LEFT);
              Serial.print("Moving LEFT for UL\n");
              counter++;
            }
              
            //moveCar(UP);
    }
}

void next_node_movement(int current_node, int orientation, int arr[])
{
    int next_node = dfs(current_node);
    if(next_node==47)
    bot_movement(47);
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
    arr[0] = next_node;
    arr[1] = node_location;
}
