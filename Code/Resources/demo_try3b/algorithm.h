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

void patch_graph()
{
  int i,j;
    for(i=0;i<n*n+1;i++)
    {
        for(j=0;j<8;j++)
        {
            if(graph[i][j] > n*n)
               graph[i][j] = 0;
        }
    }
}

int dfs(int current_node)
{
    int i,backtrace_node;
    visited_nodes[current_node] = 1;
    
    Serial.print("Visited Array for current node -> ");
    for(i=0;i<8;i++)
    {
      Serial.print(visited_nodes[graph[current_node][i]]);
      Serial.print("\t");
    }
    Serial.println();
    
    for(i=0;i<8;i++)
    {
        if(visited_nodes[graph[current_node][i]] == 0 && graph[current_node][i]!=0)
        {
            stack[stack_pointer] = graph[current_node][i];
            
            Serial.println("Stack has this value -> " + stack[stack_pointer]);
            stack_pointer++;
            
            Serial.println("DFS algo is returning this value          -       " + graph[current_node][i]);
            Serial.println("Stack Pointer has this value -> " + stack_pointer);
            
            return graph[current_node][i];
        }
    }
    if(stack_pointer>0)
    {
      stack_pointer--;
      if(stack[stack_pointer] == current_node)
      {
        Serial.print("Repeated value in Stack");
        stack_pointer--;
      }
      Serial.println("Stack Pointer has this value -> " + stack_pointer);
      
      backtrace_node = stack[stack_pointer];
      stack[stack_pointer] = 0;

      if(backtrace_node == current_node)
      {
        Serial.println("Repeated value in Stack");
        stack_pointer--;
        Serial.println("Stack Pointer has this value -> " + stack_pointer);
        backtrace_node = stack[stack_pointer];
        stack[stack_pointer] = 0;
      }
      
      Serial.println("BACKTRACE NODE ----------------->>> " + backtrace_node);
      
      if(backtrace_node !=0)
        return backtrace_node;
    }
    else
    return -1;
}

void bot_movement(int direct)
{
    int counter = 0;
    if(direct==-1)
    {
      moveCar(STOP);
      return;
    }
    switch(direct)
    {
        case u:
            Serial.println("Moving UP");
            moveCar(UP);
            break;
        case ur:
            Serial.println("Moving RIGHT for UR");
            moveCar(RIGHT);
            Serial.println("Moving UP for UR");
            moveCar(UP);
            break;
        case r:
            Serial.println("Moving RIGHT for R");
            moveCar(RIGHT);
            moveCar(RIGHT);
            Serial.println("Moving UP for R");
            moveCar(UP);
            break;
        case dr:
            Serial.println("Moving RIGHT for DR");
            moveCar(RIGHT);
            moveCar(RIGHT);
            moveCar(RIGHT);
            Serial.println("Moving UP for DR");
            moveCar(UP);
            break;
        case d:
            if(counter==0)
            {
              Serial.println("Moving RIGHT for D");
              moveCar(RIGHT);
              moveCar(RIGHT);
              moveCar(RIGHT);
              moveCar(RIGHT);
              Serial.println("Moving UP for D");
              moveCar(UP);
              counter = 1;
            }
            else
            {
              Serial.println("Moving LEFT for D");
              moveCar(LEFT);
              moveCar(LEFT);
              moveCar(LEFT);
              moveCar(LEFT);
              Serial.println("Moving UP for D");
              moveCar(UP);
              counter = 0;
            }
            break;
        case dl:
            Serial.println("Moving LEFT for DL");
            moveCar(LEFT);
            moveCar(LEFT);
            moveCar(LEFT);
            Serial.println("Moving UP for DL");
            moveCar(UP);
            break;
        case l:
            Serial.println("Moving LEFT for L");
            moveCar(LEFT);
            moveCar(LEFT);
            Serial.println("Moving UP for L");
            moveCar(UP);
            break;
        case ul:
            Serial.println("Moving LEFT for UL");
            moveCar(LEFT);
            Serial.println("Moving UP for UL");
            moveCar(UP);
            break;
    }
}

void next_node_movement(int current_node, int orientation, int arr[])
{
    int next_node = dfs(current_node);
    if(next_node==-1)
    {
      bot_movement(-1);
      Serial.println("Robot completely surrounded or Robot returned to Home node!! END OF RUN!!");
      return ;
    }
    int directions_arr[] = {u,ur,r,dr,d,dl,l,ul};
    int node_location,where_to_go;
    int i,j;
    for(i=0;i<orientation;i++)
    {
        int temp = directions_arr[7];
        for (j=7;j!=0;j--)
            directions_arr[j] = directions_arr[j-1];
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
    
    Serial.println("Inside bot_movement algorithm");
    Serial.print("Dirrection array values -> ");
    for(i=0;i<8;i++)
    {
      Serial.print(directions_arr[i]);
      Serial.print("  ");
    }
    Serial.println();
    
    where_to_go = directions_arr[node_location];
    
    Serial.println("Where to go next -> " + where_to_go);
    
    bot_movement(where_to_go);
    
    arr[0] = next_node;
    arr[1] = node_location;
}
