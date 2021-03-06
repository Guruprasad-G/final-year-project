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
    Serial.print("Entering DFS function \n");
    //Serial.print("CURRENT NODE ----------------- %d\n",current_node);
    visited_nodes[current_node] = 1;
    for(i=0;i<8;i++)
    {
        Serial.print("Looping in DFS iter = ");
        Serial.print(i);
        Serial.print("\n");
        if(visited_nodes[graph[current_node][i]] == 0 && graph[current_node][i]!=0)
        {
            stack[stack_pointer++] = graph[current_node][i];
            //Serial.print("Returning           -       Current node to be %d\n",graph[current_node][i]);
            return graph[current_node][i];
        }
    }
    if(stack_pointer>0)
    {
      backtrace_node = stack[stack_pointer];
      stack[stack_pointer--] = 0;
      if(backtrace_node !=0)
      return backtrace_node;
    }
    else
    return -1;
}

void bot_movement(int direction)
{
    int counter = 0;
    if(direction==-1)
      moveCar(STOP);
    switch(direction)
    {
        case u:
            Serial.print("Moving UP\n");
            moveCar(UP);
            /*for(counter=0;counter<5;counter++)
            {
              Serial.print("Moving UP for U\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              moveCar(UP);
            }
            //moveCar(UP);
            //Serial.print("Moving UP\n");*/
            break;
        case ur:
            Serial.print("Moving RIGHT for UR\n");
            moveCar(RIGHT);
            Serial.print("Moving UP for UR\n");
            moveCar(UP);
            /*duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            while(distance<15 || counter!= 4)
            {
              if(counter > 4)
              {
               break; 
              }
              moveCar(RIGHT);
              Serial.print("Moving RIGHT for UR\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              counter++;
            }
            for(counter=0;counter<5;counter++)
            {
              Serial.print("Moving UP for UR\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              moveCar(UP);
            }*/
            break;
        case r:
            Serial.print("Moving RIGHT for R\n");
            moveCar(RIGHT);
            moveCar(RIGHT);
            //moveCar(RIGHT);
            //moveCar(RIGHT);
            Serial.print("Moving UP for R\n");
            moveCar(UP);
            /*duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            while(distance<15 || counter!= 4)
            {
              if(counter > 4)
              {
               break; 
              }
              moveCar(RIGHT);
              Serial.print("Moving RIGHT for R\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              counter++;
            }
            for(counter=0;counter<5;counter++)
            {
              Serial.print("Moving UP for R\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              moveCar(UP);
            }*/
            break;
        case dr:
            Serial.print("Moving RIGHT for DR\n");
            moveCar(RIGHT);
            moveCar(RIGHT);
            moveCar(RIGHT);
            Serial.print("Moving UP for DR\n");
            moveCar(UP);
            /*duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            /*while(distance<15 || counter!= 4)
            {
              if(counter > 4)
              {
               break; 
              }
              moveCar(RIGHT);
              Serial.print("Moving RIGHT for DR\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              counter++;
            }
            for(counter=0;counter<5;counter++)
            {
              Serial.print("Moving UP for DR\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              moveCar(UP);
            }*/
            break;
        case d:
            if(counter==0)
            {
              Serial.print("Moving RIGHT for D\n");
              moveCar(RIGHT);
              moveCar(RIGHT);
              moveCar(RIGHT);
              moveCar(RIGHT);
              Serial.print("Moving UP for D\n");
              moveCar(UP);
              counter = 1;
            }
            else
            {
              Serial.print("Moving LEFT for D\n");
              moveCar(LEFT);
              moveCar(LEFT);
              moveCar(LEFT);
              moveCar(LEFT);
              Serial.print("Moving UP for D\n");
              moveCar(UP);
              counter = 0;
            }
            /*duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            while(distance<15 || counter!= 4)
            {
              if(counter > 4)
              {
               break; 
              }
              moveCar(RIGHT);
              Serial.print("Moving RIGHT for D\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              counter++;
            }
            for(counter=0;counter<5;counter++)
            {
              Serial.print("Moving UP for D\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              moveCar(UP);
            }*/
            break;
        case dl:
            Serial.print("Moving LEFT for DL\n");
            moveCar(LEFT);
            moveCar(LEFT);
            moveCar(LEFT);
            Serial.print("Moving UP for DL\n");
            moveCar(UP);
            /*duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            while(distance<15 || counter!= 4)
            {
              if(counter > 4)
              {
               break; 
              }
              moveCar(LEFT);
              Serial.print("Moving LEFT for DL\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              counter++;
            }
            for(counter=0;counter<5;counter++)
            {
              Serial.print("Moving UP for DL\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              moveCar(UP);
            };*/
            break;
        case l:
            Serial.print("Moving LEFT for L\n");
            moveCar(LEFT);
            moveCar(LEFT);
            Serial.print("Moving UP for L\n");
            moveCar(UP);
            /*duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            while(distance<15 || counter!= 4)
            {
              if(counter > 4)
              {
               break; 
              }
              moveCar(LEFT);
              Serial.print("Moving LEFT for L\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              counter++;
            }
            for(counter=0;counter<5;counter++)
            {
              Serial.print("Moving UP for L\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              moveCar(UP);
            }*/
            break;
        case ul:
            Serial.print("Moving LEFT for UL\n");
            moveCar(LEFT);
            Serial.print("Moving UP for UL\n");
            moveCar(UP);
            /*duration = pulseIn(echoPin, HIGH);
            distance= duration*0.034/2;
            while(distance<15 || counter!= 4)
            {
              if(counter > 4)
              {
               break; 
              }
              moveCar(LEFT);
              Serial.print("Moving LEFT for UL\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              counter++;
            }
            for(counter=0;counter<5;counter++)
            {
              Serial.print("Moving UP for UL\n");
              Serial.print("\n"); Serial.print("Counter value is ---- "); Serial.print(counter); Serial.print("\n");
              moveCar(UP);
            }*/
            break;
    }
}

void next_node_movement(int current_node, int orientation, int arr[])
{
    int next_node = dfs(current_node);
    if(next_node==-1)
    {
      bot_movement(-1);
      Serial.print("Robot completely surrounded or Robot returned to Home node!! END OF RUN!!");
    }
    Serial.print("Returned from DFS\n");
    int directions_arr[] = {u,ur,r,dr,d,dl,l,ul};
    int node_location,where_to_go;
    int i,j;
    for(i=0;i<orientation;i++)
    {
        Serial.print("Looping in Orient change iter = ");
        Serial.print(i);
        Serial.print("\n");
        int temp = directions_arr[7];
        for (j=7;j!=0;j--)
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
