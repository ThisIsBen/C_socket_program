/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<stdlib.h>//malloc & free
#define client_folder " "//can put in client folder name :   ./client_Download_file_folder/

 void create_file_for_dwn(char* filename,char* content)
 {
	 
	 FILE* gp=fopen(filename,"a");
	 
	 int i;
	 
	 for(i=0;i<strlen(content);i++)
	 {
		 if(content[i]=='\n')
		 {
			 fprintf(gp,"\n");
			 
		 } 
		 
		 else
			 fprintf(gp,"%c",content[i]);
		 
		 
		 
		 
	 }
	 
	
	 fclose(gp);
	 
	 
	 
 }
 
 void open_file_in_vi(char* filename)
 {
	 
	 
	 char open_vi[50]="vi ";
	 strcat(open_vi,filename);
	
	 system(open_vi);
	 
	 
	 
	 
	 
 }
 
 void read_editted_file_content(char *edit_filename,char * server_reply)
 {
	 
	 FILE *fp;
	char * line=NULL;
	size_t len=0;
	ssize_t read;
	long int Lines=0;
	
	fp=fopen(edit_filename,"r");
	//fail
	if(fp==NULL)
	{
		printf("Error occured at function :read editted file content" );	
		
	}
	//succeed
	else
	{
		memset(server_reply,0,strlen(server_reply));
		while ((read=getline(&line,&len,fp)) != -1)
		{
			Lines++;
			strcat(server_reply,line);
			
		}
		if(Lines==0)
		{
			strcat(server_reply," ");
		}
		
		
		fclose(fp);
	}
	if(line)
		free(line);
	
	 
	 
	 
	 
	 
	 
	 
	 
 }
 int whether_file_exist(char *client_message)
{

	FILE *fp;
	int file_exist;
	fp=fopen(client_message,"r");
	//if file doesn't exist
	if(fp==NULL)
	{
		file_exist=1;
	}
	//if file exists
	else
	{
		file_exist=0;
		fclose(fp);
		
	}
	
	return file_exist;


}
 
 
 
 //remove the client side temp file used in editting file on the server side .
void do_remove(char *del_file_name)
{

    
   
 
	//remove the client side temp file used in editting file on the server side .
   remove(del_file_name);
   

}
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
	char edit_filename_with_vi[100];
	//to denote that whether should client read_editted_file.
    char do_read_editted_file;
	char file_location[80];//store file location on client side.
	char cd_to_client_folder[50];
	//char edit_filename_for_read_use[100];
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    //keep communicating with server
    while(1)
    {
		//////////////////read content in vi editor.
				if(do_read_editted_file=='Y')
				{	
					
					//printf("read editted file later!\n");
					//send the edit content back to server.
					strcpy(server_reply,"Read file after editted in vi.");
					if( write(sock , server_reply , strlen(server_reply) ) < 0)
					{
						puts("Send failed");
						return 1;
					}
					memset(server_reply,0,strlen(server_reply));
					
					
					
					//Receive a reply from the server
					if( read(sock , server_reply , 2000 ) < 0)
					{
						puts("recv failed");
						break;
					}
					
					 if(strcmp(server_reply,"Ok send your file.")==0)
					{	
						memset(server_reply,0,strlen(server_reply));
						//read content of hthe editted file after using vi editor.
						read_editted_file_content(edit_filename_with_vi,server_reply);
						//finish reading the content of the editted file.
						
						
						if( write(sock , server_reply , strlen(server_reply) ) < 0)
						{
							puts("Send failed");
							return 1;
						}
						memset(server_reply,0,strlen(server_reply));
				
				
					}
						//remove the client side temp file used in editting file on the server side .
						do_remove(edit_filename_with_vi);
						do_read_editted_file='N';
						
					printf("The file you editted has been updated to the server side .\n");
				}

//////////////////end reading content in vi editor.		
	
        printf("\n\nHi! welcome to Ben's socket HW\nThe followings are some commands that you can use to achieve some cool purposes.\nC :Create a new file on server.\nR :Remove an existing file on server.\nL :List out all the files on server.\nD :Download a file from server.\nE :Edit an existing file on server with \"vi\" editor.\nCopy :Copy an existing file on server.\nV :View and edit an existing file on client side with \"vi\" editor.\n\nEnter your magical command : ");
		memset(message,0,sizeof(message));
         gets(message);
         
		 
		 //////////////////////call V cmd

			if(strcmp(message,"V")==0)
			{	
		
				printf("Type in the file on client side that you wnat to edit in \"vi\"\n");
				gets(message);
				if(whether_file_exist(message)==0)
				{
					
					
					strcat(file_location,message);
					
					open_file_in_vi(file_location);
					
					
				}
					else
						printf("Sorry! %s doesn't exist on client side ,so you can't edit it in vi\n",message);

				memset(message,0,sizeof(message));
				memset(file_location,0,sizeof(file_location));
				continue;

			}


/////////////////end call V cmd


		 
        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
       
		memset(server_reply,0,sizeof(server_reply));	
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
         
	    puts("Server reply :");
        puts(server_reply);
	
	
////////////////////////call C cmd get filename
	 if(strcmp(server_reply,"What's the name of your new file?\n")==0)
	  {	
		//get new file name
//bzero(server_reply,sizeof(server_reply));
		gets(message);
		
		//Send new file name to server.
	        //Send some data
		if( write(sock , message , strlen(message) ) < 0)
		{
		    puts("Send failed");
		    return 1;
		}

		//strcpy(server_reply,"\0");
		//printf("this is server reply before read created %s\n",server_reply);
		memset(message,0,strlen(message));
		memset(server_reply,0,strlen(server_reply));
		if( read(sock , server_reply , 2000) < 0)
		{
		    puts("recv failed");
		    break;
		}
		//print server reply.

		
		printf("This is server reply : %s\n",server_reply);
		
		//continue;
		
	}


///////////////////////end C



///////////////////////call R cmd
		//if call R
	else if(strcmp(server_reply,"Which file do you want to delete?\n")==0)
	{	
		//memset(server_reply,0,strlen(server_reply));
		//get file name
		//fgets(message,sizeof(message),stdin);
		gets(message);
		
		
		//Send file which will be removed name to server.
		if( write(sock , message , strlen(message) ) < 0)
		{
		    puts("Send failed");
		    return 1;
		}
		
		memset(server_reply,0,sizeof(server_reply));
		//Recv the notice of creating the file successfully. 
		if( read(sock , server_reply , 2000) < 0)
		{
		    puts("recv failed");
		    break;
		}
		

		printf("This is server reply : %s\n",server_reply);


		//continue;
	}
	









////////////////////end call R cmd


////////////////////call D cmd


	//if call D
	 else if(strcmp(server_reply,"Which file do you want to download?\n")==0)
	{	
	
		//get file name
		gets(message);
		
		//Send  file name to server to check whether this file has already existed.
		
		if( write(sock , message , strlen(message) ) < 0)
		{
		    puts("Send failed");
		    return 1;
		}

		memset(server_reply,0,strlen(server_reply));		
		//Recv the notice of creating the file successfully. 
		if( read(sock , server_reply , 2000) < 0)
		{
		    puts("recv failed");
		    break;
		}
		
		//if server reply : file dosen't exist;
		if(strcmp(server_reply," does not exist!\n")==0)
		{
			//print file does not exist.
			printf("%s does not exist!\n",message);
			
			
		}
		//create a new file to download the text from the file on server side.
		else
		{
			strcat(message,"_Download");
			//transfer the content to local existing or new file.
			//set the dest folder to set up the downloaded file from server side.
			strcpy(file_location,client_folder);
			strcat(file_location,message);
			
			create_file_for_dwn(file_location,server_reply);
			
			//let user know that the file has been downloaded.
			printf("%s has been downloaded\n",message);
			
			memset(file_location,0,sizeof(file_location));
			
		}
		
		
		

		

		
	}












/////////////////end call D cmd



////////////////////call E cmd
		
			
	
	//if call E
	 else if(strcmp(server_reply,"Which file do you want to edit in \"vi\"?\n")==0)
	{	
		
		//get file name
		gets(message);
		printf("The file name I got is %s\n",message);
		//Send  file name to server to check whether this file has already existed.
		
		if( write(sock , message , strlen(message) ) < 0)
		{
		    puts("Send failed");
		    return 1;
		}


memset(server_reply,0,strlen(server_reply));		
		//Recv content of the file on server side that user wants to edit. 
		if( read(sock , server_reply , 2000) < 0)
		{
		    puts("recv failed");
		    break;
		}
		
		//if server reply : file dosen't exist;
		if(strcmp(server_reply,"File does not exist.You can't edit it\n")==0)
		{
			//print file does not exist.
			printf("%s does not exist.You can't edit it\n",message);
			
			
		}
		//create a new file to download the text from the file on server side.
			else
			{
				
				stpcpy(edit_filename_with_vi,message);
				strcat(edit_filename_with_vi,"for_edit");
				//transfer the content to local existing or new file.
				create_file_for_dwn(edit_filename_with_vi,server_reply);
				
				//open the file with vi 
				open_file_in_vi(edit_filename_with_vi);
				printf("You have editted the file which exists on server side.\n");
				//read edit content from for_edit file.
				//read_editted_file_content(edit_filename,server_reply);
				
				//client can read_editted_file after edit.
				do_read_editted_file='Y';
				
				//strcpy(edit_filename_for_read_use,edit_filename);
				//strcpy(content_after_edit_in_vi,server_reply);
				
				
			}
		
		memset(server_reply,0,strlen(server_reply));		
		memset(message,0,strlen(message));		

		

		
	}

///////////////////////call Copy cmd
		//if call Copy
	else if(strcmp(server_reply,"Which file do you want to copy?\n")==0)
	{	
		
		//get file name
	
		gets(message);
		
		
		//Send file which will be removed name to server.
		if( write(sock , message , strlen(message) ) < 0)
		{
		    puts("Send failed");
		    return 1;
		}
		memset(message,0,sizeof(message));
		memset(server_reply,0,sizeof(server_reply));
		//Recv the notice of creating the file successfully. 
		if( read(sock , server_reply , 2000) < 0)
		{
		    puts("recv failed");
		    break;
		}
		
		printf("%s\n",server_reply);
		
		
		//get new file name
	
		gets(message);
		//Send file which will be removed name to server.
		if( write(sock , message , strlen(message) ) < 0)
		{
		    puts("Send failed");
		    return 1;
		}
		memset(message,0,sizeof(message));
		
		memset(server_reply,0,sizeof(server_reply));
		//Recv the notice of creating the file successfully. 
		if( read(sock , server_reply , 2000) < 0)
		{
		    puts("recv failed");
		    break;
		}
		printf("%s\n",server_reply);
		
		memset(server_reply,0,sizeof(server_reply));
		
	}
	









////////////////////end call Copy cmd


///////////////////trash cmd handle
	/*else
	{	
		//if user types in the cmd that doesn't exsit.
		//if(strcmp(message,"L")!=0)
		//{/
			
			printf("This command doesn't exist!\n");
		//}

		


	}
	*/
/////////////////end trash cmd


	memset(server_reply,0,sizeof(server_reply));
	memset(message,0,sizeof(message));



       
   }
     
    close(sock);
    return 0;
}