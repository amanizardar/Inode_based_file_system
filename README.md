# Inode based file system
Build an Inode-based file system with limited functionality. It is a virtual file system which runs on top of the Linux file system.
This file system is a simplified version of a typical UNIX file system and thus
serves to introduce some of the basic on-disk structures, access methods, and
various policies that we find in many file systems today.  
User can perform all the operations which are basically supported by Linux file system.

Prerequisites
- The size of virtual disk is 512 MB.
- code is written with respect to linux file system and path hierarchy.

You need to install GNU g++ compiler.

    sudo apt-get install g++
    
This inode based file system implemented in two sections :

## SECTION 1: 
### This is the region outside the disks. From here you will create a disk and then mount/open it to perform all basic operations mentioned in section 2.  

- create disk : Creates an empty disk of size 500Mb.  
While creating an empty disk a unique name will be given to it which will be used
to mount it.  

- mount disk: Opens the specified disk for various file operations. As mentioned
in section 2.  

- exit: Close the application.  

## SECTION 2 : 
### This is the region inside the disk. You may have multiple disks. You will open only 1 disk at a time. After opening/mounting a particular disk, you will perform below mentioned operations in the disk:  

- create file: creates an empty text file.  
- open file: opens a particular file in read/write/append mode as specified in input,
multiple files can be opened simultaneously.  
- read file: Displays the content of the file.  
- write file: Write fresh data to file(override previous data in file).  
- append file: Append new data to an existing file data.  
- close file: Closes the file.  
- delete file: Deletes the file.  
- list of files: List all files present in the current disk.  
- list of opened files: List all opened files and specify the mode they are open in.  
- unmount: Closes the currently mounted disk.  


## WORKING  
### Section 1:  Disk Menu
    1: create disk  
    2: mount disk  
    3: exit  
    
   > On press 1: 
      
      Take unique disk name as input on next line.  
   > On press 2: 
    
    Open the disk for mounting purposes(display options of section2 for
    that disk).  
   >On press 3:
    
    Exit the application.  
    
    
    
### Section 2 : Mounting Menu  
    1: create file  
    2: open file  
    3: read file  
    4: write file  
    5: append file  
    6: close file  
    7: delete file  
    8: list of files  
    9: list of opened files  
    10: unmount  
    
   > On press 1: 
    
    Take unique file name as input on next line.  
    
   > On press 2: 
   
    Take file name as input on next line.  
    Then take file mode as input as mentioned below on next line:  
    0: read mode  
    1: write mode  
    2: append mode    
    After specifying the mode, display the file descriptor allocated to the opened
    file along with the mode in which the file is opened.  
    
   >On press 3: 
    
    Take input file descriptor of the file which you want to read.  
    File descriptor has been obtained in the open file command.  
   
   >On press 4: 
   
    Take input file descriptor of the file which you want to write.  
    File descriptor has been obtained in the open file command.  
    Enter file content that you want to write in the file.  
    
   >On press 5: 
   
    Take input file descriptor of the file to which you want to append
    further text.  
    File descriptor has been obtained in the open file command.  
    Enter the file content that you want to append to the file.    
    
   >On press 6: 
    
    Take the input file descriptor of the file to which you want to close.  
    
   >On press 7: 
    
    Take the input file name which you want to delete.  
    
   >On press 8: 
    
    List all existing files on the disk.  
    
   >On press 9: 
    
    List all existing files which are currently open along with their file
    descriptors and mode in which they are open.  
    
   >On press 10: 
    
    Unmount/close the disk which is current mount(in which you are
    working currently) and return to the previous menu.   


