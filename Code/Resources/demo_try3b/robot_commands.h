void setup_wlan()
{
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP IP address: " + IP);
  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", "Welcome to Swarm Robo\nHere you will find more than one Robo doing a task making it efficient to solve\n For example, this robo will navigate through a maze using DFS algorithm.\nIt will search for a taget that is hidden somewhere in the maze.\nIt will also collect the environment data and send it to other robots as well as the laptop.");});
}

void print_current_node_details()
{
  Serial.println("\nCurrent node = " + current_node);
  Serial.println("Current orientation = " + current_orientation);
  sprintf(string,"%d",current_node);
  server.on("/current_node", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
  sprintf(string,"%d",current_orientation);
  server.on("/current_orientation", HTTP_GET, [](AsyncWebServerRequest *request){request->send_P(200, "text/plain", string);});
}

void reset_robot_servo()
{
  panServo.write(90);
  Serial.println("Resetting Servo to 90");
  delay(2000);
}

void print_details_from_robot()
{
  Serial.println("Before going to next node, this is the graph connections :- ");
  Serial.println("N - \tU\tUR\tR\tDR\tD\tDL\tL\tUL");
  print_graph(); Serial.println();
  pretty_print_graph(); Serial.println();
  Serial.print("Printing Stack values");
  for(i=0;i<8;i++)
  {
    Serial.print(stack[i]);
    Serial.print("  ");
  }
  Serial.println();
}

void move_robot_to_next_position()
{
  Serial.println("Calling next node movement");
  next_node_movement(current_node,current_orientation,arr);
  current_node = arr[0]; current_orientation = arr[1];
  Serial.println("Next node = " + current_node);
  Serial.println("Next orientation = " + current_orientation);
  moveCar(STOP);
  patch_graph();
  delay(2000);
}
