#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>


// Function to check if the line already exist
int lineExist(char *filename, char line[])
{
    int fd = open(filename, O_RDWR);
    if (fd == -1)
    {
        perror("Error");
    }
    
    char fline[1000] = {0};
    char c;
    int i=0;
    while(read(fd, &c, 1) > 0)
    {
        if (c == '\n')
        {
            if(strcmp(line, fline) == 0)
            {
                close(fd);
                return 1;
            }
            memset(&fline, 0, sizeof(fline));
            i = 0;
        }
        else
        {
            fline[i] = c;
            i++;
        }
    }

    close(fd);
    return 0;
}


int main(int argc, char *argv[])
{
    // If the input filename is passes as an argument
    if (argc == 2)
    {
        char *temp_file_name = "_temp.txt";
        int fd0 = open(temp_file_name, O_CREAT | O_RDWR, S_IRWXU);
        int fd = open(argv[1], O_RDWR);
        if (fd == -1 | fd0 == -1)
        {
            perror("Error");
            return 0;
        }
        char line[1000] = {0};
        char c;
        int i=0;
        close(fd0);
        while(read(fd, &c, 1) > 0)
        {
            if (c == '\n')
            {
                int flag = lineExist(temp_file_name, line);
                if (!flag)
                {
                    int fd0 = open(temp_file_name, O_RDWR | O_APPEND);
                    if (fd0 == -1)
                    {
                        perror("Error");
                    }
                    line[i] = c;
                    int size = write(fd0, &line, strlen(line));
                    close(fd0);
                }
                memset(&line, 0, sizeof(line));
                i = 0;
            }
            else
            {
                line[i] = c;
                i++;
            }
        }
        // If the file is not ended with \n
        int flag = lineExist(temp_file_name, line);
        if (!flag)
        {
            int fd0 = open(temp_file_name, O_RDWR | O_APPEND);
            if (fd0 == -1)
            {
                perror("Error");
            }
            line[i] = '\n';
            int size = write(fd0, &line, strlen(line));
            if (size == -1)
            {
                perror("Error");
            }
            
            close(fd0);
        }
        // Printing the result on screen
        fd0 = open(temp_file_name, O_RDONLY);
         if (fd0 == -1)
        {
            perror("Error");
        }
        
        memset(&line, 0, sizeof(line));
        i = 0;
        while(read(fd0, &c, 1) > 0)
        {
            if (c == '\n')
            {
                line[i] = c;
                int size = write(STDOUT_FILENO, &line, strlen(line));
                if (size == -1)
                {
                    perror("Error");
                }
                memset(&line, 0, sizeof(line));
                i = 0;
            }
            else
            {
                line[i] = c;
                i++;
            }
        }

        // Deleting the temporary file
        unlink(temp_file_name);
    }

    // If no argument is passed
    else if (argc == 1)
    {
        int fd = STDIN_FILENO;
        while(1)
        {
            char buff[1000] = "";
            int size = read(fd, &buff, 1000);
            if (size == -1)
            {
                perror("Error");
            }
            int re = write(STDOUT_FILENO, buff, strlen(buff));
            if (re == -1)
            {
                perror("Error");
            }
            
        }
        
    }
    else
    {
        printf("Wrong input arguments.\n");
        printf("Pass the name of the file as an argument.\n");
    }

    return 0;
}