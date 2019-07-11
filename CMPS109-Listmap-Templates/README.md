# CMPS109-Listmap-Templates
Input is read from each file in turn. Before any processing, each input line is
echoed to cout, preceded by its filename and line number within the file. The
name of cin is printed as a minus sign (-).
Each non-comment line causes some action on the part of the program, as
described below. Before processing a command, leading and trailing white
space is trimmed off of the key and off of the value. White space interior to the
key or value is not trimmed. Input lines are one of the following :

#  
Any input line whose first non-whitespace character is a hash is
ignored as a comment. This means that no key can begin with a hash.
An empty line or a line consisting of nothing but white space is ignored.

key  
A line consisting of at least one non-whitespace character and no equal
sign causes the key and value pair to be printed. If not found, the message
key: key not found
is printed. Note that the characters in italics are not printed exactly.
The actual key is printed. This message is printed to cout.

key =  
If there is only whitespace after the equal sign, the key and value pair is
deleted from the map.

key = value  
If the key is found in the map, its value field is replaced by the new
value. If not found, the key and value are inserted in increasing lexicographic
order, sorted by key. The new key and value pair is printed.

=  
All of the key and value pairs in the map are printed in lexicographic
order.

= value  
All of the key and value pairs with the given value are printed in lexicographic
order sorted by key
