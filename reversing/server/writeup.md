# Writeup

This task will require the contestant to reverse a server binary to
understand the protocol in use, and then use said protocol in a program
of their own (or netcat) to send a correctly formatted packet to get the
flag from the server side.

The protocol is like this:

    +---+---+------------ - -
    | t | l | payload
    +---+---+------------ - -

    t: type
    l: length

The type-field can have two values, 't' and 'r'. If anything else is submitted
the server will dump the contents of the folder it is run in. 'r' tells the
server to read the flag file, w tells the server to write (not implemented).

the length field tells the server how much to read if the type field is set
to 'r'.

The payload field is used to contain the file name to be read/written.

## reversing

This writeup will not go in detail about how to reverse the binary. IT is
recommended to open the binary in IDA or ODA to look at the different functions
and code sections. The key part is spotting the lines where the protocol is
analysed and decisions are made, and understanding what happens and why.

### flag
TG17{Sauron did nothing wrong}
