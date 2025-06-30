# Notes for the toralizer project 
-----------------------------------------------------------
# Refer:- Dr Jonas Brich
----------------------------------------------------------------------------------------


# The structure of the sockaddr_in is like this , It is used because the connect() and bind() usually take a structure that contains all this info 

int connect(int socket, const struct sockaddr *address, socklen_t address_len);

struct sockaddr_in {
    short            sin_family;   // Address family (AF_INET)
    unsigned short   sin_port;     // Port number (in network byte order)
    struct in_addr   sin_addr;     // IP address (IPv4)
};

struct in_addr {
    uint32_t s_addr;               // IPv4 address (32-bit integer)
};


The problem is the second parameter:-
1. What does the second parameter points to 
2. is it a pointer of a address?
3. Which address do we need to pass here

So the const 'struct sockaddr*' is the type of the pointer which is a structure and the address means that the &structure_objects... Hence the real argument may look like 

             struct sockaddr_in sock;
            (struct sockaddr*)&sock

# Why is the sockadd_in not written but sockaddr is written.. The reason is the sockadd is the generic
structure name while :-
sockaddr_in is used for ipv4 
sockaddr_in6 is used for ipv6
sockaddr_un is used for unix domain socket

but the connect function is used for all the struct types so we just use the sockaddr rather than the sockaddr_in ..so that the sockaddr_in is typecasted into sockaddr(generic one).. And the generic one is only recognaized by the connect() function .. 
Approach 2: we could have overloaded the function like connect()[with the sockaddr_in] and connect()[with sockaddr] but c does not support the function overloading..
Hence we have to typecast the struct

# How will you take the response and interperate the cd part of it only 
So we would use a buffer string that will read()[which is a sys call] to recieve the respose from the buffer. Now we will access the cd in it.. But the resonse is a string.. so we need to convert the string into struct[predefined].. The buffer would come in the form of a string binary stream .. So we can just explicitly convert the string to struct .. The thing is the struct should be pre-defined 


```c
//Res is a struct :- typedef struct proxy_response Res
Res* res 
res=(Res *)buf
```

# why to use \r\n while createing a htttp request 
line 1-> HTTP method[HEAD PATH protocol version] ("HEAD / HTTP/1.0\r\n")
line 2->HOST INFO["HOST: www.google.com\r\n"]
line 3-> Terminator ["\r\n"]



# Redirecting the library functions,being able to run [toralize curl ip_address port] or [toralize ./forefox ip_address port]




# What is a Shared Library?

A shared library is a collection of compiled functions and routines that can be used by multiple programs at runtime, without being copied into each program individually.
⚙️ Key Characteristics
    . Loaded at Runtime
    Programs link to shared libraries only when they are run, not at compile time.

    Common File Extensions:

        Linux/Unix: .so (shared object) — e.g., libm.so

        Windows: .dll (dynamic-link library)

        macOS: .dylib

    Example:
    Multiple programs can use math functions from a single library like libm.so, instead of having their own copies.

✅ Advantages

    Code Reuse: One library, many programs.

    Reduced Disk and Memory Usage: Shared in memory.

    Easier Updates: Update one library, all dependent programs benefit.

    Modular Design: Promotes separation of concerns.

❌ Disadvantages

    Dependency Issues: If a required library is missing or incompatible, the program may not run.

    Slight Overhead: Runtime linking takes some additional time.

🛠 How It Works

    When a program is compiled, it does not include the shared library's code.

    At runtime, the dynamic linker/loader connects the program to the required library.



# How the shared libraries are creacted in gcc



# 🛠 Shared Library Compilation Process using GCC

These notes explain how to create, compile, and use shared libraries in **C using GCC** on a Unix-like system.

---

## 📁 Components Involved

1. `mathlib.c` – Source code for shared library
2. `mathlib.h` – Header file declaring functions
3. `main.c` – Main program that uses the shared library

---

## 🧱 Step-by-Step Compilation Process

### ✅ Step 1: Compile Source to Object File

```bash
gcc -fPIC -c mathlib.c
```

- `-fPIC`: Generates **Position Independent Code**
  - Required for shared libraries (allows dynamic loading at any memory address).
- `-c`: Compile only, don’t link.
- Output: `mathlib.o`

---

### ✅ Step 2: Create the Shared Library

```bash
gcc -shared -o libmathlib.so mathlib.o
```

- `-shared`: Instructs GCC to produce a **shared object (`.so`)**.
- `-o libmathlib.so`: Names the output shared library.
- Input: `mathlib.o`

✅ Now you have a usable shared library file: `libmathlib.so`.

---

### ✅ Step 3: Compile the Main Program with the Shared Library

```bash
gcc main.c -L. -lmathlib -o main
```

- `-L.`: Look for libraries in the **current directory**.
- `-lmathlib`: Link with `libmathlib.so` (GCC uses `lib<name>.so` pattern).
- `-o main`: Output executable.

🧠 Note: You must include `#include "mathlib.h"` in `main.c` for function declarations.

---

### ✅ Step 4: Run the Executable

Before running:

```bash
export LD_LIBRARY_PATH=.
```

Then run the program:

```bash
./main
```

- `LD_LIBRARY_PATH`: Tells the dynamic linker to search the current directory (`.`) for shared libraries.

---

## ⚠️ Common Error and Fix

**Error:**
```bash
error while loading shared libraries: libmathlib.so: cannot open shared object file
```

**Fix:**
```bash
export LD_LIBRARY_PATH=.
```

---

## 📌 Summary

| Step     | Command                                     | Purpose                              |
|----------|---------------------------------------------|--------------------------------------|
| 1        | `gcc -fPIC -c mathlib.c`                    | Compile source to PIC object file    |
| 2        | `gcc -shared -o libmathlib.so mathlib.o`    | Create shared library `.so` file     |
| 3        | `gcc main.c -L. -lmathlib -o main`          | Link program with shared library     |
| 4        | `export LD_LIBRARY_PATH=.` + `./main`       | Run the executable with `.so` found  |

---

## ✅ Example Output

```
Add: 8
Subtract: 2
```

---

## 🧠 Notes

- Shared libraries reduce memory and disk space usage.
- Must be present at runtime or program will fail to load.
- You can permanently add `.so` paths via `/etc/ld.so.conf` or `ldconfig`.

---





# What are handels and symbols and dlopen() and dlsym()

## symbols: Any functions or variables
## Handels: An void pointer to a library with access permisson.. Like a access token to a library .. But for that you need to open that library using dlopen()... The handel doesn't tell you how the library is built but lets you use it . and create a wrapper on top of that 

## dll:- dynamically linked files. The files/ libraries that can be linked by multiple programs at once..from the memoryThe library that lets u create wrapper is dlfcn.h 

```C

void(*add_functtion)(int , int ){

add_function=dlsym(handel, "add");
}
```

so this dlsym() function provides a pointer to that symbol(function/variable). So what it does is that it looks inside that library using the handle and finds the symbol and provides the address of the symbol..


## The concept of pseudo handles 
The handels are like a token to a particular library while the pseudo-handle are not a reference to any particular library or a actual object.. But it is like a magic number that system knows internally what uyou u talking about.. 

Example:- "Take me to the manager" , There the man is talking about the current manager only, Which is know by everyone in the office and the statement in itself does not point to a particular manager that served there 
### There are two pseudo_handle :-
RTDL:- Run time dynamically linked :- The files that are linked at the runtime dynamically 
##### RTDL_NEXT:- 
The next symbol that was liked dynamically. Not the current one. its like saying there there are 10 burger shopes in a area and you opened a new one there and then u want ot use the burger recipe of someone else so you say that gieve me the location of the next burger shop but not me .. The shops after me
. Use them for making wrappers_function  and u want to use the old_function as a part of it 

##### RTDL_DEFAULT :- 
Tell me what is  the location in all the modules that i have loaded in the file (# include) in any order that is there



# Lets hook the ls command to hide a particular file(secret.txt)

```c
#include <stdlib.h>
#include <dlfnc.h>
#include <string.h>

// We are doing to override the ls function which uses a another function called readdir()..The signature of that function are readdir(DIR* dirp); and the return struct is 

struct dirent {
    ino_t          d_ino;       /* Inode number */
    off_t          d_off;       /* Not an offset; see below */
    unsigned short d_reclen;    /* Length of this record */
    unsigned char  d_type;      /* Type of file; not supported
                                            by all filesystem types */
    char           d_name[256]; /* Null-terminated filename */
}

// This is the return struct 

struct dirent* readdir(DIR* dirp){

struct dirent*(*original_readdir)(DIR*); //Forward declaration of the original_function that we are going to use
struct dirent* ret; // This would contain the return value

original_readdir=dlsym(RTDL_NEXT,"readdir");

// This loop would check wheather the ret is being assinged to a NULL till then assing the ret as the result of the function   
while(ret=original_readdir(dirp)){
if(strstr(ret->dname,"seceret.txt")==0){
            break;
    }

return ret;
}


```
This is called a commad hijacking




