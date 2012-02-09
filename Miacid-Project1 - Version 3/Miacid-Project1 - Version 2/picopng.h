#ifndef PICOPNG_H
#define PICOPNG_H

#include <vector>

//Do not use this function for loading PNG graphics!
//Instead use the class provided whenever you want to use PNG.
//This function is provided solely for the use of the PNG class and is not recommended elsewhere.
bool loadPNG(const char * filename, std::vector<unsigned char> &raw_32bit_data,
			 int &image_width, int &image_height);

struct Pixel
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;

	Pixel()
	{
		this->red = 0;
		this->green = 0;
		this->blue = 0;
		this->alpha = 0;
	}

	Pixel(unsigned char r, unsigned char g, unsigned char b)
	{
		this->red = r;
		this->green = g;
		this->blue = b;
		this->alpha = 1; //default to completely visible
	}

	Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		this->red = r;
		this->green = g;
		this->blue = b;
		this->alpha = a;
	}

	bool operator==(Pixel other)
	{
		return (this->red == other.red && this->green == other.green &&
			this->blue == other.blue && this->alpha == other.alpha);
	}
};

struct PNG
{
private:
	int width;
	int height;
	bool failflag;
	bool isloaded;
	GLuint texid;
	Pixel * data; //raw 32-bit data

public:
	PNG()
	{
		this->data = NULL;
		this->destroy();
	}

	PNG(const char * filename)
	{
		this->data = NULL;
		this->decode(filename);
	}

	void destroy()
	{
		this->isloaded = 0;
		this->width = 0;
		this->height = 0;
		this->failflag = 0;

		if (this->data != NULL)
			delete[] this->data;

		this->data = NULL;
	}

	void decode(const char * filename)
	{
		//First, let's reset the data
		this->destroy();

		//Attempt to read and decode the given file
		std::vector<unsigned char> raw_data;
		this->failflag = !loadPNG(filename, raw_data, this->width, this->height); //returns 0 when fails, have to flip it

		if (this->failflag)
		{
			//Failed to read file
			this->width = 0;
			this->height = 0;
			this->data = NULL;
		}
		else
		{
			//Read successfully, now store the raw data
			//WARNING: the data store will increase dramatically! Be very careful with the number of PNGs you store
			int numbytes = this->width*this->height*4;
			int numpixels = this->width*this->height; //rgba

			this->data = new Pixel[numpixels];
			int pixel = 0; //current pixel number

			for (int byte = 0; byte < numbytes; byte+=4)
			{
				this->data[pixel].red = raw_data[byte];
				this->data[pixel].green = raw_data[byte+1];
				this->data[pixel].blue = raw_data[byte+2];
				this->data[pixel].alpha = raw_data[byte+3];
				pixel++;
			}

			//Empty out raw_data
			raw_data.empty();

			this->isloaded = 1;
		}
	}

	~PNG()
	{
		this->destroy();
	}

	//Check variables
	bool isBad()
	{
		return this->failflag;
	}

	bool isOkay()
	{
		return !this->failflag;
	}

	int getWidth()
	{
		return this->width;
	}

	int getHeight()
	{
		return this->height;
	}

	Pixel getPixel(int x, int y)
	{
		Pixel result;

		if (x >= 0 && y >= 0 && x < this->width && y < this->height)
			result = this->data[y*width + x];

		return result;
	}

	//Using Textures
	void bind()
	{
		if (this->isOkay())
		{
			if (!this->texid)
			{
				//Create the texture the first time it is used.
				glGenTextures(1, &(this->texid));
				glBindTexture(GL_TEXTURE_2D, this->texid);

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->data);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				//Remove all of the data now that it's no longer needed..?
				//delete[] this->data;
				//this->data = NULL;
			}

			//Bind the texture
			glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_NOTEQUAL, 0);
			glBindTexture(GL_TEXTURE_2D, this->texid);
		}
	}

	void unbind()
	{
		if (this->isOkay())
		{
			glDisable(GL_ALPHA_TEST);
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
	}

	//Display Picture On Screen
	void displayAt(float x, float y, bool fixed = false)
	{
		this->displayAt((int)x, (int)y, fixed);
	}

	void displayAt(int x, int y, bool fixed = false)
	{
		glPushMatrix();
		if (fixed) glLoadIdentity(); //render at a fixed location on the screen
		glDisable(GL_DEPTH_TEST);
		//glTranslatef(x, glutGet(GLUT_WINDOW_HEIGHT) - y - this->height, 0); //have to vertically
		glTranslatef(x, glutGet(GLUT_WINDOW_HEIGHT) - y - this->height, 0);

		if (this->isloaded)
			this->bind();
		else
			glColor4f(1, 1, 1, 0.25);

		glBegin(GL_TRIANGLE_STRIP);
		{
			glColor3f(1, 1, 1);

			glTexCoord2f(0, 0);
			glNormal3f(0, 0, 1);
			glVertex2f(0, this->height);

			glTexCoord2f(0, 1);
			glNormal3f(0, 0, 1);
			glVertex2f(0, 0);

			glTexCoord2f(1, 0);
			glNormal3f(0, 0, 1);
			glVertex2f(this->width, this->height);

			glTexCoord2f(1, 1);
			glNormal3f(0, 0, 1);
			glVertex2f(this->width, 0);	
		}
		glEnd();

		this->unbind();

		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
	}

	//Overloaded Operator
	PNG& operator=(PNG &original)
	{
		//Check if the same (ignore if they are)
		if (this == &original)
			return *this;

		//Destroy
		this->destroy();

		//Copy header information
		this->width = original.width;
		this->height = original.height;
		this->failflag = original.failflag;
		original.isloaded = 0; //we have to transfer the data over...

		//Copy the raw image data
		int numpixels = this->width*this->height; //rgba
		this->data = new Pixel[numpixels];

		for (int pixel = 0; pixel < numpixels; pixel++)
			this->data[pixel] = original.data[pixel];

		this->isloaded = original.isloaded; //now it's okay...

		//Return a pointer to myself
		return (*this);
	}
};

//PNG loading example:
//	PNG tothouse("tothouse.png");			//one line, just specify the filename; everything else is done for you
//	tothouse.getWidth();					//returns the image width
//	tothouse.getHeight();					//returns the image height
//	Pixel pix = tohouse.getPixel(0,0);		//returns a pixel at a point

//Further support will be provided later on for simpler texturing.
//It should still be possible to do with the raw data, however

struct Sequence
{
	float duration; //number of second to complete whole sequence, each image stays for same amount of time
	int numframes; //number of frames in the sequence
	PNG * frame; //the image data

	Sequence()
	{
		this->duration = 0;
		this->numframes = 0;
		this->frame = NULL;
	}

	void destroy()
	{


	}

	void setDuration(float s)
	{
		if (this->duration >= 0)
			this->duration = s;
	}

	float getDuration()
	{
		return this->duration;
	}

	int getNumFrames()
	{
		return this->numframes;
	}

	void append(const char * filename)
	{

	}
};

#endif