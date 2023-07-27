//made by Manas R. Makde
#include<dos.h>
#include<iostream.h>
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<fstream.h>
#include<string.h>
#include<math.h>

union REGS in,out;
int k[]={0,15,8,7,4,1,6,2,5,9,13,3,12,11,14,10};//   mechanism for storage                       detailed explaination
class cor//pixels                  //               -----------------------                      ---------------------
{                                  //      (dot=0 for pixel & dot=1 for other shape)  |  i stored info about a shape/pixel into binary file and i did it in such a manner that is saves maximum storage let me show via diagram
public:                            //                         value  figure           |            ----------------------------------------------------------
long int x,y,color;//details       //                      --> 1     line             |          |           < visual representation binary_file.dat>        |
};                 //of pixel      //  (for other figure) |                           |          |             ------------------------------------          |
class other_shape                 //other_shape.info[0] = |--> 2     circle           |          | 0{12 13 2} , 0{-1,-1} , 1{2 0 13 13 10}  & so on...       |
{       // class to store shape    //                     |                           |          | ^              ^        ^
public: // which are not pixels    //                      --> 3     rectangle        |          | |              |        |__ 1 represents other_shape and the 2 represents the circle WITHIN the category
				   //_____________________________________________    |          | |              |            of other_shapes the other values like 0,13,13,10 represents color,x-cordinates
long int info[10];                 //for figure: line         circle      rectangle   |          | |              |            ,y-cor,radius respectively    |
				   //_____________________________________________    |          | |              |                                          |
};                                 // .info[1] = color        color         color     |          | |              |                                          |
void show_mouse()                  // .info[2] = start_x      start_x       start_x   |          | |              |                                          |
{                                  // .info[3] = start_y      start_y       start_y   |          | |              |                                          |
in.x.ax=1;                         // .info[4] = end_x        radi          end_x     |          | |              |
 int86(0x33,&in,&out);             // .info[5] = end_y          -           end_y     |          | |              |________this indicates a break in a flow of continuous pixels
}                                  //(input into the corresponding shape function respectively)  | |
void hide_mouse()                  //                                                            | |___initially the compiler checks if the given is a pixel/cor or other_shape
{                                  //                                                            |     since it is 0 here it checks only for the color,x-cor and y-cor
in.x.ax=2;                         //                                                            |     the class(and the class info is represented by {} )   |
 int86(0x33,&in,&out);             //             <||O||>                                        |___________________________________________________________|
}                                  //
void reset_mouse()                 //
{  //puts mouse back to it's       //
in.x.ax=0;//default position       // why did i do what i did (seperating pixels and other_shapes?
 int86(0x33,&in,&out);             // ____________________________________________________________
}                                  //
int mouse_stat()                   // well it might seem confusing why i would put shapes and pixels in diff.
{ //returns if the mouse is clicked// categories but think about it, if i had kept them both in other_shapes
in.x.ax=3;                         // then every pixel would have taken up a lot more space than required
int86(0x33,&in,&out);              // hence to cut the storage significantly i
return out.x.bx;                   // divided these into 0 as pixel and 1 as shape
}                                  //
int mouse_x()                      // (hey, i did what i could at the time, message me if you got something better)
{
in.x.ax=3;
int86(0x33,&in,&out);
return out.x.cx;//returns x-cor of mouse
}
int mouse_y()
{
in.x.ax=3;
int86(0x33,&in,&out);
return out.x.dx;//returns y-cor of mouse

}
void mouse_cor(cor &c)
{
 in.x.ax=3;        //sets x and y cordinates into c
 int86(0x33,&in,&out);
 c.x=out.x.cx;
 c.y=out.x.dx;
}
void rect(long int x_start,long int y_start,long int x_end,long int y_end)
{  //given starting cor and ending cor ,it draws a rectangle
 line(x_start,y_start,x_end,y_start);//top line
 line(x_end,y_start,x_end,y_end);//right line
 line(x_end,y_end,x_start,y_end);//bottom line
 line(x_start,y_end,x_start,y_start);//left line
}
void rect_fill(int x_start,int y_start,int x_end,int y_end,int color=WHITE)
{ //fills up a rectangle with input color (almost same mech as rect() fucntion)
setcolor(color);
 for(int i=x_start+1;i<x_end;i++)
 line(i,y_start+1,i,y_end-1);
 setcolor(BLACK);
}
void shape_box(int shape,int selected_color,int start_x,int start_y,int end_x,int end_y,int gap)
{ //draws the box which displays currently selected shape

 int block_b=((end_x-start_x-gap*3)/2)/1.5;
 int i=9;
 rect(start_x+gap,start_y+gap*i+(block_b*(i-1)),end_x-gap,end_y-gap);//the actual shape box
 rect_fill(start_x+gap,start_y+gap*i+(block_b*(i-1)),end_x-gap,end_y-gap,selected_color);//& color inside it

 if(selected_color==BLACK)//if the currently selected color is black then the
 setcolor(WHITE);//shape in it is drawn white so that it shouldnt blend into the background color

 if(shape==0) //free hand shape
 {
  line(start_x+gap*3,start_y+gap*(i+2)+(block_b*(i-1)),end_x-gap*3,start_y+gap*(i+1)+(block_b*(i-1)));
  line(start_x+gap*3,start_y+gap*(i+2)+(block_b*(i-1)),end_x-gap*4,start_y+gap*(i+3)+(block_b*(i-1)));
  circle(end_x-gap*4,start_y+gap*(i+4)+(block_b*(i-1)),gap);
 }
 else
 if(shape==1) //line shape
  line(start_x+gap*2,start_y+gap*(i+1)+(block_b*(i-1)),end_x-gap*2,end_y-gap*2);

 else
 if(shape==2) //cirlce shape
 circle(((end_x-gap)+(start_x+gap))/2,((end_y-gap)+(start_y+gap*i+(block_b*(i-1))))/2,gap*2);//cricle display shape

 else
 if(shape==3) //rectangle shape
 rect(start_x+gap*2,start_y+gap*(i+2)+(block_b*(i-1)),end_x-gap*2,end_y-gap*3);
 setcolor(BLACK);


}
void default_interface(int shape,int selected_color,int open,int start_x=539,int start_y=42,int end_x=getmaxx(),int end_y=386,int gap=10)
{
//i took all these inputs so that even if the screen changes the code should work...(should)
 int block_l,block_b,temp,j=0;
 temp=start_y;

 block_l=(end_x-start_x-gap*3)/2;
 block_b=block_l/1.5;       //something i initalized idk i don't remember


if(open==1) //if the eye icon is not hit i.e. tool bar can be seen
{
setcolor(BLACK);

 rect(start_x,start_y,end_x,end_y); //entire box which contains colors and shape-box
 rect_fill(start_x,start_y,end_x,end_y,WHITE);

 for(int i=1;i<=8;i++)  //all the color boxes
 {
  rect(start_x+gap,start_y+gap*i+(block_b*(i-1)),start_x+block_l+gap,start_y+(gap*i)+(block_b*i));//creates the outline of left boxes
  rect_fill(start_x+gap,start_y+gap*i+(block_b*(i-1)),start_x+block_l+gap,start_y+(gap*i)+(block_b*i),k[j++]);//fills that outline with corresponding color

  rect(start_x+block_l+gap*2,start_y+gap*i+(block_b*(i-1)),start_x+block_l*2+gap*2,start_y+(gap*i)+(block_b*i));//creates the outline of the right boxes
  rect_fill(start_x+block_l+gap*2,start_y+gap*i+(block_b*(i-1)),start_x+block_l*2+gap*2,start_y+(gap*i)+(block_b*i),k[j++]);//fills that outline with corresponding color
 }
 shape_box(shape,selected_color,start_x,start_y,end_x,end_y,gap);//that shape box i initalized before remember(i don't)?

 start_y=end_y;
 end_y=getmaxy();
 block_b=((getmaxy()-start_y)-3*gap)/2;
  for(i=1;i<=2;i++) //bottom 4 buttons
 {
  if(open==1)
   rect(start_x+gap,start_y+gap*i+(block_b*(i-1)),start_x+block_l+gap,start_y+(gap*i)+(block_b*i));
  rect_fill(start_x+gap,start_y+gap*i+(block_b*(i-1)),start_x+block_l+gap,start_y+(gap*i)+(block_b*i));
  if(!(i==1&&open==0))
  {
  rect(start_x+block_l+gap*2,start_y+gap*i+(block_b*(i-1)),start_x+block_l*2+gap*2,start_y+(gap*i)+(block_b*i));
  rect_fill(start_x+block_l+gap*2,start_y+gap*i+(block_b*(i-1)),start_x+block_l*2+gap*2,start_y+(gap*i)+(block_b*i));
  }
  if(i==1)
  {
   rect(554,406,579,423); //file icon
  rect_fill(554,406,579,423,YELLOW);
  rect(568,403,579,406);
  rect_fill(568,403,579,406,YELLOW);

  //this is that small circle button i made, which does nothing but i like to keep people in suspense xD
  circle(((start_x+block_l+gap*2)+(start_x+block_l*2+gap*2))/2,((start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)))/2,5);
  }
  else
  {
  if(open==1)
  {
      //eye icon  (i'm too tired to label this go figure...)

      //open eye
      setcolor(WHITE);
  line(start_x+block_l+gap*2+1,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2,start_x+block_l*2+gap*2-1,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2);
      setcolor(BLACK);
   circle(((start_x+block_l+gap*2)+(start_x+block_l*2+gap*2))/2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2,gap/3);
arc(((start_x+block_l+gap*2)+(start_x+block_l*2+gap*2))/2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2,190,350,gap); //lowwer
arc(((start_x+block_l+gap*2)+(start_x+block_l*2+gap*2))/2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2 ,10,170,gap);//upper*/
  }
  else //closed eye
  {
   circle(((start_x+block_l+gap*2)+(start_x+block_l*2+gap*2))/2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2,gap/3);
arc(((start_x+block_l+gap*2)+(start_x+block_l*2+gap*2))/2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2,190,350,gap); //lowwer
arc(((start_x+block_l+gap*2)+(start_x+block_l*2+gap*2))/2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2 ,10,170,gap);//upper
line(start_x+block_l+gap*2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2,start_x+block_l*2+gap*2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2);
  }
  line( start_x+gap*2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2,start_x+block_l,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2); //plus sign
  line(( (start_x+gap)+(start_x+block_l+gap) )/2,start_y+gap*(i+1)+(block_b*(i-1)),( (start_x+gap)+(start_x+block_l+gap) )/2,start_y+(gap*(i-1))+(block_b*i) );
  }
 }
  start_y=0;
  end_y=temp;               // ...uuh no clue
  block_b=(end_y-start_y)/2;

  rect(start_x+gap,start_y+gap,start_x+block_l+gap,start_y+gap+block_b); //top option box 1 (import option)

  for(int k2=start_x+gap+1;k2<start_x+block_l+gap;k2++)        //the filled color in the import box
  for(int k1=start_y+gap+1;k1<start_y+gap+block_b;k1++)
  putpixel(k2,k1,((k2+k1)%2==0?BROWN:RED));//cool effect if you ask me


  //again too tired to explain so DIY...
  line(start_x+gap*1.5,start_y+gap*1.5 ,start_x+block_l+gap/2,start_y+gap*1.5);  //triangle
  line(start_x+gap*1.5,start_y+gap*1.5,( (start_x+gap*1.5)+(start_x+block_l+gap/2) )/2,start_y+block_b+gap/2 );
  line(start_x+block_l+gap/2,start_y+gap*1.5,( (start_x+gap*1.5)+(start_x+block_l+gap/2) )/2,start_y+block_b+gap/2 );

  line(start_x+gap*1.5+1,start_y+gap*1.5 ,start_x+block_l+gap/2-1,start_y+gap*1.5);
  line(start_x+gap*1.5+1,start_y+gap*1.5,( (start_x+gap*1.5+1)+(start_x+block_l+gap/2-1) )/2,start_y+block_b+gap/2-1 );
  line(start_x+block_l+gap/2-1,start_y+gap*1.5,( (start_x+gap*1.5+1)+(start_x+block_l+gap/2-1) )/2,start_y+block_b+gap/2-1 );


  rect(start_x+block_l+gap*2,start_y+gap,start_x+block_l*2+gap*2,start_y+gap+block_b); //top option box 2 (close option)

  for(k2=start_x+block_l+gap*2+1;k2<start_x+block_l*2+gap*2;k2++)        //color of the close option
  for(k1=start_y+gap+1;k1<start_y+gap+block_b;k1++)
  putpixel(k2,k1,((k2+k1)%2==0?DARKGRAY:DARKGRAY)); //another cool effect


  //...
  line(start_x+block_l+gap*2.5,start_y+gap*1.5,start_x+block_l*2+gap*1.5,start_y+gap*0.5+block_b);
  line(start_x+block_l+gap*2.5,start_y+gap*0.5+block_b,start_x+block_l*2+gap*1.5,start_y+gap*1.5);

  line(start_x+block_l+gap*2.5+1,start_y+gap*1.5,start_x+block_l*2+gap*1.5-1,start_y+gap*0.5+block_b);
  line(start_x+block_l+gap*2.5+1,start_y+gap*0.5+block_b,start_x+block_l*2+gap*1.5-1,start_y+gap*1.5);
gotoxy(1,1);
cout<<"?";
}
else   //to hide everthing in the tool box and button except for the eye icon
{
setcolor(WHITE);

rect(start_x,start_y,end_x,end_y); //entire box (white)
 for(int i=1;i<=8;i++)  //white boxes to cover up the old color boxes
 {
  rect(start_x+gap,start_y+gap*i+(block_b*(i-1)),start_x+block_l+gap,start_y+(gap*i)+(block_b*i));
  rect_fill(start_x+gap,start_y+gap*i+(block_b*(i-1)),start_x+block_l+gap,start_y+(gap*i)+(block_b*i),WHITE);
  setcolor(WHITE);
  rect(start_x+block_l+gap*2,start_y+gap*i+(block_b*(i-1)),start_x+block_l*2+gap*2,start_y+(gap*i)+(block_b*i));
  rect_fill(start_x+block_l+gap*2,start_y+gap*i+(block_b*(i-1)),start_x+block_l*2+gap*2,start_y+(gap*i)+(block_b*i),WHITE);
  setcolor(WHITE);
 }

  rect(start_x+gap,start_y+gap*i+(block_b*(i-1)),end_x-gap,end_y-gap);//shape box (white)
 rect_fill(start_x+gap,start_y+gap*i+(block_b*(i-1)),end_x-gap,end_y-gap,WHITE);//& color inside

  //hiding the shapes by outlines the same shapes but in white

  setcolor(WHITE);
  if(shape==0) //free hand shape
 {
  line(start_x+gap*3,start_y+gap*(i+2)+(block_b*(i-1)),end_x-gap*3,start_y+gap*(i+1)+(block_b*(i-1)));
  line(start_x+gap*3,start_y+gap*(i+2)+(block_b*(i-1)),end_x-gap*4,start_y+gap*(i+3)+(block_b*(i-1)));
  circle(end_x-gap*4,start_y+gap*(i+4)+(block_b*(i-1)),gap);
 }

 else
 if(shape==1)// line shape (white)
  line(start_x+gap*2,start_y+gap*(i+1)+(block_b*(i-1)),end_x-gap*2,end_y-gap*2);

 else
 if(shape==2) //circle shape (white)
 circle(((end_x-gap)+(start_x+gap))/2,((end_y-gap)+(start_y+gap*i+(block_b*(i-1))))/2,gap*2);//cricle display shape

 else
 if(shape==3) //rectangle shape(white)
 rect(start_x+gap*2,start_y+gap*(i+2)+(block_b*(i-1)),end_x-gap*2,end_y-gap*3);

  start_y=end_y;
 end_y=getmaxy();
 block_b=((getmaxy()-start_y)-3*gap)/2;
  for(i=1;i<=2;i++) //bottom 4 buttons (white)
 {
  if(!(i==1&&open==0))//second box same row (white)
  {
  setcolor(BLACK);
  rect(start_x+block_l+gap*2,start_y+gap*i+(block_b*(i-1)),start_x+block_l*2+gap*2,start_y+(gap*i)+(block_b*i));
  setcolor(WHITE);
  }
  else
  rect(start_x+block_l+gap*2,start_y+gap*i+(block_b*(i-1)),start_x+block_l*2+gap*2,start_y+(gap*i)+(block_b*i));

   rect(start_x+gap,start_y+gap*i+(block_b*(i-1)),start_x+block_l+gap,start_y+(gap*i)+(block_b*i));

  if(i==1)
  {
   rect(554,406,579,423); //file icon (white)
  rect_fill(554,406,579,423,WHITE);
  setcolor(WHITE);        //i set the file icon to custom values so it you change
  rect(568,403,579,406);  //the other values you might wanna consider changing this manuelly too
  rect_fill(568,403,579,406,WHITE);
  setcolor(WHITE);

  //that weird circle that does nothing xD
  circle(((start_x+block_l+gap*2)+(start_x+block_l*2+gap*2))/2,((start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)))/2,5);
  }
  else
  {
  //eye icon but with a line to it so that it looks close (yeah not my most inovative moment)

  setcolor(BLACK);
   circle(((start_x+block_l+gap*2)+(start_x+block_l*2+gap*2))/2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2,gap/3);
   arc(((start_x+block_l+gap*2)+(start_x+block_l*2+gap*2))/2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2,190,350,gap); //lowwer
   arc(((start_x+block_l+gap*2)+(start_x+block_l*2+gap*2))/2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2 ,10,170,gap);//upper
   line(start_x+block_l+gap*2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2,start_x+block_l*2+gap*2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2);

setcolor(WHITE);
  line( start_x+gap*2,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2,start_x+block_l,( (start_y+gap*i+(block_b*(i-1)))+(start_y+(gap*i)+(block_b*i)) )/2); //plus sign
  line(( (start_x+gap)+(start_x+block_l+gap) )/2,start_y+gap*(i+1)+(block_b*(i-1)),( (start_x+gap)+(start_x+block_l+gap) )/2,start_y+(gap*(i-1))+(block_b*i) );
  }
 }
 setcolor(WHITE);

   start_y=0;
  end_y=temp;
  block_b=(end_y-start_y)/2;

  rect(start_x+gap,start_y+gap,start_x+block_l+gap,start_y+gap+block_b); //top option box 1 (import option)(white)

  rect_fill(start_x+gap,start_y+gap,start_x+block_l+gap,start_y+gap+block_b,WHITE);
  setcolor(WHITE);

  rect(start_x+block_l+gap*2,start_y+gap,start_x+block_l*2+gap*2,start_y+gap+block_b); //top option box 2 (close option) (white)

  rect_fill(start_x+block_l+gap*2,start_y+gap,start_x+block_l*2+gap*2,start_y+gap+block_b,WHITE);
  setcolor(WHITE);
 rect_fill(-1,-1,18,16,WHITE);
}
}
int clicked_in_box(int start_x,int start_y,int end_x,int end_y) //to check if the  mouse clicked in a box
{
    if(mouse_stat()!=0&&start_x<=mouse_x()&&end_x>=mouse_x()&&start_y<=mouse_y()&&end_y>=mouse_y())
    return 1;

   else
   return 0;

}
int in_box(int start_x,int start_y,int end_x,int end_y)//to check is the mouse is inside the tool box
{
    if(start_x<=mouse_x()&&end_x>=mouse_x()&&start_y<=mouse_y()&&end_y>=mouse_y())
    return 1;

   else
   return 0;

}
int button_clicked(int start_x=539,int start_y=42,int end_x=getmaxx(),int end_y=386,int gap=10)
{          //returns which button is clicked as each button has it's own number
   int block_l,block_b,temp;
 temp=start_y;

 block_l=(end_x-start_x-gap*3)/2;
 block_b=block_l/1.5;


 for(int i=1;i<=8;i++)//if the mouse clicks in a color box it returns the corresponding number
 {
  if(clicked_in_box(start_x+gap,start_y+gap*i+(block_b*(i-1)),start_x+block_l+gap,start_y+(gap*i)+(block_b*i))==1)
  return 2*i+1;
  else if(clicked_in_box(start_x+block_l+gap*2,start_y+gap*i+(block_b*(i-1)),start_x+block_l*2+gap*2,start_y+(gap*i)+(block_b*i))==1)
  return 2*(i+1);
 }
  if(clicked_in_box(start_x+gap,start_y+gap*i+(block_b*(i-1)),end_x-gap,end_y-gap)==1)//clicked in shape box
   return 2*i+1;

   start_y=end_y;
 end_y=getmaxy();
 block_b=((getmaxy()-start_y)-3*gap)/2;
   for(i=1;i<=2;i++) //selection of any of the bottom 4 button
 {
   if(clicked_in_box(start_x+gap,start_y+gap*i+(block_b*(i-1)),start_x+block_l+gap,start_y+(gap*i)+(block_b*i))==1)
    return 19+2*i-1;
  if(clicked_in_box(start_x+block_l+gap*2,start_y+gap*i+(block_b*(i-1)),start_x+block_l*2+gap*2,start_y+(gap*i)+(block_b*i))==1)
    return 19+2*i;
  }

   start_y=0;
  end_y=temp;
  block_b=(end_y-start_y)/2;

  if(clicked_in_box(start_x+gap,start_y+gap,start_x+block_l+gap,start_y+gap+block_b)==1) //selected top option box 1 (import option)
     return 1;

  else if(clicked_in_box(start_x+block_l+gap*2,start_y+gap,start_x+block_l*2+gap*2,start_y+gap+block_b)==1) //selected top option box 2 (close option)
     return 2;
     if(clicked_in_box(0,0,7,15)==1)
     return 0; //quesion mark button

  return -1;
}
int in_button(int open,int start_x=539,int start_y=42,int end_x=getmaxx(),int end_y=386,int gap=10)
{ //to check if the mouse is inside any box and if it is return the corresponding box number
   int block_l,block_b,temp;
 temp=start_y;

 block_l=(end_x-start_x-gap*3)/2;
 block_b=block_l/1.5;

 if(in_box(start_x,start_y,end_x,end_y)==1&&open==1)
 {

 for(int i=1;i<=8;i++)  //mouse inside a color box
 {
  if(in_box(start_x+gap,start_y+gap*i+(block_b*(i-1)),start_x+block_l+gap,start_y+(gap*i)+(block_b*i))==1)
  return 2*i+1;
  else if(in_box(start_x+block_l+gap*2,start_y+gap*i+(block_b*(i-1)),start_x+block_l*2+gap*2,start_y+(gap*i)+(block_b*i))==1)
  return 2*(i+1);
 }
  if(in_box(start_x+gap,start_y+gap*i+(block_b*(i-1)),end_x-gap,end_y-gap)==1)//selection of shape box
   return 2*i+1;
  return 24;
 }

 start_y=end_y;
 end_y=getmaxy();
 block_b=((getmaxy()-start_y)-3*gap)/2;
   for(int i=1;i<=2;i++) //mouse inside any  bottom 4 button
 {
   if(open==1&&in_box(start_x+gap,start_y+gap*i+(block_b*(i-1)),start_x+block_l+gap,start_y+(gap*i)+(block_b*i))==1)
    return 19+2*i-1;  //left
  if(in_box(start_x+block_l+gap*2,start_y+gap*i+(block_b*(i-1)),start_x+block_l*2+gap*2,start_y+(gap*i)+(block_b*i))==1&&(open==1||(open==0&&i==2)))    return 19+2*i;    //right
  }

   start_y=0;
  end_y=temp;
  block_b=(end_y-start_y)/2;

  if(open==1)
  {
  if(in_box(start_x+gap,start_y+gap,start_x+block_l+gap,start_y+gap+block_b)==1) //selected top option box 1 (import option)
     return 1;

  else if(in_box(start_x+block_l+gap*2,start_y+gap,start_x+block_l*2+gap*2,start_y+gap+block_b)==1) //selected top option box 2 (close option)
     return 2;
  else if(in_box(0,0,7,15)==1) //mouse inside question mark box
     return 0;
  }
  return -1;
}

int file_empty(char file_name[100]) //to check "is the file_empty"? and return 1 as "yes" and 0 as "no"
{
fstream f;
f.open(file_name,ios::binary|ios::in);
f.seekg(0,ios::end);
if(f.tellg()==0)//file is empty
return 1;
else
return 0;
}

void lifted() //loop so that the same button oesnt get clicked multiple times
{
 while(mouse_stat()!=0);
}

void store(char file_name[100],cor c,other_shape fig,int dot)
{
fstream f;

if(dot==0)//if to store a pixel
{
f.open(file_name,ios::in|ios::binary);
int d,i=0;
cor c1,c2;
other_shape fig_temp;
while(f.read((char*)&d,sizeof(int)))
 {
c2.x=c1.x;
c2.y=c1.y;

if(d==0)
f.read((char*)&c1,sizeof(cor));
else
  {
f.read((char*)&fig_temp,sizeof(other_shape));
i=0;
  }
i++;
 }

if(c2.x==mouse_x()&&c2.y==mouse_y()&&i>2&&d==0)
goto skip;
f.close();

f.open(file_name,ios::binary|ios::app);
f.seekp(0,ios::end);
f.write((char*)&dot,sizeof(int));
f.write((char*)&c,sizeof(cor));
f.close();
}
skip:;
f.close();

if(dot==1)  //if to store any other_shape
{
f.open(file_name,ios::binary|ios::app);
f.seekp(0,ios::end);
f.write((char*)&dot,sizeof(int));
f.write((char*)&fig,sizeof(other_shape));
f.close();
}

}
void show(char file_name[100]) //prints all the currently stored shapes
{
fstream f;
int dot;
other_shape fig;
cor c1,c2;
f.open(file_name,ios::binary|ios::in);
while( f.read((char*)&dot,sizeof(int)) )
 {
  if(dot==0)
  {
   f.read((char*)&c1,sizeof(cor));

   while(f.read((char*)&dot,sizeof(int)))
   {
    f.read((char*)&c2,sizeof(cor));
    if(c2.x!=-1&&c1.x!=-1)
    {
    setcolor(c1.color);
    line(c1.x,c1.y,c2.x,c2.y);
    c1.x=c2.x;
    c1.y=c2.y;
    }
    else
    break;
   }
  }
  if(dot==1)
  {
  f.read((char*)&fig,sizeof(other_shape));
  if(fig.info[0]==1)
   {
   setcolor(fig.info[1]);
   line(fig.info[2],fig.info[3],fig.info[4],fig.info[5]);
   }

   if(fig.info[0]==2)
   {
     setcolor(fig.info[1]);
     circle(fig.info[2],fig.info[3],fig.info[4]);
   }
   if(fig.info[0]==3)
   {
   setcolor(fig.info[1]);
   rect(fig.info[2],fig.info[3],fig.info[4],fig.info[5]);
   }
  }

 }
 f.close();

}
void end(char file_name[100])
{  //to break the continious flow of pixels
 fstream f;
 cor c;
 int dot=0;
 f.open(file_name,ios::binary|ios::app);
 f.seekp(0,ios::end);
 c.x=-1;
 c.y=-1;
 c.color=-1;
 f.write((char*)&dot,sizeof(int));
 f.write((char*)&c,sizeof(cor));
 f.close();
}

int change(int x,int y)//to check if x and y inputs are as same
{                      //as the current "x" and "y" cor of mouse
in.x.ax=3;
int86(0x33,&in,&out);
 if(out.x.cx!=x)  //cor "changed?"
 return 1;//"yes"
 else if(out.x.dx!=y)
 return 1;//"yes"
 return 0;//"no"
}

int draw(char file_name[100],int open,int shape,int selected_color,int start_x,int start_y,int end_x,int end_y)
{ //this is the big deal, this function is responsible for actually drawing
 cor c1,c2;//shapes from mouse
 other_shape fig;
 if(shape==0&&in_button(open)==-1&&mouse_stat()!=0)//to draw free hand/pixels
 {
  if(open==1)//to hide toolbox if open so that you don't draw over the  tools box
   {
    default_interface(shape,selected_color,0);
  show(file_name);
   }
   mouse_cor(c2);
   mouse_cor(c1);
   setcolor(selected_color);
  while(mouse_stat()!=0)//basic algorithm go figure
   {
   hide_mouse();
   line(c1.x,c1.y,c2.x,c2.y);
   show_mouse();
   c1.x=c2.x;
   c1.y=c2.y;
    c2.color=selected_color;
   mouse_cor(c2);
    c2.color=selected_color;
    store(file_name,c2,fig,0);

   }
   end(file_name);
   hide_mouse();
 show(file_name);
  if(open==1)
   default_interface(shape,selected_color,1);
   show_mouse();
   return 1;
 }
else
if(shape==1&&in_button(open)==-1)   //to draw line
{
 int x=mouse_x(),y=mouse_y(),cl=0,n=0,s=1;
 long int lines[10];

//before you read this segment of algorithm
//keep in mind that this is an old algorithm and
//the compiler keeped glitching due to some random error
//so it might not look like the most efficient algorithm but it get's the job done
//so keep in mind i COULD do better and you COULD modify this but i highly
//recommend you do NOT as it took me hours to config. (idk maybe compiler's broken or something)

////////////////////////current me explaining stops here//////////////////////////

while(shape==1&&(cl!=0?1:in_button(open)==-1?1:0)==1)
{
 while(change(x,y)==0&&cl!=0&&n==2)//to show line when mouse is held
  {
 setcolor(selected_color);
  line(lines[0],lines[1],x,y);
  cl=mouse_stat();
  }

 while(in_button(open)==-1&&change(x,y)==0&&cl==0&&n==0)//when mouse is not active
  {
 cl=mouse_stat();
  }

 do //makes sure that the mouse button is only pressed once/has been released
 s=mouse_stat();
 while(change(x,y)==0&&s!=0&&n==0);

 if(cl!=0&&n==2)//if button held and mouse position changed it sets a new line
 {              //and erases the old one
 hide_mouse();
  setcolor(WHITE);
  line(lines[0],lines[1],x,y);
  setcolor(selected_color);
show(file_name);
  show_mouse();
 }
 else if(cl==0&&n==2) //inserts the end point of the line into the list when released
 {                    //and increases the no. of existing lines
  n=0;
  cl=0;
  fig.info[0]=1;
  fig.info[1]=selected_color;
   fig.info[2]=lines[0];
   fig.info[3]=lines[1];
   fig.info[4]=mouse_x();
   fig.info[5]=mouse_y();
   store(file_name,c1,fig,1);

     lines[0]=mouse_x();
  lines[1]=mouse_y();
    if(open==1)
   default_interface(shape,selected_color,1);
 }
 else if(cl!=0&&n==0) //inserts the start point of the line into the list when clicked newly
  {
   lines[0]=mouse_x();
  lines[1]=mouse_y();
  n=2;
  cl=1;
  if(open==1)
   {
    default_interface(shape,selected_color,0);
show(file_name);
   }
  }
  x=mouse_x();
  y=mouse_y();

 }
}
else
if(shape==2&&in_button(open)==-1)   //to draw circle using mouse
{

int i=0,n=0;
int cl=0;
long float radi=0,x,y;
long float circles[10];


while(shape==2&&(cl!=0?1:in_button(open)==-1?1:0)==1)   //circle condition
 {
 x=mouse_x();
 y=mouse_y();

 while(in_button(open)==-1&&n==0&&cl==0)//shows mouse when not clicked
  {
 cl=mouse_stat();
  }
 if(n==1)//sets radi of circle if mouse is clicked and held
 radi=sqrt(pow(circles[0]-x,2)+pow(circles[1]-y,2))/2;

 while(n==1&&cl!=0&&x==mouse_x()&&y==mouse_y())//shows the click and hold circles unless mouse position is changed or mouse button is let go
  {
 setcolor(selected_color);
  circle((circles[0]+x)/2,(circles[1]+y)/2,radi);
  cl=mouse_stat();
  }

 if(n==0&&cl!=0)//if mouse button is clicked and held to create a new circle
  {
  circles[0]=mouse_x();
  circles[1]=mouse_y();
  n=1;
  if(open==1)
   {
  hide_mouse();
   default_interface(shape,selected_color,0);
   show(file_name);
   show_mouse();
   }
  }

 else if(n==1&&cl!=0)//if mouse button is clicked and held but mouse position is changed
  {                   //erase old circle

 hide_mouse();
 setcolor(WHITE);
 circle((circles[0]+x)/2,(circles[1]+y)/2,radi);
 setcolor(selected_color);
 show(file_name);
 show_mouse();
  }

 else if(n==1&&cl==0)//if circle is created and mouse click is let go
  {
  circles[0]=(circles[0]+x)/2;//creating new circle
  circles[1]=(circles[1]+y)/2;
  circles[2]=radi;
  n=0;
  cl=0;
  fig.info[0]=2;
  fig.info[1]=selected_color;
  fig.info[2]=circles[0];
  fig.info[3]=circles[1];
  fig.info[4]=circles[2];
  store(file_name,c1,fig,1);

  circles[0]=mouse_x();//creating new circle
  circles[1]=mouse_y();
  circles[2]=0;

  if(open==1)
   {
  hide_mouse();
  show(file_name);
  default_interface(shape,selected_color,1);
  show_mouse();
   }

  }

 }

}

else
if(shape==3&&in_button(open)==-1)   //rectangle
{
int i=0,n=0;
int cl=0;
long float x,y,x_end,y_end;
long float rectangles[10];


while(shape==3&&(cl!=0?1:in_button(open)==-1?1:0)==1)
{
 x=mouse_x();
 y=mouse_y();

 while(in_button(open)==-1&&n==0&&cl==0)//shows mouse when not clicked and held
 cl=mouse_stat();

 while(n==1&&cl!=0&&x==mouse_x()&&y==mouse_y())//shows the click and hold rectangles unless mouse position is changed or mouse button is let go
 {
  rect(rectangles[0],rectangles[1],x,y);
  cl=mouse_stat();
 }

 if(n==0&&cl!=0)//if mouse button is clicked and held to create a new rectangle
 {
  rectangles[0]=mouse_x();
  rectangles[1]=mouse_y();
  n=1;

    if(open==1)
   {
  hide_mouse();
   default_interface(shape,selected_color,0);
   show(file_name);
   show_mouse();
   }

 }

 else if(n==1&&cl!=0)//if mouse button is clicked and held but mouse position is changed
 {
 hide_mouse();
 show(file_name);
 setcolor(WHITE);
 rect(rectangles[0],rectangles[1],x,y);
 setcolor(selected_color);
 show_mouse();
 }

 else if(n==1&&cl==0)//if rectangle is created and mouse click is let go
 {
  n=0;
  fig.info[0]=3;
  fig.info[1]=selected_color;
  fig.info[2]=rectangles[0];
  fig.info[3]=rectangles[1];
  fig.info[4]=mouse_x();
  fig.info[5]=mouse_y();
  store(file_name,c1,fig,1);

  if(open==1)
   {
  hide_mouse();
  show(file_name);
  default_interface(shape,selected_color,1);
  show_mouse();
   }


 }

}


}
 return 0;
}

/////////////and i'm back!/////////////////

int file_exist(char file_name[100])//to check if the file_name exists
{
fstream f;
f.open(file_name,ios::in|ios::binary);
if(!f)//does the "file_exist"?
return 0;//"no"
else
return 1;//"yes"
}
void create_file(char file_name[100])//to create a new file or empty an existing one
{
fstream f;
f.open(file_name,ios::out|ios::binary);
}

void fill_background(int color=WHITE)
{   //prints that fancy white background
for(int i=0;i<=getmaxx();i++)
for(int j=0;j<=getmaxy();j++)
putpixel(i,j,color);
}

int main()
{
clrscr();
int d=DETECT,shape=0,selected_color=BLACK,open=1;
int start_x=539,start_y=42,end_x=getmaxx(),end_y=386,gap=10;
char file_name[]="untitled.dat";
initgraph(&d,0,"c:\\turboc3\\bgi");
// initgraph(&d,0,"c:\\tc\\bgi");

if(file_exist(file_name)==0)
 create_file(file_name);

 cout<<"                                  loading...";

fill_background(WHITE);
show(file_name);         //fancy loading of background
default_interface(shape,selected_color,open,start_x=539,start_y=42,end_x=getmaxx(),end_y=386,gap=10);

reset_mouse();
show_mouse();

//i could have made a function of these buttons but it didn't hit me at that moment so...sry
while((button_clicked()==2&&open==1?0:1)==1)//while not clicked on the cross button
{
 if(open==1)
 {
  if(button_clicked()==1) //import button clicked
   {
     reset_mouse();
 char name[100];
 gotoxy(1,1);
 cout<<"enter file name to import=>";
 gets(name);
 rect_fill(-1,-1,500,50);
 strcat(name,".dat");
  if(file_exist(name)==0)
    {
 gotoxy(1,1);
 cout<<"no such file exists!"<<endl;
 cout<<"press any key to continue...";
 getch();

 reset_mouse();
 rect_fill(-1,-1,500,50);
 show(file_name);
 default_interface(shape,selected_color,open,start_x=539,start_y=42,end_x=getmaxx(),end_y=386,gap=10);
 show_mouse();
    }
 else
    {
  gotoxy(1,1);
  cout<<"would you like to save this file first?"<<endl;
  cout<<"press enter to save...";
  char c=getch();
 rect_fill(-1,-1,300,50);

 if(int(c)!=13)
  {
  strcpy(file_name,name);
  gotoxy(30,1);
  goto end2;
  }
 char nam[100];

 default_interface(shape,selected_color,0,start_x=539,start_y=42,end_x=getmaxx(),end_y=386,gap=10);

 gotoxy(1,1);
 cout<<"enter name to save by=>";
 gets(nam);

 strcat(nam,".dat");
 rename(file_name,nam);
 strcpy(file_name,name);

 gotoxy(20,1);
 cout<<"saving & ";
 end2:;
 cout<<"importing...";

  reset_mouse();
 fill_background(WHITE);
 show(file_name);
 default_interface(shape,selected_color,open,start_x=539,start_y=42,end_x=getmaxx(),end_y=386,gap=10);
 show_mouse();

    }
 }
 else
 if(button_clicked()==22)//clicked on the plus icon
 {
 gotoxy(1,1);
 cout<<"do you want to save first?"<<endl;
 cout<<"press enter to save..."<<endl;
 cout<<"press backspace to cancel..."<<endl;
 cout<<"press any other key to continue...";
 char c=getch();
 if(int(c)==0)
 c=getch();
 rect_fill(-1,-1,400,70);

 if(int(c)==8)//user hits backspace button
 {
 show(file_name);
 default_interface(shape,selected_color,open,start_x=539,start_y=42,end_x=getmaxx(),end_y=386,gap=10);
  goto end;
 }
 if(int(c)!=13&&int(c)!=8)//user hits any button except backspace and enter
 {
  fstream f;
  f.open(file_name,ios::out|ios::binary);
  f.close();
   show(file_name);
 default_interface(shape,selected_color,open);
 gotoxy(1,1);
 cout<<"                                    clearing...";
  goto here;
 }
 char name[100];
 default_interface(shape,selected_color,0,start_x=539,start_y=42,end_x=getmaxx(),end_y=386,gap=10);
 gotoxy(1,1);
 cout<<"enter name to save by=>";
 gets(name);
 strcat(name,".dat");
 rename(file_name,name);
 create_file("untitled.dat");
 strcpy(file_name,"untitled.dat");
 cout<<"                                    saving...";
 here:;
 reset_mouse();
 fill_background(WHITE);
 show(file_name);
  default_interface(shape,selected_color,open,start_x=539,start_y=42,end_x=getmaxx(),end_y=386,gap=10);
  show_mouse();


 end:;
 }

 else
 if(button_clicked()==20) //presses file icon
 {
 char name[100];
 default_interface(shape,selected_color,0,start_x=539,start_y=42,end_x=getmaxx(),end_y=386,gap=10);
 gotoxy(1,1);
 cout<<"enter name to save by=>";
 gets(name);
 strcat(name,".dat");
 rename(file_name,name);
 strcpy(file_name,name);
 cout<<"                                    saving...";
 reset_mouse();
 fill_background(WHITE);
 show(file_name);
  default_interface(shape,selected_color,open,start_x=539,start_y=42,end_x=getmaxx(),end_y=386,gap=10);
  show_mouse();

 }

 if(button_clicked()==23)//clicked on open eye icon
  {
  hide_mouse();
 open=0;
 default_interface(shape,selected_color,open,start_x=539,start_y=42,end_x=getmaxx(),end_y=386,gap=10);
  show(file_name);
  show_mouse();
  lifted();
  }
 else
 if(button_clicked()==19) //clicked on shape-box to change current shape
  {
 shape++;
 if(shape==4)
 shape=0;
 hide_mouse();
 shape_box(shape,selected_color,start_x,start_y,end_x,end_y,gap);
 show_mouse();
 lifted();
  }
 for(int i=3;i<=18;i++)
  {
 if(button_clicked()==i&&selected_color!=k[i-3]) //clicked on a color box to change current color
 {
 selected_color=k[i-3];
  hide_mouse();
 shape_box(shape,selected_color,start_x,start_y,end_x,end_y,gap);
 show_mouse();
 lifted();
 break;
 }
  }
}
 else
 if(button_clicked()==23)  //clicked on  closed eye icon
 {
   open=1;
   hide_mouse();
 default_interface(shape,selected_color,open);
 show_mouse();
 lifted();

 }

 if(button_clicked()==0)   //if help button is clicked
 {
  gotoxy(1,2);
  cout<<"help";
  gotoxy(64,2);
  cout<<"import";
  gotoxy(76,1);
  cout<<"exit";
  gotoxy(58,23);
  cout<<"select shape";
  gotoxy(23,1);
  cout<<"press any key to continue...";
  getch();
  hide_mouse();
  rect_fill(-1,-1,35,35);
  rect_fill(150,-1,400,30);
  rect_fill(500,-1,600,35);
  rect_fill(590,-1,650,35);
  rect_fill(450,350,550,370);
   show(file_name);
  default_interface(shape,selected_color,open);

  show_mouse();

 }
draw(file_name,open,shape,selected_color,start_x,start_y,end_x,end_y);
}
while(button_clicked()==2);//while the cross button is not released
return 1;
}

//if you read this all in one go then you are truly a legend
//message if you don't understand anything of this code (despite my awesome //comments )

//peace out!
