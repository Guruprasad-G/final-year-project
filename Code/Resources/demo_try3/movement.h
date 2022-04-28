
void rotate_backward()
{
  digitalWrite(grey_pin, HIGH);
  digitalWrite(purple_pin, LOW);
  digitalWrite(blue_pin, HIGH);
  digitalWrite(green_pin, LOW);
  delay(2250);
}

void rotate_left()
{
  digitalWrite(grey_pin, LOW);
  digitalWrite(purple_pin, HIGH);
  digitalWrite(blue_pin, HIGH);
  digitalWrite(green_pin, LOW);
  delay(686);
}

void rotate_right()
{
  digitalWrite(grey_pin, HIGH);
  digitalWrite(purple_pin, LOW);
  digitalWrite(blue_pin, LOW);
  digitalWrite(green_pin, HIGH);
  delay(686);
}

void rotate_forward()
{
  digitalWrite(grey_pin, LOW);
  digitalWrite(purple_pin, HIGH);
  digitalWrite(blue_pin, LOW);
  digitalWrite(green_pin, HIGH);
  delay(2250);
}

void rotate_stop()
{
  digitalWrite(grey_pin, LOW);
  digitalWrite(purple_pin, LOW);
  digitalWrite(blue_pin, LOW);
  digitalWrite(green_pin, LOW);
  delay(1000);
}

void moveCar(int inputValue)
{
  //Serial.printf("Got value as %d\n", inputValue);
  switch(inputValue)
  {

    case UP:
      rotate_forward();
      break;

    case DOWN:
      rotate_backward();
      break;

    case LEFT:
      rotate_left();
      break;

    case RIGHT:
      rotate_right();
      break;

    case STOP:
      rotate_stop();
      break;

    default:
      rotate_stop();
      break;
  }
}
