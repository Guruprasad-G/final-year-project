int check_if_in_array(int num, int arr, int len)
{
    int i;
    for(i=0;i<len;i++)
    {
        if(arr[i]==num)
            return 1;
    }
    return 0;
}

int dfs(int current_node)
{
    int i,bactrace_node;
    visited_nodes[current_node] = 1;
    for(i=0;i<8;i++)
    {
        if(visited_nodes[graph[current_node][i]] == 0)
        {
            stack[stack_pointer++] = graph[current_node][i];
            return graph[current_node][i];
        }
    }
    bactrace_node = stack[stack_pointer];
    stack[stack_pointer--] = 0;
    return backtrace_node;
}

void next_node_movement(int current_node, int next_node, int orientation)
{

}
