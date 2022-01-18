## IPC - POSIX Shared Memory Example



Files 

- [Makefile] (requires GNU `make`)
- [shm-posix-producer.c]
- [shm-posix-consumer.c]

Compile and run

Run on Ubuntu Machine

>  make 

It would display:

> gcc -std=c99 -Wall -Wextra -D_XOPEN_SOURCE=700 -o cons shm-posix-consumer.c -lrt
> gcc -std=c99 -Wall -Wextra -D_XOPEN_SOURCE=700 -o prod  shm-posix-producer.c -lrt

Then run:

> **./prod**  

Would display:

> display: prod  
> 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  
> 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  
> 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  
> 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

> display: prod  
> 53 74 75 64 79 69 6e 67 20 4f 70 65 72 61 74 69  
> 6e 67 20 53 79 73 74 65 6d 73 20 49 73 20 46 75  
> 6e 21 0a 00 00 00 00 00 00 00 00 00 00 00 00 00  
> 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

**Again run

> ./prod

It would display:

> display: prod  
> 53 74 75 64 79 69 6e 67 20 4f 70 65 72 61 74 69  
> 6e 67 20 53 79 73 74 65 6d 73 20 49 73 20 46 75  
> 6e 21 0a 00 00 00 00 00 00 00 00 00 00 00 00 00  
> 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

> display: prod  
> 53 74 75 64 79 69 6e 67 20 4f 70 65 72 61 74 69  
> 6e 67 20 53 79 73 74 65 6d 73 20 49 73 20 46 75  
> 6e 21 0a 00 00 00 00 00 00 00 00 00 00 00 00 00  
> 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

Then run

> **./cons**  

It would display:

> display: cons  
> 53 74 75 64 79 69 6e 67 20 4f 70 65 72 61 74 69  
> 6e 67 20 53 79 73 74 65 6d 73 20 49 73 20 46 75  
> 6e 21 0a 00 00 00 00 00 00 00 00 00 00 00 00 00  
> 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

Studying Operating Systems Is Fun!

Again run:

> **cons**  

This would return:

> cons: Shared memory failed: No such file or directory

This is because the consumer after reading it deletes the file.

- `shm_open()`, the new shared memory object's size is 0, so `ftruncate()` is used to make it larger. Since the object's size is being increased, `ftruncate()` initializes the new bytes to 0. If the shared memory object already exists, for example after running `prod` twice with no intervening `cons`, this example of `ftruncate()` leaves the file size and the object's contents unchanged.

- Linux actually creates an entry in the file system under `/dev/shm` . Here's an example, after running `prod` but before running `cons`.

- While running the commands after % symbol may results in the output shown on next line:

  % ls -l /dev/shm/shm-example

  > ls: cannot access /dev/shm/shm-example: No such file or di rectory

  Run:

  % ./proc

  > display: prod
  > 00 0 0 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  > 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  > 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  > 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

  > display: prod
  > 53 74 75 64 79 69 6e 67 20 4f 70 65 72 61 74 69
  > 6e 67 20 53 79 73 74 65 6d 73 20 49 73 20 46 75
  > 6e 21 0a 00 00 00 00 00 00 00 00 00 00 00 00 00
  > 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

  

  % ls -l /dev/shm/shm-example

  > -rw------- 1 dheller fcse 4096 Jan 31 15:29 /dev/shm/shm-e xample

  % cat /dev/shm/shm-example

  > Studying Operating Systems Is Fun!

  Run: 

  % ./cons

  > display: cons
  > 53 7 4 75 64 79 69 6e 67 20 4f 70 65 72 61 74 69
  > 6e 67 20 53 79 73 74 65 6d 73 20 49 73 20 46 75
  > 6e 21 0a 00 00 00 00 00 00 00 00 00 00 00 00 00
  > 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

  >  Studying Operating Systems Is Fun!

  % ls -l /dev/shm/shm-example

  > ls: cannot access /dev/shm/shm-example: No such file or di rectory



### Read Sockets communication for Client - Server model

Use the following commands to run the server

```
javac DateServer.java && java DateServer

```

To see server is running, open another terminal

```
$ netstat -an | grep 6013
tcp46      0      0  *.6013 
```

Then run 

```
 javac DateClient.java && java DateClient 127.0.0.1
```

