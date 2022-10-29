
//Author :-> Aman Izardar
#include <bits/stdc++.h>
#define ll long long
#define ff first
#define ss second
#define pb push_back
#define mp make_pair
#define phi 3.1415926536
#define mod 1000000007
using namespace std;

#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>

// #define disk_size 500 * 1024 * 1024
// #define block_size 4 * 1024
// #define no_of_blocks 128000 //Disksize/blocksize
// #define no_of_inodes 10000
// #define no_of_fd 108

#define disk_size 500 * 1024 
#define block_size 4 * 1024
#define no_of_blocks 125 //Disksize/blocksize
#define no_of_inodes 25
#define no_of_fd 10

//Code Section : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : :
bool ismounted = false;

vector<int> currently_free_inode;
vector<int> currently_free_datablock;
map<string, int> file_and_inode;
map<int, string> inode_and_file;
vector<int> availabe_filedescriptor;
map<int, pair<int, int>> fd_map; //fd,inode,filepointer
map<int, int> fd_mode_map;       //fd,fdmode
map<string, int> file_to_fd;     //filename to fd map
int total_no_of_currently_opened_files = 0;
int fd;

ofstream fout;

struct inode //Inode size is ....
{
public:
    int filesize;
    int pointer[10];
};

inode array_of_inodes[no_of_inodes];

struct file_to_inode
{
    char filename[40];
    
    // string filename;
    int inode_no;
};
file_to_inode array_of_file_to_inode[no_of_inodes];

struct super_block
{
    // int size_of_sb=
    // int no_of_datablocks;
    // public:
    int total_blocks_by_super_block = ceil(sizeof(super_block) / (block_size));
    int total_blocks_by_file_to_inodes = ceil((sizeof(struct file_to_inode) * no_of_inodes) / (block_size));
    int total_blocks_by_inodes = ceil(sizeof(inode) * no_of_inodes / (block_size));
    int begin_of_inodes = total_blocks_by_super_block + total_blocks_by_file_to_inodes;
    int begin_of_data_blocks = total_blocks_by_super_block + total_blocks_by_file_to_inodes + total_blocks_by_inodes;

    int freeblocks = no_of_blocks - begin_of_data_blocks;

    bool is_inode_free[no_of_inodes];
    bool is_disk_block_free[no_of_blocks];

    // // Debug
    // char temp[12];
    // // end of Debug
};

struct super_block curr_superblock;

void create_disk(string newdisk)
{
    if ((access(newdisk.c_str(), 0)) != -1)
    {
        cout << "This Disk is already Present.Please Try Again.\n";
        return;
    }

    vector<char> empty(4 * 1024, 0);

    fout.open(newdisk.c_str(), ios::binary | ios::out);

    cout << no_of_blocks << endl;
    for (int i = 0; i < no_of_blocks; i++)
    {
        fout.write(&empty[0], empty.size());
    }

    for (int i = 0; i < no_of_inodes; ++i)
    {
        for (int j = 0; j < 10; j++)
        {
            array_of_inodes[i].pointer[j] = -1;
        }
    }

    // debug
    fout.close();
    // End of Debug

    super_block mysuper_block;

    cout<<"begin "<<mysuper_block.begin_of_data_blocks<<endl;
    cout<<"total block by super block "<<mysuper_block.total_blocks_by_super_block<<endl;
    cout<<"total block by file to inode "<<mysuper_block.total_blocks_by_file_to_inodes<<endl;
    cout<<"total block by inod "<<mysuper_block.total_blocks_by_inodes<<endl;
    cout<<"begin of inode "<<mysuper_block.begin_of_inodes<<endl;
    cout<<"begin of data blocks "<<mysuper_block.begin_of_data_blocks<<endl;
    cout<<"free blocks "<<mysuper_block.freeblocks<<endl;
    cout<<"file to inode map struct size"<<sizeof(file_to_inode)<<endl;

    for (int i = 0; i < mysuper_block.begin_of_data_blocks; i++)
    {
        mysuper_block.is_disk_block_free[i] = false; //false means occupied
    }
    for (int i = mysuper_block.begin_of_data_blocks; i < no_of_blocks; i++)
    {
        mysuper_block.is_disk_block_free[i] = true;
    }
    for (int i = 0; i < no_of_inodes; i++)
    {
        mysuper_block.is_inode_free[i] = true;
    }

    // // Debug
    // string name="aman";
    // strcpy(mysuper_block.temp,name.c_str());

    // end of Debug
    //superblock initialized

    //now we need to store it in the starting of the filesystem

    // Debug

    for(int i=0;i<no_of_inodes;i++)
    {
        strcpy(array_of_file_to_inode[i].filename,"qq");
    }

    int fd = open(newdisk.c_str(), 01);
    // write(fd,&mysuper_block,sizeof(mysuper_block));

    write(fd, &mysuper_block, sizeof(mysuper_block));

    // //Now lets store filetoinode mapping
    lseek(fd, mysuper_block.total_blocks_by_super_block * block_size, SEEK_SET);
    write(fd, array_of_file_to_inode, sizeof(array_of_file_to_inode));

    lseek(fd, mysuper_block.begin_of_inodes * block_size, SEEK_SET);
    write(fd, array_of_inodes, sizeof(array_of_inodes));

    close(fd);

    // End of Debug

    // int size_of_super_block = sizeof(mysuper_block);
    // // cout<<"size of Super Block "<<size_of_super_block<<endl;
    // fout.seekp(0,fout.beg);
    // char super_block_data[size_of_super_block];
    // memcpy(super_block_data,&mysuper_block,size_of_super_block);
    // fout<<super_block_data;

    // //Now lets store filetoinode mapping

    // fout.seekp(mysuper_block.total_blocks_by_super_block*block_size,fout.beg);
    // int size_of_file_to_inode_array=sizeof(array_of_file_to_inode);
    // char file_to_array_buffer[size_of_file_to_inode_array];
    // memcpy(file_to_array_buffer,&array_of_file_to_inode,size_of_file_to_inode_array);
    // fout<<file_to_array_buffer;

    // // Now lets store inodes

    // fout.seekp(mysuper_block.begin_of_inodes*block_size,fout.beg);
    // int size_of_inode_array=sizeof(array_of_inodes);
    // char inode_array_buffer[size_of_inode_array];
    // memcpy(inode_array_buffer,&array_of_inodes,size_of_inode_array);
    // fout<<inode_array_buffer;

    // fout.close();

    cout << "Disk Created Successfully\n";
}

bool mount_disk(string old_disk)
{

    // Debug

    // FILE* diskptr = fopen(old_disk.c_str(), "rb+");
    // if (diskptr == NULL)
    // {
    //     cout << string(RED) << "Disk does not exist :(" << string(DEFAULT) << endl;
    //     return 0;
    // }
    // char sb_buff[sizeof(curr_superblock)];
    // memset(sb_buff, 0, sizeof(curr_superblock));
    // fread(sb_buff, sizeof(char), sizeof(curr_superblock), diskptr);
    // memcpy(&curr_superblock, sb_buff, sizeof(curr_superblock));
    // for (int i = no_of_inodes - 1; i >= 0; --i)
    //     if (curr_superblock.is_inode_free[i] == true)
    //     {
    //         cout<<"Occupied";
    //         // file_to_inode_map[string(file_inode_mapping_arr[i].file_name)] = file_inode_mapping_arr[i].inode_num;
    //         // inode_to_file_map[file_inode_mapping_arr[i].inode_num] = string(file_inode_mapping_arr[i].file_name);
    //     }
    //     else
    //     {
    //         // free_inode_vector.push_back(i);
    //         cout<<"Free Inode\n";
    //     }
    //     return false;

    // End of Debug

    // ifstream fin(old_disk,ios::in|ios::binary);
    fd = open(old_disk.c_str(), 02);

    if (fd == -1)
    {
        cout << "Disk Does Not Exists. Please Try Again Later\n";
        return false;
    }



    read(fd, &curr_superblock, sizeof(curr_superblock));

    //restoring file_to_inode mapping

    lseek(fd, curr_superblock.total_blocks_by_super_block * (block_size), SEEK_SET);
    read(fd, &array_of_file_to_inode, sizeof(array_of_file_to_inode));

    // // Now lets restore inodes
    lseek(fd, curr_superblock.begin_of_inodes * block_size, SEEK_SET);
    read(fd, &array_of_inodes, sizeof(array_of_inodes));

    // if(fin.fail())
    // {
    //     cout<<"Disk Does Not Exists. Please Try Again Later\n";
    //     return false;
    // }

    // Storing Super block
    // int size_of_super_block=sizeof(curr_superblock);
    // cout<<"size of Super Block "<<size_of_super_block<<endl;
    // cout<<"total block by super block "<<curr_superblock.total_blocks_by_super_block<<endl;
    // char super_block_data[size_of_super_block];
    // fin.read(&curr_superblock,size_of_super_block);

    // printf("data written is %s ",curr_superblock.temp);
    // fin.read(super_block_data,size_of_super_block);
    // memcpy(&curr_superblock,super_block_data,size_of_super_block);

    //restoring file_to_inode mapping

    // fin.seekg(curr_superblock.total_blocks_by_super_block*(block_size),fin.beg);
    // int size_of_file_to_inode_array=sizeof(array_of_file_to_inode);
    // char file_to_array_buffer[size_of_file_to_inode_array];

    // fin.read(file_to_array_buffer,size_of_file_to_inode_array);
    // memcpy(array_of_file_to_inode,file_to_array_buffer,size_of_file_to_inode_array);

    // // Now lets restore inodes

    // fin.seekg(curr_superblock.begin_of_inodes*block_size,fin.beg);
    // int size_of_inode_array=sizeof(array_of_inodes);
    // char inode_array_buffer[size_of_inode_array];
    // fin.read(inode_array_buffer,size_of_inode_array);
    // memcpy(array_of_inodes,inode_array_buffer,size_of_inode_array);



    for (int i = 0; i < no_of_inodes; i++)
    {
        if (curr_superblock.is_inode_free[i])
        {
            // cout<<"This is a free inode\n";
            currently_free_inode.push_back(i);
        }
        else
        {
            // cout<<"hi";
            inode_and_file[array_of_file_to_inode->inode_no] = string(array_of_file_to_inode->filename);
            file_and_inode[string(array_of_file_to_inode->filename)] = array_of_file_to_inode->inode_no;
        }
    }

    for (int i = curr_superblock.begin_of_data_blocks; i < no_of_blocks; i++)
    {
        if (curr_superblock.is_disk_block_free[i])
        {
            // cout<<"This is a free data block\n";
            currently_free_datablock.push_back(i);
        }
    }

    for(int i=0;i<no_of_fd;i++)
    {
        availabe_filedescriptor.push_back(i);
    }

    for(int i=0;i<no_of_inodes;i++)
    {
        if(strcmp(array_of_file_to_inode[i].filename,"qq")!=0 and strlen(array_of_file_to_inode[i].filename)!=0)
        {
            // cout<<"File name is "<<array_of_file_to_inode[i].filename<<endl;
            file_and_inode[string(array_of_file_to_inode[i].filename)]=array_of_file_to_inode[i].inode_no;
            inode_and_file[array_of_file_to_inode[i].inode_no]=string(array_of_file_to_inode[i].filename);
        }
    }





    ismounted = 1;
    cout << "Disk Mounted Successfully" << endl;

    return true;
}

void create_file()
{
    cout << "Enter filename to be created \n";
    string filename;
    cin >> filename;

    if (file_and_inode.find(filename) != file_and_inode.end())
    {
        cout << "Error : File is already Present \n";
        return;
    }

    if (currently_free_datablock.size() == 0)
    {
        cout << "Error : No More Free DataBlocks\n";
        return;
    }

    if (currently_free_inode.size() == 0)
    {
        cout << "Error : No More Free inodes";
    }

    int this_file_inode = currently_free_inode.back();
    currently_free_inode.pop_back();
    int this_file_datablock = currently_free_datablock.back();
    currently_free_datablock.pop_back();

    array_of_inodes[this_file_inode].pointer[0] = this_file_datablock;
    array_of_inodes[this_file_inode].filesize = 0;

    file_and_inode[filename] = this_file_inode;
    inode_and_file[this_file_inode] = filename;

    // cout<<"This file inode is "<<this_file_inode<<endl;
    // cout<<"This file filename is "<<filename<<endl;
    // cout<<"inode and file map value "<<inode_and_file[this_file_inode]<<endl;



    array_of_file_to_inode[this_file_inode].inode_no = this_file_inode;
    strcpy(array_of_file_to_inode[this_file_inode].filename, filename.c_str());


    cout << "File Created Successfully\n";

    return;
}

void open_file()
{
    cout << "Please Enter the Name of the File : \n";
    string filename;
    cin >> filename;

    if (file_and_inode.find(filename) == file_and_inode.end())
    {
        cout << "NO file named " << filename << " Exists.\n";
    }

    if (availabe_filedescriptor.size() == 0)
    {
        cout << "NO FD Available \n";
    }
    int mode;
    while (1)
    {
        cout << "Enter The Mode \n";
        cout << "1 : Read Mode \n";
        cout << "2 : Write Mode \n";
        cout << "3 : Append Mode \n";

        cin >> mode;

        if (mode == 1)
        {
            break;
        }
        else if (mode == 2)
        {
            break;
        }
        else if (mode == 3)
        {
            break;
        }
        else
        {
            cout << "Please Enter a Valid Mode\n";
        }
    }

    int curr_inode = file_and_inode[filename];
    // cout<<"inode of that file is "<<curr_inode<<endl;

    if (mode == 2 or mode == 3)
    {
        for (int i = 0; i < no_of_fd; i++)
        {
            if (fd_map.find(i) != fd_map.end() and fd_map[i].first == curr_inode and (fd_mode_map[i] == 1 or fd_mode_map[i] == 2))
            {
                cout << "You Opened That file already in Write or append mode \n";
            }
        }
    }

    int this_file_fd = availabe_filedescriptor.back();
    cout << "File discriptor assigned is " << this_file_fd << endl;

    fd_map[this_file_fd].second = 0;
    fd_map[this_file_fd].first = curr_inode;

    fd_mode_map[this_file_fd] = mode;

    total_no_of_currently_opened_files++;

    file_to_fd[filename] = this_file_fd;

    cout << "File " << filename << " Opened Successfully \n";
}

void read_file()
{
    cout << "Enter the Name of The File you want to Read \n";
    string Filename;
    cin >> Filename;

    if (file_and_inode.find(Filename) == file_and_inode.end())
    {
        cout << "Error : That file is not present . please Try Again Later \n";
        return;
    }

    if (file_to_fd.find(Filename) == file_to_fd.end())
    {
        cout << "Error : File Is not Opened Yet \n";
        return;
    }

    int that_file_fd = file_to_fd[Filename];

    if (fd_mode_map[that_file_fd] != 1)
    {
        cout << "Error : File Is not Opened In Read Mode \n";
        return;
    }

    int that_file_inode = file_and_inode[Filename];

    int fs = fd_map[that_file_fd].second;

    struct inode that_inode_str = array_of_inodes[that_file_inode];

    int that_file_filesize = that_inode_str.filesize;

    int that_file_total_blocks = that_file_filesize / block_size;

    int remaining_blocks_to_read = that_file_total_blocks;

    char that_file_buffer[that_file_filesize];

    char temp_buffer[block_size];
    int totalread = 0;

    for (int i = 0; i < 10; i++)
    {
        int block_to_read = that_inode_str.pointer[i];
        if (block_to_read == -1)
            break;

        if (lseek(fd, block_to_read * block_size, SEEK_SET) < 0)
        {
            perror("block_read: failed to lseek");
            return;
        }

        int n;
        if ((n = read(fd, temp_buffer, sizeof(temp_buffer))) < 0)
        {
            perror("block_read: failed to read");
            return;
        }
        strcat(that_file_buffer,temp_buffer);
        totalread += n;
    }
    cout << "File Data Is :\n";
    cout << that_file_buffer;
    cout<<"\n";
}

int block_write(int block, char *buf, int size, int start_position)
{

    if ((block < 0) || (block >= no_of_blocks))
    {
        fprintf(stderr, "block_write: block index out of bounds\n");
        return -1;
    }

    if (lseek(fd, (block * block_size) + start_position, SEEK_SET) < 0)
    {
        perror("block_write: failed to lseek");
        return -1;
    }

    // write(fd,buf,sizeof(buf));
    if (write(fd,buf,sizeof(buf)) < 0)
    {
        perror("block_write: failed to write");
        return -1;
    }

    return 0;
}

int _write_into_file(int fd, char *buff, int len, int *bytes_written)
{
    int cur_pos = fd_map[fd].second;
    int filled_data_block = cur_pos / block_size;
    int file_inode = fd_map[fd].first;

    /* if last Data block is partially filled */
    if (cur_pos % block_size != 0)
    {
        /* Write file into direct pointed block */
        if (filled_data_block < 10)
        {
            int data_block_to_write = array_of_inodes[file_inode].pointer[filled_data_block];
            block_write(data_block_to_write, buff, len, cur_pos % block_size);
            array_of_inodes[file_inode].filesize += len;
            fd_map[fd].second += len; //updating the cur pos in the file
            *bytes_written += len;
        }
    }
    else
    {
        /* if filesize = 0 means file is empty then start writting into file from first direct block. */
        if (array_of_inodes[fd_map[fd].first].filesize == 0)
        {
            block_write(array_of_inodes[file_inode].pointer[0], buff, len, 0);
            array_of_inodes[file_inode].filesize += len;
            fd_map[fd].second += len;
            *bytes_written += len;
        }
        /* if filesize != 0 then */
        else
        {
            if (cur_pos == 0)
            {
                /* flush all data blocks and start writting into file from first block */
                if (fd_map[fd].second == 0)
                {
                    for (int i = 0; i < 10; i++)
                    {
                        if (array_of_inodes[file_inode].pointer[i] == -1)
                        {
                            break;
                        }
                        else
                        {
                            currently_free_datablock.push_back(array_of_inodes[file_inode].pointer[i]);
                            array_of_inodes[file_inode].pointer[i] = -1;
                        }
                    }
                    //check if datablock are available
                    if (currently_free_datablock.size() == 0)
                    {
                        cout  << "Write File Error : No more DataBlock available"  << endl;
                        return -1;
                    }
                    int next_avl_datablock = currently_free_datablock.back();
                    currently_free_datablock.pop_back();
                    array_of_inodes[file_inode].pointer[0] = next_avl_datablock;
                }
                block_write(array_of_inodes[file_inode].pointer[0], buff, len, 0);
                array_of_inodes[file_inode].filesize += len;
                fd_map[fd].second += len;
                *bytes_written += len;
            }
            else
            {
                if (filled_data_block < 10)
                {
                    //check if datablock are available
                    if (currently_free_datablock.size() == 0)
                    {
                        cout  << "Write File Error : No more DataBlock available" << endl;
                        return -1;
                    }
                    int data_block_to_write = currently_free_datablock.back();
                    currently_free_datablock.pop_back();
                    array_of_inodes[file_inode].pointer[filled_data_block] = data_block_to_write;

                    block_write(data_block_to_write, buff, len, cur_pos % block_size);
                    array_of_inodes[file_inode].filesize += len;
                    fd_map[fd].second += len; //updating the cur pos in the file
                    *bytes_written += len;
                }
            }
        }
    }
    return 0;
}

void write_file()
{
    cout << "Enter the Name of The File : \n";
    string Filename;
    cin >> Filename;

    if (file_and_inode.find(Filename) == file_and_inode.end())
    {
        cout << "Error : That file is not present . please Try Again Later \n";
        return;
    }

    if (file_to_fd.find(Filename) == file_to_fd.end())
    {
        cout << "Error : File Is not Opened Yet \n";
        return;
    }

    int that_file_fd = file_to_fd[Filename];

    if (fd_mode_map[that_file_fd] != 2)
    {
        cout << "Error : File Is not Opened In Write Mode \n";
        return;
    }

    int that_file_inode = file_and_inode[Filename];

    for (int i = 0; i < no_of_fd; i++)
    {
        if (fd_map.find(i) != fd_map.end() &&
            fd_map[i].first == that_file_inode &&
            fd_mode_map[i] == 0)
        {
            fd_map[i].second = 0;
        }
    }

    cout << "Enter file content : " << endl;
    string s;
    // do
    // {
    //     string tmp_str;
    //     getline(cin, tmp_str);
    //     s += (tmp_str + "\n");
    // } while (!cin.eof());
    // s.pop_back();
    // s.pop_back();

    // Debug 
    string ss;
    getline(cin,s);
    getline(cin,ss);

    s+=ss;

    s.pop_back();
    s.pop_back();

    // End of Debug


    cout<<"Eof Reached \n";
    // return;

    s = s.substr(1);

    int byte_w = 0;
    unsigned int remaining_size_in_last_written_data_block = block_size - ((fd_map[that_file_fd].second) % (block_size));
    int len;
    if (remaining_size_in_last_written_data_block >= s.size())
    {
        len = s.size();
        char buff[len + 1];
        memcpy(buff, s.c_str(), len);
        buff[len] = '\0';
        _write_into_file(that_file_fd, buff, len, &byte_w);
    }
    else
    {
        //1st write remaining size in last written data block
        len = remaining_size_in_last_written_data_block;
        char buff[len + 1];
        memcpy(buff, s.c_str(), len);
        buff[len] = '\0';
        _write_into_file(fd, buff, len, &byte_w);
        s = s.substr(len);

        //write block by block till last block
        int remaining_block_count = s.size() / block_size;
        while (remaining_block_count--)
        {
            len = block_size;
            char buff[len + 1];
            memset(buff, 0, len);
            memcpy(buff, s.c_str(), len);
            buff[len] = '\0';
            s = s.substr(len);
            if (_write_into_file(fd, buff, len, &byte_w) == -1)
            {
                cout << "No Enough space. Only " << byte_w << " bytes written." << endl;
                return;
            }
        }

        //write last partial block
        int remaining_size = s.size() % block_size;
        len = remaining_size;
        char buff1[len + 1];
        memset(buff1, 0, len);
        memcpy(buff1, s.c_str(), len);
        buff1[len] = '\0';
        if (_write_into_file(fd, buff1, len, &byte_w) == -1)
        {
            cout << "No Enough space. Only " << byte_w << " bytes written." << endl;
            return;
        }
    }
    cout << byte_w << " bytes written. \nFile Written Successfully." << endl;
    return;
}

void append_file()
{
}

void close_file()
{
    cout << "Enter The name of the File to close\n";
    string filename;
    cin >> filename;

    if (file_to_fd.find(filename) == file_to_fd.end())
    {
        cout << "Error : Please Open that file First \n";
        return;
    }
    total_no_of_currently_opened_files--;
    int this_file_fd = file_to_fd[filename];
    fd_map.erase(this_file_fd);
    fd_mode_map.erase(this_file_fd);
    availabe_filedescriptor.push_back(this_file_fd);

    cout << "File Closed Successfully \n";
}

void delete_file()
{
    cout << "Enter Filename to be Deleted \n";
    string filename;
    cin >> filename;

    if (file_and_inode.find(filename) == file_and_inode.end())
    {
        cout << "File " << filename << " Is Not Present \n";
        return;
    }
    int that_file_inode = file_and_inode[filename];
    file_and_inode.erase(filename);
    inode_and_file.erase(that_file_inode);
    currently_free_inode.push_back(that_file_inode);

    for (int i = 0; i < 10; i++)
    {
        if (array_of_inodes[that_file_inode].pointer[i] == -1)
        {
            break;
        }
        else
        {
            currently_free_datablock.push_back(array_of_inodes[that_file_inode].pointer[i]);
            array_of_inodes[that_file_inode].pointer[i] = -1;
        }
    }

    array_of_file_to_inode[that_file_inode].inode_no = -1;
    strcpy(array_of_file_to_inode[that_file_inode].filename, "");

    cout << "File Deleted Successfully\n";
}

void list_of_files()
{

    if(file_and_inode.size()==0)
    {
        cout<<"\nThis Filesystem currently has no files, Please Create Some\n";
        return;
    }

    cout << "This is the List of all Files : \n\n";
    for (auto i : file_and_inode)
    {
        cout << i.first << "\n";
    }
    cout << "\n\n";
    return;
}

void list_of_opened_files()
{

    if(fd_map.size()==0)
    {
        cout<<"\nNo Open Files.\n\n";
        return;
    }

    cout << "List of Opened Files are : \n";
    for (auto i : fd_map)
    {
        int this_fd = i.first;
        int this_inode = i.second.first;
        // cout<<"this fd is "<<this_fd<<endl;
        // cout<<"this inode is "<<this_inode<<endl;
        cout << "File name : " << inode_and_file[this_inode] << " is Opened \n";
    }
    return;
}

void unmount()
{
    if (!ismounted)
    {
        cout << "Please Mount a disk First Before Unmounting \n";
        return;
    }

    for (int i = 0; i < no_of_inodes; i++)
    {
        curr_superblock.is_inode_free[i] = false;
    }
    for (int i = 0; i < currently_free_inode.size(); i++)
    {
        curr_superblock.is_inode_free[currently_free_inode[i]] = true;
    }

    for (int i = curr_superblock.begin_of_data_blocks; i < no_of_blocks; i++)
    {
        curr_superblock.is_disk_block_free[i] = false;
    }
    for (int i = 0; i < currently_free_datablock.size(); i++)
    {
        curr_superblock.is_disk_block_free[currently_free_datablock[i]] = true;
    }

    currently_free_inode.clear();
    currently_free_datablock.clear();

    //  //now we need to store it in the starting of the filesystem

    lseek(fd, 0, SEEK_SET);
    write(fd, &curr_superblock, sizeof(curr_superblock));

    // //Now lets store filetoinode mapping
    lseek(fd, curr_superblock.total_blocks_by_super_block * block_size, SEEK_SET);
    write(fd, &array_of_file_to_inode, sizeof(array_of_file_to_inode));

    // // Now lets store inodes
    lseek(fd, curr_superblock.begin_of_inodes * block_size, SEEK_SET);
    write(fd, &array_of_inodes, sizeof(array_of_inodes));

    close(fd);

    //  //now we need to store it in the starting of the filesystem

    // int size_of_super_block = sizeof(curr_superblock);
    // fout.seekp(0,fout.beg);
    // char super_block_data[size_of_super_block];
    // memcpy(super_block_data,&curr_superblock,size_of_super_block);
    // fout<<super_block_data;

    // //Now lets store filetoinode mapping

    // fout.seekp(curr_superblock.total_blocks_by_super_block*block_size,fout.beg);
    // int size_of_file_to_inode_array=sizeof(array_of_file_to_inode);
    // char file_to_array_buffer[size_of_file_to_inode_array];
    // memcpy(file_to_array_buffer,&array_of_file_to_inode,size_of_file_to_inode_array);
    // fout<<file_to_array_buffer;

    // // Now lets store inodes

    // fout.seekp(curr_superblock.begin_of_inodes*block_size,fout.beg);
    // int size_of_inode_array=sizeof(array_of_inodes);
    // char inode_array_buffer[size_of_inode_array];
    // memcpy(inode_array_buffer,&array_of_inodes,size_of_inode_array);
    // fout<<inode_array_buffer;


     //fd,fdmode
   //filename to fd map

    file_and_inode.clear();
    inode_and_file.clear();
    fd_map.clear();
    fd_mode_map.clear();
    file_to_fd.clear();
    total_no_of_currently_opened_files=0;

    cout << "Unmount Successful.\n";
    ismounted = false;

    return;
}

void section_2()
{
    while (1)
    {
        
        cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
        cout << "1  : Create File \n";
        cout << "2  : Open File \n";
        cout << "3  : Read File \n";
        cout << "4  : Write File \n";
        cout << "5  : Append File \n";
        cout << "6  : Close File \n";
        cout << "7  : Delete File \n";
        cout << "8  : List of Files \n";
        cout << "9  : List of opened Files \n";
        cout << "10 : Unmount \n";
        cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n\n$";

        int op;
        cin >> op;

        if (op == 1)
            create_file();

        else if (op == 2)
            open_file();

        else if (op == 3)
            read_file();

        else if (op == 4)
            write_file();

        else if (op == 5)
            append_file();

        else if (op == 6)
            close_file();

        else if (op == 7)
            delete_file();

        else if (op == 8)
            list_of_files();

        else if (op == 9)
            list_of_opened_files();

        else if (op == 10)
        {
            unmount();
            break;
        }

        else
        {
            cout << "Please Enter a Valid Command\n";
            break;
        }
    }
    return;
}

void solve()
{

    int op;
    while (1)
    {
        cout << "1 : create disk" << endl;
        cout << "2 : mount disk" << endl;
        cout << "4 : exit" << endl;

        cin >> op;
        if (op == 1)
        {
            cout << "Please Enter a Disk Name:\n";
            string newdisk;
            cin >> newdisk;
            create_disk(newdisk);
            // break;
        }
        else if (op == 2)
        {
            cout << "Enter Diskname to mount: \n";
            string olddisk;
            cin >> olddisk;
            if (mount_disk(olddisk))
            {
                section_2();
            }
            // break;
        }
        else if (op == 4)
        {
            cout << "\nSayonara Bye!\n";
            break;
        }
        else
        {
            cout << "Please Enter a Valid choice\n";
        }
    }
}

int main()
{

    // ios_base::sync_with_stdio(false);
    // cin.tie(NULL);
    // #ifndef ONLINE_JUDGE
    //     freopen("input.txt", "r", stdin);
    //     freopen("output.txt", "w", stdout);
    // #endif
    ll t = 1;
    //cin>>t;
    while (t--)
    {
        solve();
    }
    return 0;
}
