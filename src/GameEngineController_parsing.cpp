#include "../includes/aleung-c_engine.hpp"

/*
**	Get the content of a file and returns it as a char *.
**	Public static method.
**	Thoses error are fatal as they impeach the loading of models.
*/

char		*GameEngineController::GetFileContent(std::string file_path)
{
	FILE *fp;
	long lsize;
	char *buffer;

	fp = fopen(file_path.c_str(), "rb" );
	if (!fp)
	{
		std::cout << "Error opening file: " << file_path << std::endl;
		exit(-1);
	}

	fseek(fp, 0L, SEEK_END);
	lsize = ftell(fp);
	rewind(fp);

	buffer = (char *)malloc(sizeof(char) * lsize);
	if (!buffer)
	{
		std::cout << "Error allocating vertex shader memory for file: " << file_path << std::endl;
		exit(-1);
	}

	if (fread(buffer, lsize, 1, fp) != 1)
	{
		fclose(fp);
		free(buffer);
		std::cout << "Error: entire read fails for file: " << file_path << std::endl;
		exit(1);
	}
	buffer[lsize] = '\0';
	fclose(fp);
	return (buffer);
}

/*
**	Get the content of a bmp file and returns it in the argument t_bmp_texture *.
**	Public static method.
**	Those errors are not fatal, and as such are hidden when not in debug mode.
*/

int		GameEngineController::LoadTextureFile(t_bmp_texture *texture, std::string path)
{
	// Open the file
	FILE	*file;
	bool	debugMode;

	debugMode = GameEngineController::Instance().DebugMode;
	if (!texture)
	{
		if (debugMode)
		{
			std::cout << "Unallocated texture object. Allocate it with malloc()" << path << std::endl;
		}
		return (-1);
	}

	file = fopen(path.c_str(), "rb");
	if (!file)
	{
		if (debugMode)
		{
			std::cout << "Image could not be opened: " << path << std::endl;
		}
		return (-1);
	}
	if (fread(texture->header, 1, 54, file) != 54)  // If not 54 bytes read == problem
	{
		if (debugMode)
		{
			std::cout << "Not a correct BMP file: " << path << std::endl;
		}
		return (-1);
	}
	texture->data_pos = *(int*)&(texture->header[10]);
	texture->image_size = *(int*)&(texture->header[34]);
	texture->width = *(int*)&(texture->header[18]);
	texture->height = *(int*)&(texture->header[22]);
	if ( texture->header[0] != 'B' || texture->header[1] != 'M' )
	{
		if (debugMode)
		{
			std::cout << "Not a correct BMP file: " << path << std::endl;
		}
		return (-1);
	}
	// Debug print.
	//printf("default_texture infos:\nimage_size: %d \nwidth: %d \nheight: %d\n",
	//	texture->image_size, texture->width, texture->height);

	// Create a buffer
	texture->data = (unsigned char *)malloc(sizeof(unsigned char) * texture->image_size);
	// Read the actual data from the file into the buffer
	fread(texture->data, 1, texture->image_size, file);
	fclose(file);
	return (0);
}
