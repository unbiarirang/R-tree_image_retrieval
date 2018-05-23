
void load_feature(int dim)
{
	std::ifstream inputStream;
    char buffer[101];
    char temp[100];
	char Filename[10]={"RGB"};
	strcat(Filename,itoa(dim,temp,10));
	strcat(Filename,"D.txt");

    inputStream.open("RGB4D.txt");
    if (! inputStream.is_open())
          { std::cout << "Error opening file"; exit (1); }
          while (!inputStream.eof() )
          {
            int j=0;
            inputStream.getline(buffer,100);
            for(int i=0;i<strlen(buffer);i++)
            {
                temp[j]=buffer[i];
                j++;
                if(buffer[i+1]==':')
                {
                    strcpy(IMGbuffer.name,temp);
                    memset(temp,NULL,sizeof(temp));
                    j=0;
                    i++;
                }
                if(buffer[i+1]==' ')
                {
                    IMGbuffer.RGBfeature.push_back(atoi(temp));
                    memset(temp,NULL,sizeof(temp));
                    j=0;
                    i++;
                }
            }
            IMGinfo.push_back(IMGbuffer);
            memset(IMGbuffer.name,NULL,sizeof(IMGbuffer.name));
            IMGbuffer.RGBfeature.erase(IMGbuffer.RGBfeature.begin(),IMGbuffer.RGBfeature.end());
	}
}