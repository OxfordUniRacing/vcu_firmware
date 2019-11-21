There are two seperate logging subsystems: one for message logging and one for data logging.

## Message log

Text log of error and debug messages  
Each message has a timestamp and severity level  
Supports concurrent logging from multiple threads and ISRs  
Maximum of 255 bytes per message supported

Currently only logging to uart (hence embedded debugger) is supported  
Logging to sd card will be supported in the future  
Only one target can be selected at compile time  
