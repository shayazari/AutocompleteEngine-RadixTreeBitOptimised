dict2: driver2.c parsecsv.c linkedlist.c query.c array.c
	gcc -o dict2 driver2.c parsecsv.c linkedlist.c query.c array.c

dict3: driver3.c parsecsv.c linkedlist.c query.c array.c radix.c
	gcc -g -o dict3 driver3.c parsecsv.c linkedlist.c query.c array.c radix.c
