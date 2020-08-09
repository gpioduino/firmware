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
      unsigned long readTime = (micros() - startTime) / 1000; // micros to milis then apply scale factor

      // timeout occurred
      if (readTime > (TIMEOUT * TIMEOUT_SCALE)) {
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
