#define SFML_STATIC

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


#include <iostream>
#include <string>
#include <sstream>


using std::stringstream;
using std::cout;
using std::endl;
using std::string;

/*
//function used for multithreading
void ThreadFunction(void* UserData)
{
  //convert our userdata back to its original type

  int *testObject = static_cast<int*>(UserData);
  int test2 = *testObject;
}
*/
bool Running = true;

float p1Y = 0;
int wrapper = 20;

int player1Score = 0;
int player2Score = 0;

class Paddle
{
  public:

    void move(int direction)
    {

      if(direction == 0)			//direction of movement; up = 0, down  = 1, 2 = none
      {
	yadder -= 3;
	if(acceloration >= 0)
	  acceloration = 0;
	acceloration -= 0.05;
      }
      else if(direction == 1)
      {
	yadder += 3;
	if(acceloration <= 0)
	  acceloration = 0;
	acceloration += 0.05;
      }
      else if(acceloration != 0 && direction == 2)
      {

	if(acceloration > wrapper)
	  acceloration = wrapper;
	else if(acceloration < -wrapper)
	  acceloration = -wrapper;
	if(acceloration < 0)
	{
	  yadder += acceloration;
	  acceloration += 0.05;
	}
	else if(acceloration > 0)
	{
	  yadder += acceloration;
	  acceloration -= 0.05;

	}
      }

     if(yadder < -30)
	yadder = -30;
     else if( yadder > 520)
	yadder = 520;


     redraw();
    }

    void redraw()
    {

      paddleShape.SetPointPosition(0, xpos,ypos+yadder);
      paddleShape.SetPointPosition(1, xpos, ypos + 50 + yadder);
      paddleShape.SetPointPosition(2, xpos + 10, ypos + 50 + yadder);
      paddleShape.SetPointPosition(3, xpos + 10, ypos + yadder);

    }

    void reset()
    {





    }

    void aimove(int direction)
    {
      if(direction == 0)
	move(0);
      else if(direction == 1)
	move(1);
      else
	move(2);



    }

    Paddle(int xStart, int yStart)
    {
      xadder = 0;
      yadder = 0;
      acceloration = 0;

     // ypos = 30;
     // xpos = 30;
      xpos = xStart;
      ypos = yStart;


      paddleShape.AddPoint(xpos, ypos + yadder,  sf::Color(255, 0, 0),     sf::Color(255, 0, 0));
      paddleShape.AddPoint(xpos, ypos + 50 + yadder,  sf::Color(255, 255, 125),     sf::Color(0, 255, 0));
      paddleShape.AddPoint(xpos + 10, ypos + 50 + yadder,  sf::Color(255, 125, 255),     sf::Color(0, 0, 255));
      paddleShape.AddPoint(xpos + 10, ypos + yadder,  sf::Color(255, 0, 0),     sf::Color(0, 0, 0));
    }

    float returnXPos()
    {
      return xpos + 10;
    }
    float returnYPos()
    {
      return (ypos + yadder);
    }


    sf::Shape paddleShape;

  private:

    float xpos;
    float ypos;

    float xadder;
    float yadder;
    float acceloration;


};




class Ball
{

  public:

    sf::Shape ballShape;


    Ball(int xStart, int yStart, bool xMove, bool yMove)
    {
      xpos = xStart;
      ypos = yStart;
      xRight = xMove;
      yDown = yMove;
      xadder = 0;
      yadder = 0;
      speed = 1.05;
      defaultSpeed = speed;
      toggler = true;

      ballShape = sf::Shape::Circle(xpos,ypos,15,sf::Color(255, 255, 255),1.f);
    }

    void Move()
    {
     // cout << "X is " << xpos << " and Y is " << ypos << endl;

      if(xRight)
      {
	xpos += speed;
      }
      else
      {
	xpos -= speed;
      }
      if(yDown)
      {
	ypos += speed;
      }
      else
      {
	ypos -= speed;
      }
      ballShape.SetX(xpos);
      ballShape.SetY(ypos);

    }

    float getXPos()
    {
      return xpos;
    }

    float getYPos()
    {
      return ypos;
    }

    void bounceX()
    {
      if(xRight)
	xRight = false;
      else
	xRight = true;
    }

    void bounceY()
    {
      if(yDown)
	yDown = false;
      else
	yDown = true;
    }

    void setX(int position)
    {
      xpos = position;

    }

    void setY(int position)
    {
      ypos = position;

    }

    void reset()
    {
      xpos = 200;
      ypos = 150;
      this->bounceX();
      this->bounceY();
      speed = defaultSpeed;
      for(int i = 0; i < 40; i++)
	  ballShape.SetPointColor(i, sf::Color::White);
    }

    void increaseSpeed(float accel)
    {
      speed += accel;
    }

    void toggle()
    {
      if(toggler)
      {
	for(int i = 0; i < 40; i++)
	  ballShape.SetPointColor(i, sf::Color::Green);
	toggler = false;
      }
      else
      {
	for(int i = 0; i < 40; i++)
	  ballShape.SetPointColor(i, sf::Color::Red);
	toggler = true;
      }
    }

    float defaultSpeed;

  private:

    float speed;
    bool xRight;
    bool yDown;
    float xpos;
    float ypos;
    float xadder;
    float yadder;
    bool toggler;

};











int main(int argc, char** argv)
{
  int x = 800, y = 600;

  sf::RenderWindow App(sf::VideoMode(x, y, 32), "SFML Window");
  App.SetFramerateLimit(90);
  App.Create(sf::VideoMode(x, y, 32), "SFML Window", sf::Style::Resize|sf::Style::Close);


  //we create an event for...handling events
  sf::Event Event;

  //we grab our window's default input events
  const sf::Input& Input = App.GetInput();

  bool UpKeyDown = false;
  bool DownKeyDown = false;

  float acceloration = 0;

  Paddle* player1 = new Paddle(30,30);
  Paddle* player2 = new Paddle(750,30);
  Ball* ball = new Ball(200,150,false,false);

  sf::String title("Simple Pong", sf::Font::GetDefaultFont(), 30);
  title.Move(250,50);

  sf::String players("Player 1        Player 2", sf::Font::GetDefaultFont(), 20);
  players.Move(220,90);

  sf::String score1("0", sf::Font::GetDefaultFont(), 10);
  score1.Move(220,120);
  sf::String score2("0", sf::Font::GetDefaultFont(), 10);
  score2.Move(320,120);

  while(Running)
  {
    App.GetEvent(Event);
    if(Event.Type == sf::Event::Closed)
      Running = false;

    App.Clear(sf::Color(0,0,0));


    if(Input.IsKeyDown(sf::Key::Up))
    {
      player1->move(0);
 //     player2->move(0);
    }
    else if(Input.IsKeyDown(sf::Key::Down))
    {
      player1->move(1);
   //   player2->move(1);
    }
    else
    {
      player1->move(2);
   //   player2->move(2);
    }

    ball->Move();
    float ballX = ball->getXPos();
    float ballY = ball->getYPos();

    float p1x = player1->returnXPos() - 150;
    float p1y = player1->returnYPos() - 150;
    float p2x = player2->returnXPos() - 150;
    float p2y = player2->returnYPos() -150;

    int ballDirection;
    if(ballY > p2y + 40)
      ballDirection = 1;
    else if(ballY < p2y + 10)
      ballDirection = 0;
    else
      ballDirection = 2;

    player2->aimove(ballDirection);


    ///////
    ////
    ///
    ////
    /// if the ball hits a paddle


    if(ballX <= p1x - 35)
    {
      if(ballY + 15 >= p1y && ballY <= p1y + 50)
      {
	ball->bounceX();
	if(ballY + 15 < p1y + 25)
	{
	  ball->increaseSpeed(0.4);
	  ball->bounceY();
	}
	else
	  ball->increaseSpeed(0.2);
	ball->setX(ballX);
      }
      ball->toggle();
    }
    else if(ballX >= p2x - 70)
    {
      if(ballY + 15 >= p2y && ballY <= p2y + 50)
      {
	ball->bounceX();
	if(ballY + 15 < p2y + 25)
	{
	  ball->increaseSpeed(0.4);
	  ball->bounceY();
	}
	else
	  ball->increaseSpeed(0.2);
	ball->setX(ballX);

      }
      ball->toggle();
    }

///
////
/////
/// IF we score...

    if(ballX < -200)
    {
      ball->setX(-200);
      ball->reset();
      player2Score += 1;
      string p2score;
      stringstream out;
      out << player2Score;
      p2score = out.str();
      score2.SetText(p2score);

      for(int i =0; i < 20; i++)
      {
	score2.Scale(1.1,1.1);
	score2.Rotate(-15);
	App.Draw(score2);
	App.Display();
	App.Clear();
	sf::Sleep(0.01);
      }
      for(int i =0; i < 20; i++)
      {
	score2.Scale(0.9,0.9);
	score2.Rotate(15);
	App.Draw(score2);
	App.Display();
	App.Clear();
	sf::Sleep(0.01);
      }

      score2.SetSize(11);
      score2.SetScale(1,1);
    }
    else if(ballX > 600)
    {
      ball->setX(600);
      ball->reset();
      player1Score += 1;
      string p1score;
      stringstream out;
      out << player1Score;
      p1score = out.str();
      score1.SetText(p1score);


      for(int i =0; i < 30; i++)
      {
	score1.Scale(1.1,1.1);
	score1.Rotate(15);
	App.Draw(score1);
	App.Display();
	App.Clear();
	sf::Sleep(0.01);
      }
      for(int i =0; i < 30; i++)
      {
	score1.Scale(0.9,0.9);
	score1.Rotate(-15);
	App.Draw(score1);
	App.Display();
	App.Clear();
	sf::Sleep(0.01);
      }
      score1.SetSize(11);
      score1.SetScale(1,1);
    }

    if(ballY < -150)
    {
      ball->bounceY();
      ball->setY(-150);
    }
    else if(ballY > 440)
    {
      ball->bounceY();
      ball->setY(440);
    }

    App.Draw(ball->ballShape);

    App.Draw(player1->paddleShape);
    App.Draw(player2->paddleShape);

    App.Draw(title);
    App.Draw(players);
    App.Draw(score1);
    App.Draw(score2);

    App.Display();

  }
  delete ball;
  delete player1;
  delete player2;

  return EXIT_SUCCESS;
}
