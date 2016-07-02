/*
    C socket server example, handles multiple clients using threads
*/
 
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#define _GNU_SOURCE
#define PATH_MAX 40

//the thread function
void *connection_handler(void *);

 
//create new file .return 1:file has already existed on server side. return 0: it's a new file.
void create_file(char* client_message)
{
	FILE *fp=fopen(client_message,"r");
	
	//if file doesn't exist,create a new file 
	if(fp==NULL)
	{	
		
		
		FILE *gp;
		gp=fopen(client_message,"w");
		
		strcat( client_message," has created!\n");
		fclose(gp);
		
		
	}
	//if file has already exist ,let user know.
	else
	{
		
		strcat(client_message," has already existed on server side.\n");
		fclose(fp);
		
		
	}
	
	
	
} 

//return 0 means file removal is succeed,retrun 1 means it's failed.
 int do_remove(char del_file_name[])
{

    
   
 
 
   if(remove(del_file_name) == 0 )
      return 0;
   else
      return 1;

}


void do_list(char* client_message)
{
 FILE *fp;
 int status;
 char path[PATH_MAX];


 fp = popen("ls *", "r");
 if (fp == NULL)
  printf("Handle error \n");


 while (fgets(path, PATH_MAX, fp) != NULL)
  {

	strcat(client_message,path);
	
	
}

 status = pclose(fp);
 if (status == -1) {
  printf("Error reported by pclose()\n");
  
 } else {
  printf("Use macros described under wait() to inspect `status' in order to determine success/failure of command executed by popen()\n");
   
 }
}
//return 0: file exist.  return 1: file doesn't exist.
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


void  read_file_content(char *client_message)
{
	FILE *fp;
	char * line=NULL;
	size_t len=0;
	ssize_t read;
	long int Lines=0;
	
	fp=fopen(client_message,"r");
	//fail
	if(fp==NULL)
	{
		printf("EXIT_FAILURE");	
		
	}
	//succeed
		else
		{
			memset(client_message,0,strlen(client_message));
			while ((read=getline(&line,&len,fp)) != -1)
			{
				Lines++;
				strcat(client_message,line);
				
			}
			
			if(Lines==0)
			{
				strcat(client_message," ");
			}
			//print content to test D.	
			printf("file content: %s",client_message);
			
			
			fclose(fp);
		}
	if(line)
		free(line);
	



}

void write_editted_content_to_file(char* editted_filename,char* content)
{
	printf("this is write in content %s\n",content);
	 FILE* gp=fopen(editted_filename,"w");
	 
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
//copy a file
void copy_file(char* copied_file_name, char* new_file_name)
{

	char cmd[100]="cp ";
	strcat(cmd,copied_file_name);
	strcat(strcat(cmd," "),new_file_name);
	system(cmd);




}

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 2);
     
    
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
    
	
    char received_cmd;
	char editted_filename[100];
	 char Copied_filename[100];
    //Send some messages to the client
    //message = "Greetings! I am your connection handler\n";
    //write(sock , message , strlen(message));
     
    //message = "Now type something and i shall repeat what you type \n";
    //write(sock , message , strlen(message));
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {


////////////////recv C cmd	
		if(strcmp ( client_message,"C")==0 )
		{
			received_cmd='C';//it means we have to do  C  cmd exec later.
		

			//memset(client_message,0,sizeof(client_message));

			strcpy(client_message,"What's the name of your new file?\n");

			//Send the message back to client
			if(write(sock , client_message , strlen(client_message))<0)
			{
				 puts("Send failed");
				 return 1;
			}
			memset(client_message,0,sizeof(client_message));
		}

///////////////recv C end




////////////////recv R cmd

		else if(strcmp (client_message,"R")==0)
		{
			received_cmd='R';//it means we have to do  R  cmd exec later.
			strcpy(client_message,"Which file do you want to delete?\n");
			//Send the message back to client
	

			if(write(sock , client_message , strlen(client_message))<0)
			{
						    puts("Send failed");
						    return 1;
			}
			memset(client_message,0,sizeof(client_message));
		
		
 		


		}




///////////////recv R end
	
///////////////recv L cmd
		else if(strcmp (client_message,"L")==0)
		{
			received_cmd='L';//it means we have to do  L  cmd exec later.
			
			
			strcpy(client_message,"The followings are the files created on server side.\n");
			//call ls
			do_list(client_message);
	
			strcat(client_message,"The aboves are the files created on server side.\n");
	
			
 			//Send the message back to client
			if(write(sock , client_message , strlen(client_message))<0)
			{
						    puts("Send failed");
						    return 1;
			}
			memset(client_message,0,sizeof(client_message));

		}





///////////////recv L end

///////////////recv D cmd

	else if(strcmp (client_message,"D")==0)
	{
		received_cmd='D';//it means we have to do  D  cmd exec later.
		strcpy(client_message,"Which file do you want to download?\n");
		//Send the message back to client
		if(write(sock , client_message , strlen(client_message))<0)
		{
			puts("Send failed");
			return 1;
		}

		memset(client_message,0,sizeof(client_message));
		
		
		
 		


	}









/////////////////end recv D cmd

//////////////////recv E cmd


		else if(strcmp (client_message,"E")==0)
		{
			received_cmd='E';//it means we have to do  E  cmd exec later.
			strcpy(client_message,"Which file do you want to edit in \"vi\"?\n");
			
			if(write(sock , client_message, strlen(client_message)) < 0)
				{
				    puts("Send failed");
				    return 1;
				}
				
				memset(client_message,0,sizeof(client_message));
				

		}
		
		
		else if(strcmp (client_message,"Copy")==0)
		{
			received_cmd='P';//it means we have to do  Copy  cmd exec later.
			strcpy(client_message,"Which file do you want to copy?\n");
			//Send the message back to client
	

			if(write(sock , client_message , strlen(client_message))<0)
			{
						    puts("Send failed");
						    return 1;
			}
			memset(client_message,0,sizeof(client_message));
		
		
 		


		}










//////////////////////////end recv E cmd

///////////////////////////recv write editted content into file

//get the editted content and write them to the file.
			else if(strcmp (client_message,"Read file after editted in vi.")==0)
			{
				strcpy(client_message,"Ok send your file.");
				if(write(sock , client_message , strlen(client_message))<0)
				{
						    puts("Send failed");
						    return 1;
				}
				memset(client_message,0,sizeof(client_message));
				
				received_cmd='e';
				


			}

///////////////////////////end  writing editted content into file







////////////////////////////////client send a file name according to the previous command.

	else
	{
///////////////recv C cmd
		if(received_cmd=='C')
		{
				
				//int file_exist;
				//printf("this is new file info before check %s \n",client_message);
				//if the this file doesn't exist on server side,
		
			    create_file(client_message);
				
				
	
				
				
        			
				//Send the message back to client
				if(write(sock , client_message , strlen(client_message))<0)
				{
						    puts("Send failed");
						    return 1;
				}
				memset(client_message,0,sizeof(client_message));
				//printf("this is new file info %s \n",client_message);
				

				received_cmd=' ';
				
				

 		}
/////////////////////end creating new file exec


/////////////////////recv removing file cmd 
		//remove an existing file
		else if(received_cmd=='R')
		{
				
				
				
				
				if(do_remove(client_message)==0)
				{
					strcat(client_message," has been deleted!\n");
				}
				
				else 
					strcat(client_message," does not exist or can't be deleted!\n");
				
	
				
				
				if(write(sock , client_message, strlen(client_message)) < 0)
				{
				    puts("Send failed");
				    return 1;
				}
				
				memset(client_message,0,sizeof(client_message));
				
				received_cmd=' ';

 		}

///////////////////////////end exec removing file

//////////////////////////exec Download cmd

		else if(received_cmd=='D')
		{
				//if_file_exist
				int exist_or_not;
			    if(exist_or_not=whether_file_exist(client_message)==0)
				{
					//read the content of the file which user wants to download.
					 read_file_content(client_message);
					 

				}
				//download error :if the file does not exist.
				else
				{
					strcpy(client_message," does not exist!\n");
					printf("this is D result: %s ",client_message);
				        
				}
				//return the file content or the download error msg.
				if(write(sock , client_message, strlen(client_message)) < 0)
				{
				    puts("Send failed");
				    return 1;
				}
				
				memset(client_message,0,sizeof(client_message));
				
				received_cmd=' ';


 		}







////////////////////////end exec Download cmd

/////////////////////////recv Edit cmd

		else if(received_cmd=='E')
		{
				//To record the editted_filename because we have to store the editted content back to it.
				strcpy(editted_filename,client_message);
				//received_cmd='B';
				received_cmd=' ';
				//if_file_exists
				
				printf("Edit file name: %s",editted_filename);
				int exist_or_not;
				//if file exists
			    if(exist_or_not=whether_file_exist(client_message)==0)
				{
					//read the content of the file which user wants to download.
					 read_file_content(client_message);
					 

				}
					//download error :if the file does not exist.
					else
					{
						strcpy(client_message,"File does not exist.You can't edit it\n");
							
					}
				
				
				
				
				//return the file content or the download error msg.
				if(write(sock , client_message, strlen(client_message)) < 0)
				{
				    puts("Send failed");
				    return 1;
				}
				
				memset(client_message,0,strlen(client_message));
				

				

				
				

		}
		
		
		
		
		/////to write the editted content back to the file on server side.
		
		else if(received_cmd=='e')
		{
			
			//get the editted content and write them to the file.
			write_editted_content_to_file(editted_filename,client_message);
			printf("this is editted content form client %s\n",client_message);
			printf("write_editted_content_to_file %s ",editted_filename);
			
			
			memset(client_message,0,sizeof(client_message));
			received_cmd=' ';
			
		}
///////////////////////end exec E cmd



////////////////////////exec Copy cmd

		else if(received_cmd=='P')
		{
				
				//int file_exist;
				//printf("this is new file info before check %s \n",client_message);
				//if the this file doesn't exist on server side,
				int exist_or_not;
			    if(exist_or_not=whether_file_exist(client_message)==0)
				{
					strcpy(Copied_filename,client_message);
					strcpy(client_message,"What's the name of your new file which is the copy of ");
					strcat(client_message,Copied_filename);
				}
					else
					{
						
						strcat(client_message," doesn't exist! You can't copy it.\n");
						
					}
				
				
        			
				//Send the message back to client
				if(write(sock , client_message , strlen(client_message))<0)
				{
						    puts("Send failed");
						    return 1;
				}
				memset(client_message,0,sizeof(client_message));
				//printf("this is new file info %s \n",client_message);
				

				received_cmd='p';
				
				

 		}

		else if(received_cmd=='p')
		{
			copy_file(Copied_filename, client_message);
			
			strcat(client_message," has been created by coping from ");
			strcat(client_message,Copied_filename);
			
			//Send the message back to client
				if(write(sock , client_message , strlen(client_message))<0)
				{
						    puts("Send failed");
						    return 1;
				}
				memset(client_message,0,sizeof(client_message));
				//printf("this is new file info %s \n",client_message);
				
			
			
			received_cmd=' ';
			
		}









////////////////////////end Copy cmd exec
















///////////////////////////recv trash cmd
		else
		{
			printf("illegal cmd before send %s \n",client_message);
			//strcat( client_message," is created!\n");

				
				strcpy(client_message,"This command doesn't exist!\n");
				if(write(sock , client_message, strlen(client_message)) < 0)
				{
				    puts("Send failed");
				    return 1;
				}
				printf("illegal cmd %s \n",client_message);			
				
				memset(client_message,0,sizeof(client_message));
				
				received_cmd=' ';
			
			
		}
///////////////////////////end exec trash cmd

	}
	

	//memset(client_message,0,sizeof(client_message));
  }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    //Free the socket pointer
    free(socket_desc);
     
    return 0;
}