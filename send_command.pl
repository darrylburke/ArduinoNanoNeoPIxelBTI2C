#!/usr/bin/perl
# For Linux/Raspberry pi BT connection to Arduino
use Device::SerialPort;
my $port = Device::SerialPort->new("/dev/rfcomm1");

$port->baudrate(9600); # Configure this to match your device
$port->databits(8);
$port->parity("none");
$port->stopbits(1);

#$port->write("WORM|");
print "Sending:[".$ARGV[0]."]\n";
$port->write($ARGV[0]);

# Writing to the serial port

while (1) {
    my $char = $port->lookfor();
    if ($char) {
        print "Received character: $char \n";
        exit;
    }
    $port->lookclear; # needed to prevent blocking
    sleep (1);
}
