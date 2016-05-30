//
// Raspberry Pi I2C client to send the command strings to the Arduino Nano
//
//
//
//
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
 
// The PiWeather board i2c address
#define ADDRESS 0x04
 
// The I2C bus: This is for V2 pi's. For V1 Model B you need i2c-0
static const char *devName = "/dev/i2c-1";
 
int main(int argc, char** argv) {
 
  if (argc == 1) {
    printf("Supply one or more commands to send to the Arduino\n");
    exit(1);
  }
 
  printf("I2C: Connecting\n");
  int file;
 
  if ((file = open(devName, O_RDWR)) < 0) {
    fprintf(stderr, "I2C: Failed to access %d\n", devName);
    exit(1);
  }
 
  printf("I2C: acquiring buss to 0x%x\n", ADDRESS);
 
  if (ioctl(file, I2C_SLAVE, ADDRESS) < 0) {
    fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", ADDRESS);
    exit(1);
  }
 
  int arg=1;
  int i;
  int len;

    char tosend[64];
    if (0 == sscanf(argv[arg], "%s", &tosend)) {
      fprintf(stderr, "Invalid parameter %d \"%s\"\n", arg, argv[arg]);
      exit(1);
    }
    len=strlen(tosend);
 
    printf("Sending %s\n", tosend);
   
    if (write(file, tosend, len) == len) {
    printf("Sent\n");
    } else {
    printf("Error Sending \n");
 
   }
 
  close(file);
  return (EXIT_SUCCESS);
}
