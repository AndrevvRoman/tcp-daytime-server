
# TCP Date time server

## Build
To build the project, run

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

## Testing
To run tests

    $ cd build/tests
    $ ./tcp-datetime-server-tests

## Running
To run tests

    $ cd build
    $ ./tcp-datetime-server-tests 9000

9000 - port
You can choose any port that you want

## Connecting
To establish connection with server

    $ telnet 127.0.0.1 9000
    $ Trying 127.0.0.1...
    $ Connected to 127.0.0.1.
    $ Escape character is '^]'.
    $ MSK
    $ Thu Oct 28 12:24:08 MSK 2021
    
If you input timezone that server don't know, you will recieve list of known timezones