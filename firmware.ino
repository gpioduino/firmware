#define MAX_LENGTH 15
#define TIMEOUT 950000
#define BPS 9600

void setup() {
  Serial.begin(BPS);
  Serial.setTimeout(TIMEOUT);
}

void loop() {
    if (Serial.available() > 0) {
      
      char input[MAX_LENGTH+1];
      
      unsigned long startTime = micros();
      int n = Serial.readBytesUntil(';', input, MAX_LENGTH);
      unsigned long readTime = micros() - startTime;

      // timeout occurred
      if (readTime > TIMEOUT) {
        Serial.write("ERROR;");
        return;
      }
      
      // bytes have been read
      if (n > 0) {
        input[n] = '\0'; // null-terminate so c string functions work correctly.
        executeCommand(input);
      }
    }
}

void executeCommand(char *input) {
  char *command = strtok(input, " ");
  if (command == NULL) {
    Serial.write("ERROR;");
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
    Serial.write("ERROR;");
  }
}

void executeEcho(char *command) {
 Serial.println("ECHO");
}

void executePinMode(char *command) {
  Serial.println("PIN MODE");
}

void executeDigitalRead(char *command) {
  Serial.println("DIGITAL READ");
}

void executeDigitalWrite(char *command) {
  Serial.println("DIGITAL WRITE");
}

void executeAnalogRead(char *command) {
  Serial.println("ANALOG READ");
}

void executeAnalogWrite(char *command) {
  Serial.println("ANALOG WRITE");
}
