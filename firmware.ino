#define BPS 9600 // default serial speed
#define TIMEOUT 1000 // default timeout
#define TIMEOUT_SCALE 0.9 // 90% accuracy
#define INPUT_MAX_LENGTH 15 // max size for our input buffer

void setup() {
  Serial.begin(BPS);
  Serial.setTimeout(TIMEOUT);
}

void loop() {
    if (Serial.available() > 0) {

      char input[INPUT_MAX_LENGTH+1];

      unsigned long startTime = micros();
      int n = Serial.readBytesUntil(';', input, INPUT_MAX_LENGTH);
      unsigned long readTime = (micros() - startTime) / 1000; // micros to millis

      // timeout occurred
      if (readTime > (TIMEOUT * TIMEOUT_SCALE)) {
        Serial.write("ERROR TIMEOUT;");
        return;
      }

      // bytes have been read
      if (n > 0) {
        input[n] = '\0'; // null-terminate so C string functions work correctly.
        executeCommand(input);
      }
    }
}

void executeCommand(char *input) {
  char *command = strtok(input, " ");
  if (command == NULL) {
    Serial.write("ERROR NO COMMAND;");
    return;
  }

  if (strcmp(command, "ECHO") == 0) {
    executeEcho(command);
  } else if (strcmp(command, "PM") == 0) {
    executePinMode(command);
  } else if (strcmp(command, "DR") == 0) {
    executeDigitalRead(command);
  } else if (strcmp(command, "DW") == 0) {
    executeDigitalWrite(command);
  } else if (strcmp(command, "AR") == 0) {
    executeAnalogRead(command);
  } else if (strcmp(command, "AW") == 0) {
    executeAnalogWrite(command);
  } else {
    Serial.write("ERROR UNKNOWN COMMAND;");
  }
}

void executeEcho(char *command) {
  char *value = strtok(NULL, " ");
  ack(value);
}

void executePinMode(char *command) {
  int pin = atoi(strtok(NULL, " "));
  char *mode = strtok(NULL, " ");

  if (strcmp(mode, "I") == 0) {
    pinMode(pin, INPUT);
  } else if (strcmp(mode, "O") == 0) {
    pinMode(pin, OUTPUT);
  } else {
    Serial.write("ERROR UNKNOWN MODE;");
    return;
  }

  Serial.write("ACK;");
}

void executeDigitalRead(char *command) {
  int pin = atoi(strtok(NULL, " "));
  int value = digitalRead(pin);

  char c[2];
  c[0] = value > 0 ? 'H' : 'L';
  c[1] = '\0';

  ack(c);
}

void executeDigitalWrite(char *command) {
   int pin = atoi(strtok(NULL, " "));
   char *val = strtok(NULL, " ");

   if (strcmp(val, "H") == 0) {
      digitalWrite(pin, HIGH);
   }else if (strcmp(val, "L") == 0) {
      digitalWrite(pin, LOW);
   } else {
      Serial.write("ERROR UNKNOWN DIGITAL VALUE;");
      return;
   }

   Serial.write("ACK;");
}

void executeAnalogRead(char *command) {
  int pin = atoi(strtok(NULL, " "));
  int value = analogRead(pin);

  char c[4];
  itoa(value, c, 10);
  ack(c);
}

void executeAnalogWrite(char *command) {
  int pin = atoi(strtok(NULL, " "));
  int val = atoi(strtok(NULL, " "));
  analogWrite(pin, val);
  Serial.write("ACK;");
}

void ack(char *value) {
  Serial.write("ACK ");
  Serial.write(value);
  Serial.write(";");
}
