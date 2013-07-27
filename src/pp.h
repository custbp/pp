extern ALuint source;
extern ALuint source2;
extern ALuint source3;
extern float ROT;
//callbacks
void keyboard (unsigned char key, int x, int y);
void timer(int value);
void mouse_move(int x, int y);
void mouse_event(int button, int state, int x, int y);
void display();
//------------
bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData);
class Obj;
class Ball;
class Rocket;
class Texture;

//Базовый-----
class Obj {
private:
    float _x;
    float _y;
    float _w;
    float _h;
public:
    Obj() { _x = 0; _y = 0; _w = 1; _h = 1;};
    Obj(int x, int y, int w, int h) { _x = x; _y = y; _w = w; _h = h; };

    virtual void draw();

    float xPos() { return _x;}
    float yPos() { return _y;}
    float gW() { return _w; }
    float gH() { return _h; }
    float gTop() { return _y; }
    float gBottom() { return _y + _h; }
    float gLeft() { return _x; }
    float gRight() { return _x + _w; }
    float gR() { return gW()/2; }
    float cX() { return gLeft() + gW()/2; }
    float cY() { return gTop() + gH()/2; }

    virtual void move(float x, float y) {
      _x = x ? x : _x;
      _y = y ? y : _y;
    }

    virtual bool collusion(const Obj & _target, int *r);
};
//------------


class Texture {
    private:
    GLuint  t;
    int width, height; // ширина и высота текстуры
    bool hasAlpha;     // наличие альфа-канала
  public:
    Texture() {};
    Texture(char * filename) {
      GLubyte *textureImage;
      glGenTextures(1, &t);
      loadPngImage(filename, width, height, hasAlpha, &textureImage);
      glBindTexture(GL_TEXTURE_2D, t);
      glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? 4 : 3, width,
                          height, 0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureImage);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }

    GLuint getT() {
      return t;
    };
};

//Ракетки-----
class Rocket : public Obj, public Texture {
private:
  float drag_x;
  float drag_x_mouse;
  bool used;
public:
  Rocket() {};
  Rocket(float x, float y, char * filename) : Obj(x, y, ROCKET_W, ROCKET_H), Texture(filename) {
    used = false;
  }

  void use(float x, float y) {
    drag_x = xPos();
    drag_x_mouse = x;
    used = true;
  };

  bool isUse() { return used; }
  void move(float x, float y);
  void draw();
};
//------------

//Мячик-------
class Ball : public Obj, public Texture {
private:
  float R;
  float vx;
  float vy;
public:
  Ball() {};
  Ball(float r, float x, float y, char * filename) : Obj(x,y,2*r,2*r), Texture(filename) {
    R = r;
    vx = -4;
    vy = 4;
  };

  void redirX() { vx = -vx; }
  void redirY() { vy = -vy; }

  void tick() {
    move(xPos() + vx, yPos() + vy);
  };

  void draw();

  void restart();

  void increaseV() { //TODO:
    vx++;
    vy++;
  };



};
//------------




//Поле
class Field : public Texture {
private:
  GLuint  back;
  int width, height; // ширина и высота текстуры
  bool hasAlpha;     // наличие альфа-канала
public:
  Field() {};
  Field(float w, float h, char *filename) : Texture(filename) {
    
  }

  void draw() {
    glBindTexture(GL_TEXTURE_2D, getT());
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glRotatef(0, 0, 0, 0);

    glBegin( GL_QUADS );
    glTexCoord2d(0.0,0); glVertex2d(0,0);
    glTexCoord2d(1.0,0); glVertex2d(GAME_WIDTH, 0);
    glTexCoord2d(1.0,1); glVertex2d(GAME_WIDTH, GAME_HEIGHT);
    glTexCoord2d(0.0,1); glVertex2d(0, GAME_HEIGHT);
    glEnd();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity;
  };
};


//игра, перебор с классами =)
class Game {
private:
  Field *f;
  Rocket *rT;
  Rocket *rB;
  Ball *b;
public:
  Game() {

  };
 
  void init(float w, float h) {
    b = new Ball(BALL_RADIUS, w/2 - BALL_RADIUS, h/2 - BALL_RADIUS, "img\\3d_pepsi_logo_by_010j-d4t9052.png");
    rT = new Rocket(w/2 - h/2, ROCKET_PADDING, "img\\dark-red-button.png");
    //rB = new Rocket(w/2 - h/2, h - ROCKET_H - ROCKET_PADDING, "");
    rB = new Rocket(w/2 - h/2, h - ROCKET_H - ROCKET_PADDING, "img\\blue-button (1).png");
    f = new Field (w, h, "img\\square_background.png");

    srand (time(NULL));
  }


  ~Game() {
    delete b;
    delete rT;
    delete rB;
    delete f;
  }

  void start() {

  }

  void update() {
    f->draw();
    b->draw();
    rT->draw();
    rB->draw();
  }

  void tick() {
    ROT+=1.5;
    if ( (b->gTop() < ROCKET_PADDING) || ( b->gBottom() > GAME_HEIGHT - ROCKET_PADDING)) {
        //ball.redirY();
        printf("GOL\n");
        //b->restart();
        b->redirY();
        //b->redirY();
    }
    if ( (b->gLeft() < 0) || ( b->gRight() > GAME_WIDTH)) {
        b->redirX();
        alSourcePlay(source3);
    }
    int top = 0, bot = 0;
    
    if (rB->collusion(*b, &bot)) {
      printf("BCOLLUSION RET %d\n", bot );
      alSourcePlay(source);
      b->redirY();
    }
    
    if (rT->collusion(*b, &top)) {
      printf("TCOLLUSION RET %d\n", bot );
      alSourcePlay(source);
        b->redirY();
    }
    
    //printf("Collision with R1\n");

    b->tick();
    
  }

  void use_rocket(int x, int y) {
      rT->use(x,0);
      rB->use(x,0);
  }

  void move_rocket(int x, int y) {
      rT->move(x, 0);
      rB->move(x, 0);
  }
};