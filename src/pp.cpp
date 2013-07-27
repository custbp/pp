#include "def.h"
#include "inc.h"
#include "pp.h"

//База
void Obj::draw() {

	glBegin(GL_QUADS);
      glVertex2f( _x, _y); 
      glVertex2f( _x + _w, _y); 
      glVertex2f( _x +_w, _y + _h); 
      glVertex2f( _x , _y + _h);
    glEnd();
}

bool Obj::collusion(const Obj & _target, int *r) { //TODO : есть проблема при соприкосновении с ракеткой, подумать/переделать
  //float top = _target.gTop(); // унылый баг сборки конкретно на mingw g++, на msvc все хорошо
  Obj &T = const_cast<Obj &>(_target); // временное решение
  //printf("BALL: L: %f, R: %f, T: %f, B: %f, ROCKET: L: %f, R: %f, T: %f, B: %f\n", T.gLeft(), T.gRight(), T.gTop(), T.gBottom(), gLeft(), gRight(), gTop(), gBottom());
  
  printf("ROCKET: L: %f, R: %f, T: %f, B: %f\n", gLeft(), gRight(), gTop(), gBottom());
  
  if ( T.cY() >= gTop() - T.gR() && T.cY() <= gBottom() + T.gR() && T.cX() >= gLeft() - T.gR() && T.cX() <= gRight() + T.gR() ) {
      return true;
  }

  return false;
}

//Ракетка
void Rocket::move(float x, float y) {

	float dx = drag_x + x - drag_x_mouse;
	//printf("new X: %f, getW()+ dx: %f\n", dx, dx + gW() );
	if ( dx < 1)  {
	  Obj::move( 1, 0);
	} else if ( dx + gW() + 1 >= GAME_WIDTH) {
	  Obj::move( GAME_WIDTH - gW() - 1, 0);
	} else {
	  Obj::move( dx, 0);
	}
}


//Мячик
void Ball::restart() {

	move(GAME_WIDTH/2 - R, GAME_HEIGHT/2 - R);
	if (rand()%10 - 5) {
	  	redirY();
		//PlaySound("name_of_file.wav", NULL, SND_FILENAME | SND_ASYNC);
		printf("redir Y\n");
	}
	if (rand()%10 - 5) {
		redirX();
		printf("redir X\n");
	}
}

void Ball::draw() {
	/*
	glBegin(GL_LINE_LOOP);
	//glColor3f (0.2, 0.9, 0.0);
	for ( int i=0 ; i< 360 ;i++)
	{
		glVertex2d(cos(i)*R + xPos() + R,sin(i)*R + yPos() + R);
	}
	glEnd();
	*/
  glBindTexture(GL_TEXTURE_2D, getT());
  glMatrixMode(GL_TEXTURE); 
  glLoadIdentity; 
  glTranslatef(0.5,0.5,0);
  glRotatef( ROT, 0.0, 0.0, 1.0); 
  //glRotatef( 0, 0.0, 1.0, 0.0); 
  //glRotatef( 0, 1.0, 0.0, 0.0); 
  
  
  
   
    glBegin( GL_QUADS );
    glTexCoord2d(-0.5,-0.5); glVertex2f( gLeft(), gTop() ); 
    glTexCoord2d(0.5,-0.5); glVertex2f( gRight(), gTop() ); 
    glTexCoord2d(0.5,0.5); glVertex2f( gRight(), gBottom() ); 
    glTexCoord2d(-0.5,0.5); glVertex2f( gLeft() , gBottom());
    glEnd();

  glMatrixMode(GL_TEXTURE);

  glLoadIdentity();
  glTranslatef(0,0,0);
  glRotatef(0, 0, 0, 0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity; 
}


void Rocket::draw() {
  glMatrixMode(GL_TEXTURE);

  glLoadIdentity();
  glRotatef(0, 0, 0, 0);


  //glMatrixMode(GL_MODELVIEW); 
  //
	glBindTexture(GL_TEXTURE_2D, getT());
    glBegin( GL_QUADS );
    glTexCoord2d(0.0,0.33); glVertex2f( gLeft(), gTop() ); 
    glTexCoord2d(1.0,0.33); glVertex2f( gRight(), gTop() ); 
    glTexCoord2d(1.0,0.66); glVertex2f( gRight(), gBottom() ); 
    glTexCoord2d(0.0,0.66); glVertex2f( gLeft() , gBottom());
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity; 
}


// загрузка png файла
bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) {
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    FILE *fp;

    if ((fp = fopen(name, "rb")) == NULL)
        return false;

    /* Create and initialize the png_struct with the desired error handler
     * functions.  If you want to use the default stderr and longjump method,
     * you can supply NULL for the last three parameters.  We also supply the
     * the compiler header file version, so that we know if the application
     * was compiled with a compatible version of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);

    if (png_ptr == NULL) {
        fclose(fp);
        return false;
    }

    // Allocate/initialize the memory for image information.  REQUIRED.
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
        return false;
    }

    /* Set error handling if you are using the setjmp/longjmp method
     * (this is the normal method of doing things with libpng).
     * REQUIRED unless you  set up your own error handlers in
     * the png_create_read_struct() earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
        fclose(fp);
        /* If we get here, we had a problem reading the file */
        return false;
    }

    /* Set up the output control if you are using standard C streams */
    png_init_io(png_ptr, fp);

    /* If we have already read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);

    /*
     * If you have enough memory to read in the entire image at once, and
     * you need to specify only transforms that can be controlled
     * with one of the PNG_TRANSFORM_* bits (this presently excludes
     * dithering, filling, setting background, and doing gamma
     * adjustment), then you can read the entire image (including pixels)
     * into the info structure with this call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, png_voidp_NULL);

    outWidth = png_get_image_width(png_ptr,info_ptr); // ширина изображение
    outHeight = png_get_image_height(png_ptr,info_ptr); // высота изображения

   // проверка на наличие альфа-канала
    switch (png_get_color_type(png_ptr,info_ptr)) {
        case PNG_COLOR_TYPE_RGBA:
            outHasAlpha = true;
            break;
        case PNG_COLOR_TYPE_RGB:
            outHasAlpha = false;
            break;
        default:
            std::cout << "Color type " << png_get_color_type(png_ptr,info_ptr) << " not supported" << std::endl;
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return false;
    }
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *outData = (unsigned char*) malloc(row_bytes * outHeight);

    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

    for (int i = 0; i < outHeight; i++) {
        // note that png is ordered top to bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }

    /* Clean up after the read, and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

    /* Close the file */
    fclose(fp);

    /* да, мы сделали это */
    return true;
}