#pragma comment(lib, "glaux.lib") 
#include    <windows.h>		                  // Windows��ͷ�ļ�
#include    <gl/glut.h>		                  // ����OpenGLʵ�ÿ�
#include	<stdio.h>	                      // ��׼����/������ͷ�ļ�
#include	<gl/glaux.h>						  // GLaux���ͷ�ļ�
#include	<math.h>
#include    <stdlib.h>
#include    <time.h>
#include    <iostream.h>
//..................................���������............................................................//

//.............................���ڳ��������йر���........................................//
GLfloat		  xrot=0.0f;								// X ��ת��
GLfloat		  yrot=0.0f;								// Y ��ת��
GLfloat		  x=0.0f;								    // X ƽ����
GLfloat		  y=0.0f;								    // Y ƽ����
GLfloat	      z=-30.0f;								    // ������Ļ�ľ���
//.............................���ڼ��̲����йر���.......................................//
BOOL	      light=TRUE;									               // ��Դ�Ŀ�/��
BOOL	      lp;									                   // L��������ô?
BOOL	      fp;									                   // F��������ô?
BOOL          TurnOn=FALSE;                                            // ����
GLUquadricObj *qobj;                   
int           fantheta=0;                                               // ����ת���Ƕ�
HGLRC         hRC=NULL;						                        	// ������ɫ��������
HDC           hDC=NULL;							                        // OpenGL��Ⱦ��������
HWND          hWnd=NULL;							                    // �������ǵĴ��ھ��
HINSTANCE     hInstance;							                    // ��������ʵ��
bool	      keys[256];								                // ������̰���������
bool	      active=TRUE;								                // ���ڵĻ��־��ȱʡΪTRUE
bool	      fullscreen=TRUE;							                // ȫ����־ȱʡ��ȱʡ�趨��ȫ��ģʽ
//.................................���ղ����йر���......................................//
GLfloat       light_Ambient[]= { 0.1f, 0.1f, 0.1f, 1.0f }; 				// ���������
GLfloat       light_Diffuse[]= { 1.0f, 1.0f, 0.45f, 1.0f };				// ��������
GLfloat       light_position[]={0.0,29.9,0.0,1.0};                     // ��Դλ��
GLfloat       light_position1[]={20.0,29.9,0.0,1.0};
GLfloat       mat_ambient[]={0.05f,0.05f,0.05f,1.0f};                   // ���ʻ��������
GLfloat       mat_specular[]={0.8,1.0,1.0,1.0};                         // ���ʾ�������
GLfloat       mat_shininess[]={5.0};                                  
GLfloat       white_light[]={1.0,1.0,1.0,1.0};                          // ��
GLfloat	      no_mat[]= {0.0f, 0.0f, 0.0f, 1.0f};		                // �޲���ɫ
GLfloat	      no_shininess[]= {0.0f};						            // �޷���
//GLfloat	      mat_diffuse[]= {1.0f, 1.0f, 1.0f, 1.0f};		            // ������
//GLfloat       diffusion[]={0.521f,0.121f,0.0547f,1.0};
//......................................����...........................................................//
GLuint	      texture[6];								                // 7������Ĵ���ռ�
//.....................................ʱ���йر���...................................................//
GLfloat       cx=15.5,cy=20.0,cz=-17.4,radius=5.0;
GLfloat       hradius=2.0,mradius=3.0,sradius=4.0;
GLfloat       sangle,mangle,hangle;
GLuint        second,miniter,hour;
char          timebuffer[9];
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);				    // WndProc�Ķ���




//......................................��������..........................................................//
AUX_RGBImageRec *LoadBMP(char *Filename)					           // ����λͼͼ��
{
	FILE *File=NULL;							                       // �ļ����
	if (!Filename)								                       // ȷ���ļ������ṩ
	{
	  return NULL;							                           // ���û�ṩ������ NULL
	}
	File=fopen(Filename,"r");						                   // ���Դ��ļ�
	if (File)								                           // �ļ�����ô?
	{
	  fclose(File);						                           	   // �رվ��
	  return auxDIBImageLoad(Filename);				                   // ����λͼ������ָ��
	}
	return NULL;
}								                                      // �������ʧ�ܣ����� NULL

	
int LoadGLTextures()							 	                  // ����λͼ��ת��������
{
	int Status=FALSE;							                      // ״ָ̬ʾ��
	AUX_RGBImageRec *TextureImage[6];					              // ��������Ĵ洢�ռ�
	memset(TextureImage,0,sizeof(void *)*1);				          // ��ָ����Ϊ NULL
		                                                              // ����λͼ��������޴������λͼû�ҵ����˳�
	Status=TRUE;	                                                  // �� Status ��Ϊ TRUE
 //...............................������һ�������ذ�.......................................//  
    if (TextureImage[0]=LoadBMP("4.bmp"))
	{
							                     
        glGenTextures(1, &texture[0]);// ��������                                                                 
		glBindTexture(GL_TEXTURE_2D, texture[0]);                  
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//Զ�����������˲�
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);//��������MipMapped�˲�  
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);//������ 
	}
    if (TextureImage[0])//�����Ƿ����
	{
		if (TextureImage[0]->data)//����ͼ���Ƿ����
		{
			free(TextureImage[0]->data);//�ͷ�����ͼ��ռ�õ��ڴ�
		}
		free(TextureImage[0]);//�ͷ�ͼ��ṹ
	}
 //...............................�����ڶ�������ǽ.......................................//
	 if (TextureImage[1]=LoadBMP("2.bmp"))
	{
	
        glGenTextures(1, &texture[1]);   //��������					                                                                            
		glBindTexture(GL_TEXTURE_2D, texture[1]);                     
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//Զ�����������˲�
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);//��������MipMapped�˲� 
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);//������  
	}
    if (TextureImage[1])//�����Ƿ����							                  
	{
		if (TextureImage[1]->data)//����ͼ���Ƿ����					                  
		{
			free(TextureImage[1]->data);//�ͷ�����ͼ��ռ�õ��ڴ�				              
		}
		free(TextureImage[1]);//�ͷ�ͼ��ṹ					 	                 
	}
 //...............................���������������.......................................//
	if (TextureImage[4]=LoadBMP("lamp.bmp"))
	{
		Status=TRUE;						
        glGenTextures(1, &texture[4]);//��������				
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//Զ�����������˲�
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);//��������MipMapped�˲� 
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[4]->sizeX, TextureImage[4]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[4]->data);//������  
	}
    if (TextureImage[4])//�����Ƿ����							
	{
		if (TextureImage[4]->data)//����ͼ���Ƿ����					
		{
			free(TextureImage[4]->data);//�ͷ�����ͼ��ռ�õ��ڴ�			
		}
		free(TextureImage[4]);//�ͷ�ͼ��ṹ						
	}
    return Status;							
}

//...................................����OpenGL���ڴ�С..............................................//
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)				// ����OpenGL���ڴ�С
{
	if (height==0)								                // ��ֹ�����
	{
		height=1;							                    // ��Height��Ϊ1
	}
	glViewport(0, 0, width, height);					        // ���õ�ǰ���ӿ�
    glMatrixMode(GL_PROJECTION);					         	// ѡ��ͶӰ����
	glLoadIdentity();							                // ����ͶӰ����
	                                                            // �����ӿڵĴ�С
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);
	glMatrixMode(GL_MODELVIEW);						            // ѡ��ģ�͹۲����
	glLoadIdentity();							                // ����ģ�͹۲����
} 


//...................................��ʼ������................................................//
int InitGL(GLvoid)								                // �˴���ʼ��OpenGL������������
{
	if (!LoadGLTextures())							            // ������������������
	{
		return FALSE;							                // ���δ�����룬����FALSE
	}
	glEnable(GL_TEXTURE_2D);						            // ��������ӳ��
	glShadeModel(GL_SMOOTH);						            // ������Ӱƽ��
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					    // ��ɫ����
    glClearDepth(1.0f);							                // ������Ȼ���
	glEnable(GL_DEPTH_TEST);						            // ������Ȳ���
	glDepthFunc(GL_LEQUAL);							            // ������Ȳ��Ե�����
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// ����ϵͳ��͸�ӽ�������
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,mat_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
	glLightfv(GL_LIGHT1,GL_POSITION,light_position);            // ���ù�Դλ��
	glLightfv(GL_LIGHT1,GL_AMBIENT,light_Ambient);               // ���û�����
	glLightfv(GL_LIGHT1,GL_DIFFUSE,white_light);                 // ���������
	glLightfv(GL_LIGHT1,GL_SPECULAR,white_light);

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);	                                     // ����һ�Ź�Դ	
	glEnable(GL_COLOR_MATERIAL);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);                       //���������ֽں���

	return TRUE;								                 // ��ʼ�� OK

}

//..............................................��ʼ����..........................................//
int DrawGLScene(GLvoid)								             // �����￪ʼ�������еĻ���
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			 // �����Ļ����Ȼ���
	glLoadIdentity();						                   	 // ���õ�ǰ��ģ�͹۲����
	glTranslatef(x,y,z)	;                                        //��X,Y,Z���ƶ�
    glRotatef(xrot,1.0f,0.0f,0.0f);						         // ��X����ת
	glRotatef(yrot,0.0f,1.0f,0.0f);						         // ��Y����ת
  
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);                  //ѡ����ʹ���
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
//.............................................����........................................................//



 //.............................................�ذ�........................................................//
  	glBindTexture(GL_TEXTURE_2D, texture[0]);	// ѡ������
	glBegin(GL_QUADS); //��ʼ�����ı���			
   	glColor3f(1.0f, 1.0f, 1.0f);                    
	glNormal3f(0.0,1.0,0.0); //glNormal3f���ڶ����ķ�����		
	glTexCoord2f(0.0f, 0.0f);glVertex3f(-30.0f,  -21.0f,  72.0f);//����
	glTexCoord2f(0.0f, 1.0f);glVertex3f(-30.0f,  -21.0f, -97.0f);//����
	glTexCoord2f(1.0f, 1.0f);glVertex3f( 30.0f,  -21.0f, -97.0f);//����
	glTexCoord2f(1.0f, 0.0f);glVertex3f( 30.0f,  -21.0f,  72.0f);//����
	glEnd();
    
//.............................................�컨��......................................................//
    glBindTexture(GL_TEXTURE_2D, texture[2]);				// ѡ������
	glBegin(GL_QUADS);//��ʼ�����ı��� 
	glColor3f(1.0f, 1.0f, 1.0f);
	glNormal3f(0.0f, -1.0f, 0.0f);//glNormal3f���ڶ����ķ�����
	glTexCoord2f(1.0f, 1.0f);glVertex3f(-30.0f, 29.8f, -97.0f);//����
	glTexCoord2f(1.0f, 0.0f);glVertex3f(-30.0f, 29.8f,  72.0f);//����
	glTexCoord2f(0.0f, 0.0f);glVertex3f( 30.0f, 29.8f,  72.0f);//����
	glTexCoord2f(0.0f, 1.0f);glVertex3f( 30.0f, 29.8f, -97.0f);//����				
	glEnd();


//............................................��ǽ��.......................................................//
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);//��ʼ�����ı���
	glColor3f(1.0f, 1.0f, 1.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);//glNormal3f���ڶ����ķ�����				
	glTexCoord2f(1.0f, 1.0f);glVertex3f(-30.0f,  30.0f, -97.0f);//����
	glTexCoord2f(1.0f, 0.0f);glVertex3f(-30.0f, -21.0f, -97.0f);//����
	glTexCoord2f(0.0f, 0.0f);glVertex3f(  30.0f, -21.0f, -97.0f);//����
	glTexCoord2f(0.0f, 1.0f);glVertex3f(  30.0f,  30.0f, -97.0f);//����


//............................................ǰǽ��.......................................................//
	glNormal3f(0.0f, 0.0f, -1.0f);//glNormal3f���ڶ����ķ�����						
	glTexCoord2f(1.0f, 1.0f);glVertex3f( 30.0f,  30.0f,  72.0f);//����
	glTexCoord2f(1.0f, 0.0f);glVertex3f( 30.0f, -21.0f,  72.0f);//����
	glTexCoord2f(0.0f, 0.0f);glVertex3f(-30.0f, -21.0f,  72.0f);//����
	glTexCoord2f(0.0f, 1.0f);glVertex3f(-30.0f,  30.0f,  72.0f);//����
//............................................��ǽ��.......................................................//
	glNormal3f( 1.0f, 0.0f, 0.0f);//glNormal3f���ڶ����ķ�����						
	glTexCoord2f(1.0f, 1.0f);glVertex3f(-30.0f,  30.0f,  72.0f);//����
	glTexCoord2f(1.0f, 0.0f);glVertex3f(-30.0f, -21.0f,  72.0f);//����
	glTexCoord2f(0.0f, 0.0f);glVertex3f(-30.0f, -21.0f, -97.0f);//����
	glTexCoord2f(0.0f, 1.0f);glVertex3f(-30.0f,  30.0f, -97.0f);//����
//............................................��ǽ��.......................................................//
	glNormal3f(-1.0f, 0.0f, 0.0f);//glNormal3f���ڶ����ķ�����					
	glTexCoord2f(1.0f, 1.0f);glVertex3f( 30.0f,  30.0f, -97.0f);//����
	glTexCoord2f(1.0f, 0.0f);glVertex3f(30.0f, -21.0f, -97.0f);//����
	glTexCoord2f(0.0f, 0.0f);glVertex3f( 30.0f, -21.0f,  72.0f);//����
	glTexCoord2f(0.0f, 1.0f);glVertex3f( 30.0f, 30.0f,   72.0f);//����

	glEnd();
//.............................................��..........................................................//
	GLuint i=0,t,l;
	//һ���������´�

//............................................��һ�Ŵ�......................................................//
    //..........................�ϴ���.......................//
	glBindTexture(GL_TEXTURE_2D, texture[0]);
		glPushMatrix();//��Ϊ����Ҫ��������б任�������Ƚ���Ϣ��ջ������ʹ�������̵����겻�����ı�
		glColor3f(1.0,1.0,1.0);
		glTranslatef(-22.0f,-3.0f,49.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.3f,30.0f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ
    glEnd();
    //..........................�´���......................//

		glPushMatrix();//ѹ���ջ
		glColor3f(1.0,1.0,0.45);
		glTranslatef(-22.0f,-15.0f,49.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.3f,30.0f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ
	
   
    //...........................���������...................//

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-29.5f,-17.0f,64.5f);//ƽ�Ʊ任
		glScalef(0.5f,33.0f,0.3f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-29.5f,-17.0f,34.5f);//ƽ�Ʊ任
		glScalef(0.5f,33.0f,0.3f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

    //...........................�����ұ���...................//

	    glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-14.5f,-17.0f,64.5f);//ƽ�Ʊ任
		glScalef(0.5f,33.0f,0.3f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-14.5f,-17.0f,34.5f);//ƽ�Ʊ任
		glScalef(0.5f,33.0f,0.3f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ


		//............................����............................//

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-14.5f,-9.0f,36.5f);//ƽ�Ʊ任
		glScalef(0.2f,12.0f,0.2f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-14.5f,-9.0f,40.5f);//ƽ�Ʊ任
		glScalef(0.2f,12.0f,0.2f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		for(t=0;t<3;t++)
		{
			glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-14.5f,-15.0f+4*t,38.5);//ƽ�Ʊ任
		glScalef(0.2f,0.2f,4.0f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}
	    //............................�±߼�............................//

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-22.5f,-13.0f,34.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.2f,0.2f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-22.5f,-13.0f,64.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.2f,0.2f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		for(t=0;t<3;t++)
		{
		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-27.0f+t*5,-14.0f,34.5f);//ƽ�Ʊ任
		glScalef(0.2f,2.0f,0.2f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}

		for(t=0;t<3;t++)
		{
		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-27.0f+t*5,-14.0f,64.5f);//ƽ�Ʊ任
		glScalef(0.2f,2.0f,0.2f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}

		//............................�ϱ߼�............................//

     	glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-22.5f,-1.0f,34.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.2f,0.2f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-22.5f,-1.0f,64.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.2f,0.2f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		for(t=0;t<3;t++)
		{
		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-27.0f+t*5,-2.0f,34.5f);//ƽ�Ʊ任
		glScalef(0.2f,2.0f,0.2f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}

		for(t=0;t<3;t++)
		{
		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-27.0f+t*5,-2.0f,64.5f);//ƽ�Ʊ任
		glScalef(0.2f,2.0f,0.2f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}


//............................................�ڶ��Ŵ�......................................................//
    //..........................�ϴ���.......................//

		glPushMatrix();//��Ϊ����Ҫ��������б任�������Ƚ���Ϣ��ջ������ʹ�������̵����겻�����ı�
		glColor3f(1.0,1.0,0.45);
		glTranslatef(22.0f,-3.0f,49.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.3f,30.0f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ
    //..........................�´���......................//

		glPushMatrix();//ѹ���ջ
		glColor3f(1.0,1.0,0.45);
		glTranslatef(22.0f,-15.0f,49.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.3f,30.0f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ
   
    //...........................���������...................//

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(29.5f,-17.0f,64.5f);//ƽ�Ʊ任
		glScalef(0.5f,33.0f,0.3f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(29.5f,-17.0f,34.5f);//ƽ�Ʊ任
		glScalef(0.5f,33.0f,0.3f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

    //...........................�����ұ���...................//

	    glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-17.0f,64.5f);//ƽ�Ʊ任
		glScalef(0.5f,33.0f,0.3f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-17.0f,34.5f);//ƽ�Ʊ任
		glScalef(0.5f,33.0f,0.3f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ


		//............................����............................//

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-9.0f,36.5f);//ƽ�Ʊ任
		glScalef(0.2f,12.0f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-9.0f,40.5f);//ƽ�Ʊ任
		glScalef(0.2f,12.0f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		for(t=0;t<3;t++)
		{
			glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-15.0f+4*t,38.5);//ƽ�Ʊ任
		glScalef(0.2f,0.2f,4.0f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}
	    //............................�±߼�............................//

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-13.0f,34.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.2f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-13.0f,64.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.2f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		for(t=0;t<3;t++)
		{
		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-14.0f,34.5f);//ƽ�Ʊ任
		glScalef(0.2f,2.0f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}

		for(t=0;t<3;t++)
		{
		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-14.0f,64.5f);//ƽ�Ʊ任
		glScalef(0.2f,2.0f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}

		//............................�ϱ߼�............................//

     	glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-1.0f,34.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.2f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-1.0f,64.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.2f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		for(t=0;t<3;t++)
		{
		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-2.0f,34.5f);//ƽ�Ʊ任
		glScalef(0.2f,2.0f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}

		for(t=0;t<3;t++)
		{
		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-2.0f,64.5f);//ƽ�Ʊ任
		glScalef(0.2f,2.0f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}

		//............................................�����Ŵ�......................................................//
    //..........................�ϴ���.......................//

		glPushMatrix();//��Ϊ����Ҫ��������б任�������Ƚ���Ϣ��ջ������ʹ�������̵����겻�����ı�
		glColor3f(1.0,1.0,0.45);
		glTranslatef(22.0f,-3.0f,4.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.3f,30.0f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ
    //..........................�´���......................//

		glPushMatrix();//ѹ���ջ
		glColor3f(1.0,1.0,0.45);
		glTranslatef(22.0f,-15.0f,4.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.3f,30.0f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ
   
    //...........................���������...................//

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(29.5f,-17.0f,19.5f);//ƽ�Ʊ任
		glScalef(0.5f,33.0f,0.3f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(29.5f,-17.0f,-10.5f);//ƽ�Ʊ任
		glScalef(0.5f,33.0f,0.3f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

    //...........................�����ұ���...................//

	    glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-17.0f,19.5f);//ƽ�Ʊ任
		glScalef(0.5f,33.0f,0.3f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-17.0f,-10.5f);//ƽ�Ʊ任
		glScalef(0.5f,33.0f,0.3f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ


		//............................����............................//

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-9.0f,17.5f);//ƽ�Ʊ任
		glScalef(0.2f,12.0f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-9.0f,13.5f);//ƽ�Ʊ任
		glScalef(0.2f,12.0f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		for(t=0;t<3;t++)
		{
			glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-15.0f+4*t,15.5);//ƽ�Ʊ任
		glScalef(0.2f,0.2f,4.0f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}
	    //............................�±߼�............................//

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-13.0f,-10.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.2f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-13.0f,19.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.2f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		for(t=0;t<3;t++)
		{
		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-14.0f,-10.5f);//ƽ�Ʊ任
		glScalef(0.2f,2.0f,0.2f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}

		for(t=0;t<3;t++)
		{
		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-14.0f,19.5f);//ƽ�Ʊ任
		glScalef(0.2f,2.0f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}

		//............................�ϱ߼�............................//

     	glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-1.0f,-10.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.2f,0.2f);//���ű任

		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-1.0f,19.5f);//ƽ�Ʊ任
		glScalef(15.0f,0.2f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		for(t=0;t<3;t++)
		{
		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-2.0f,-10.5f);//ƽ�Ʊ任
		glScalef(0.2f,2.0f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}

		for(t=0;t<3;t++)
		{
		glPushMatrix();//ѹ���ջ
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-2.0f,19.5f);//ƽ�Ʊ任
		glScalef(0.2f,2.0f,0.2f);//���ű任
		glutSolidCube(1.0);//����������
		glPopMatrix();//������ջ

		}

		//....................................����.............................................................//
		for(t=0;t<3;t++){
   	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-23.0f,-15.5f,27.5f-t*31);//ƽ�Ʊ任
	glScalef(13.0f,11.0f,0.3f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	

	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-23.0f,-15.5f,-2.5f-t*31);//ƽ�Ʊ任
	glScalef(13.0f,11.0f,0.3f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-25.5f,2.5f,-2.5f-t*31);//ƽ�Ʊ任
	glScalef(8.0f,25.0f,0.3f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	glPushMatrix();//ѹ���ջ
    glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-25.5f,2.5f,12.5f-t*31);//ƽ�Ʊ任
	glScalef(8.0f,25.0f,0.3f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-25.5f,2.5f,27.5f-t*31);//ƽ�Ʊ任
	glScalef(8.0f,25.0f,0.3f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-25.5f,15.0f,12.5f-t*31);//ƽ�Ʊ任
	glScalef(8.5f,0.3f,30.0f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-25.5f,1.0f,12.5f-t*31);//ƽ�Ʊ任
	glScalef(8.5f,0.3f,30.0f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-25.5f,-2.5f,12.5f-t*31);//ƽ�Ʊ任
	glScalef(6.0f,0.3f,30.0f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	for(l=0;l<2;l++){
	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-21.5f,8.0f,23.8f-t*31-l*15.5);//ƽ�Ʊ任
	glScalef(0.3f,14.0f,7.4f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-21.5f,8.0f,16.2f-t*31-l*15.5);//ƽ�Ʊ任
	glScalef(0.3f,14.0f,7.4f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-23.0f,-12.0f,21.8f-t*31-l*18.6);//ƽ�Ʊ任
	glScalef(13.0f,0.3f,11.2f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-16.5f,-11.0f,21.8f-t*31-l*18.6);//ƽ�Ʊ任
	glScalef(0.3f,2.0f,11.2f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-16.5f,-12.5f-l*5.2,12.5f-t*31);//ƽ�Ʊ任
	glScalef(0.3f,5.0f,7.4f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ


	}

	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-23.0f,-15.5f,12.5f-t*31);//ƽ�Ʊ任
	glScalef(13.0f,11.0f,7.4f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	glPushMatrix();//ѹ���ջ
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-23.0f,-10.0f,12.5f-t*31);//ƽ�Ʊ任
	glScalef(13.5f,0.3f,30.0f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

		}

	//................................��̨................................................//
    glBindTexture(GL_TEXTURE_2D, texture[1]);
	glPushMatrix();//ѹ���ջ
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(18.7f,4.5f,-19.5f);//ƽ�Ʊ任
	glScalef(22.6f,51.0f,2.0f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ
    glEnd();
	glPushMatrix();//ѹ���ջ
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(8.5f,4.5f,-45.8f);//ƽ�Ʊ任
	glScalef(2.5f,51.0f,52.6f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	glPushMatrix();//ѹ���ջ
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(11.2f,22.0f,-71.0f);//ƽ�Ʊ任
	glScalef(37.5f,20.0f,2.0f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ

	glPushMatrix();//ѹ���ջ
	glColor3f(1.0f,1.0f,1.0f);

	glTranslatef(-17.8f,4.5f,-71.0f);//ƽ�Ʊ任
	glScalef(22.5f,51.0f,2.0f);//���ű任
	glutSolidCube(1.0);//����������
	glPopMatrix();//������ջ


	

	
//................................����................................................//
    GLuint m=0;
	for(;m<2;m++)//һ�����ŷ���
	{  
		qobj = gluNewQuadric();//��ʼ���������沢����һ��ָ����������ָ�� 
		
	   glPushMatrix();//ѹ���ջ����֤�˴�����任��Ӱ���ⲿ�󻷾� 
       glTranslatef(0.0,-3.0,0.0+40*m);  //���ȵ�λ��
		      if(TurnOn==TRUE)  //���TurnOnΪ1
			  {
		             fantheta+=10;  //����ת���ĽǶ�fantheta��������
		             if(fantheta==360)  //�����ת�Ƕȳ���360��
						 fantheta=0;   //��fantheta���»�0
	                 
			  }         
			glRotatef(fantheta,0,1,0);        //������ת������

		//..........�����м�ı�Բ��....................//

			glPushMatrix();//ѹ���ջ
			glColor3f(0.7,0.7,0.7);
			glTranslatef(0.0,20.0,0.0);//ƽ�Ʊ任
			glScalef(1,0.3,1);//���ű任
			gluSphere(qobj,2,20,20);//����һ���뾶Ϊ2������
            glPopMatrix();//������ջ
		//............������ת120�ȵ���Ҷ...............//	
		
			glPushMatrix();//ѹ���ջ
         	glColor3f(0.7,0.7,0.7);
			glTranslatef(0,20,0.0);//ƽ�Ʊ任
			glScalef(1,0.05,1);//���ű任
			glRotatef(120,0,1,0);//��y����ת120��
			gluCylinder(qobj,1,3,10,20,20);//����һ������Բ�뾶Ϊ1������Բ�뾶Ϊ2����Ϊ10��Բ׶��
            glPopMatrix();//������ջ
		//............���Ȳ�����ת����Ҷ.................//

			glPushMatrix();//ѹ���ջ
			glColor3f(0.7,0.7,0.7);
			glTranslatef(0,20,0.0);//ƽ�Ʊ任
			glScalef(1,0.05,1);//���ű任
			gluCylinder(qobj,1,3,10,20,20);//����һ������Բ�뾶Ϊ1������Բ�뾶Ϊ2����Ϊ10��Բ׶��

			glPopMatrix();//������ջ
        //.............������ת240�ȵ���Ҷ...............//

			glPushMatrix();//ѹ���ջ
            glColor3f(0.7,0.7,0.7);
			glTranslatef(0,20,0.0);//ƽ�Ʊ任
			glScalef(1,0.05,1);//���ű任
			glRotatef(240,0,1,0);//��y����ת240��	
	    	gluCylinder(qobj,1,3,10,20,20);//����һ������Բ�뾶Ϊ1������Բ�뾶Ϊ2����Ϊ10��Բ׶��

	        glPopMatrix();//������ջ
        //.............���ȵĸ�...........................//

			glPushMatrix();//ѹ���ջ
	        glTranslatef(0,33,0.0);//ƽ�Ʊ任
	        glRotatef(90,1,0,0);//���ű任
	        gluCylinder(qobj,0.5,0.5,13,10,10);//����һ���뾶Ϊ0.5����Ϊ13��Բ׶��
            glPopMatrix();//������ջ

            glPopMatrix();//������ջ

     
			}		


 //............................................��......................................................//
	GLuint j=0;
	for(;j<2;j++)  //һ�����ŵ�
	{
      glBindTexture(GL_TEXTURE_2D,texture[4]);//������
	  glColor3f(1.0f,1.0f,1.0f);
	  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,light_Diffuse);//������ʹ���	
	  glBegin(GL_QUADS);//��ʼ�����ı���
	  glColor3f(1.0f,1.0f,1.0f);
	  glNormal3f(0.0f, -1.0f, 0.0f);
	  glTexCoord2f(0.0f, 1.0f);glVertex3f( -10.0f,  29.9f, -20.0f+j*40);//����
	  glTexCoord2f(1.0f, 1.0f);glVertex3f(  10.0f,  29.9f, -20.0f+j*40);//����
	  glTexCoord2f(1.0f, 0.0f);glVertex3f(  10.0f,  29.9f, -0.0f+j*40);//����
	  glTexCoord2f(0.0f, 0.0f);glVertex3f( -10.0f,  29.9f, -0.0f+j*40);//����

	  glEnd();
	
	}

    
//........................................ʱ��..........................................................//
	_strtime(timebuffer);//ȡϵͳʱ�䵽timebuffer������
    hour=((int)timebuffer[0]-48)*10+((int)timebuffer[1]-48);//��timebuffer�����еõ�Сʱ��ֵ
    miniter=((int)timebuffer[3]-48)*10+((int)timebuffer[4]-48);//��timebuffer�����еõ����ӵ�ֵ
    second=((int)timebuffer[6]-48)*10+((int)timebuffer[7]-48);//��timebuffer�����еõ����ӵ�ֵ
    sangle=(float)(3.1415/2-(float)second*2*3.1415/60);//��������߹��ĽǶ�       
    mangle=(float)3.1415/2-(float)(miniter+(float)second/60)/60*2*3.1415;//��÷����߹��ĽǶ�  
    hangle=(float)3.1415/2-(float)(hour+(float)(miniter+(float)second/60)/60)*2*3.1415/12;//���ʱ���߹��ĽǶ�
 
  //..............�����ϵ�ʱ�̵�............................//
    int array[13][2];//12�����
    int arr[60][2];//60��С��
    float angle1,angle2;
    angle1=2*3.1415/12;//��Բ�ֳܷ�12��
    angle2=2*3.1415/60;//��Բ�ֳܷ�60��
    for(int q=0;q<60;q++)
	{
       arr[q][0]=cx+radius*cos(q*angle2);//���x������
       arr[q][1]=cy+radius*sin(q*angle2);//���y������
       glBegin(GL_POINTS);//���Ƶ�
       glPointSize(1.0);//�ƶ���Ĵ�С
       glVertex3f(arr[q][0]+0.4,arr[q][1]+0.4,cz);//ȷ�����λ��
       glEnd();
	}
     for(int w=0;w<12;w++)
	 {
       array[w][0]=cx+radius*cos(w*angle1);//���x������
       array[w][1]=cy+radius*sin(w*angle1);//���y������
       glPointSize(1.5);//���Ƶ�
       glBegin(GL_POINTS);//�ƶ���Ĵ�С
       glVertex3f(array[w][0]+0.4,array[w][1]+0.4,cz);//ȷ�����λ��
       glEnd();
	 }
  //.....................ʱ�ӵ�����...........................//
     float x,y,d;
     float bradius=5.2;//ʱ�������İ뾶
     y=bradius;
     d=10-0.1*bradius;
     x=0;
     while(x<=y)
	 {
	   glColor3f(1.0,0.0,1.0);
       glBegin(GL_POINTS);
       glVertex3f(cx+x,cy+y,cz);
       glVertex3f(cx+x,cy-y,cz);
       glVertex3f(cx-x,cy+y,cz);
       glVertex3f(cx-x,cy-y,cz);
       glVertex3f(cx+y,cy+x,cz);
       glVertex3f(cx+y,cy-x,cz);
       glVertex3f(cx-y,cy+x,cz);
       glVertex3f(cx-y,cy-x,cz);
       glEnd();
       if(d<0)
        d+=0.1*x+0.2;
       else
	   {
          d+=4*(x-y)+10;
          y=y-0.1;
	   }
       x=x+0.1;
	 }
  //....................����..................................//
     float harr1[2];
     harr1[0]=cx+sradius*cos(sangle);//���x������
     harr1[1]=cy+sradius*sin(sangle);//���y������
     glBegin(GL_LINES);//�����߶�
     glVertex3f(harr1[0],harr1[1],cz);//�߶��յ�
     glVertex3f(cx,cy,cz);//�߶����
     glEnd();
  //.....................����..................................//
     float harr2[2];
     harr2[0]=cx+mradius*cos(mangle);//���x������
     harr2[1]=cy+mradius*sin(mangle);//���y������
     glBegin(GL_LINES);//�����߶�
     glVertex3f(harr2[0],harr2[1],cz);//�߶��յ�
     glVertex3f(cx,cy,cz);//�߶����
     glEnd();
  //......................ʱ��..................................//
     float harr3[2];
     harr3[0]=cx+hradius*cos(hangle);//���x������
     harr3[1]=cy+hradius*sin(hangle);//���y������
     glBegin(GL_LINES);//�����߶�
     glVertex3f(harr3[0],harr3[1],cz);//�߶��յ�
     glVertex3f(cx,cy,cz);//�߶����
     glEnd();
  //...........................................................//
   return TRUE;	
}


GLvoid KillGLWindow(GLvoid)							// �������ٴ���
{  
	if (fullscreen)								// ���Ǵ���ȫ��ģʽ��?
	{
		ChangeDisplaySettings(NULL,0);					// �ǵĻ����л�������
		ShowCursor(TRUE);						// ��ʾ���ָ��
	}
    if (hRC)								// ����ӵ��OpenGL��Ⱦ��������?
	{
        if (!wglMakeCurrent(NULL,NULL))					// �����ܷ��ͷ�DC��RC������?
		{
			MessageBox(NULL,"�ͷ�DC��RCʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);
		}
    if (!wglDeleteContext(hRC))					// �����ܷ�ɾ��RC?
		{
	MessageBox(NULL,"�ͷ�RCʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;							// ��RC��Ϊ NULL
	}
    if (hDC && !ReleaseDC(hWnd,hDC))					// �����ܷ��ͷ� DC?
	{
		MessageBox(NULL,"�ͷ�DCʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;							// �� DC ��Ϊ NULL
	}
    if (hWnd && !DestroyWindow(hWnd))					// �ܷ����ٴ���?
	{
		MessageBox(NULL,"�ͷŴ��ھ��ʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;							// �� hWnd ��Ϊ NULL
	}
	if (!UnregisterClass("OpenG",hInstance))				// �ܷ�ע����?
	{
		MessageBox(NULL,"����ע�������ࡣ","�رմ���",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;							// �� hInstance ��Ϊ NULL
	}
}


BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;						// �������ƥ��Ľ��
    WNDCLASS	wc;							// ������ṹ
    DWORD		dwExStyle;						// ��չ���ڷ��
	DWORD		dwStyle;						// ���ڷ��
    RECT WindowRect;							// ȡ�þ��ε����ϽǺ����½ǵ�����ֵ
	WindowRect.left=(long)0;						// ��Left   ��Ϊ 0
	WindowRect.right=(long)width;						// ��Right  ��ΪҪ��Ŀ��
	WindowRect.top=(long)0;							// ��Top    ��Ϊ 0
	WindowRect.bottom=(long)height;						// ��Bottom ��ΪҪ��ĸ߶�
    fullscreen=fullscreenflag;						// ����ȫ��ȫ����־
    hInstance		= GetModuleHandle(NULL);			// ȡ�����Ǵ��ڵ�ʵ��
	wc.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// �ƶ�ʱ�ػ�����Ϊ����ȡ��DC
	wc.lpfnWndProc		= (WNDPROC) WndProc;				// WndProc������Ϣ
	wc.cbClsExtra		= 0;						// �޶��ⴰ������
	wc.cbWndExtra		= 0;						// �޶��ⴰ������
	wc.hInstance		= hInstance;					// ����ʵ��
	wc.hIcon		= LoadIcon(NULL, IDI_WINLOGO);			// װ��ȱʡͼ��
	wc.hCursor		= LoadCursor(NULL, IDC_ARROW);			// װ�����ָ��
    wc.hbrBackground	= NULL;						// GL����Ҫ����
	wc.lpszMenuName		= NULL;						// ����Ҫ�˵�
	wc.lpszClassName	= "OpenG";					// �趨������
    if (!RegisterClass(&wc))						// ����ע�ᴰ����
	{

		MessageBox(NULL,"ע�ᴰ��ʧ��","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// �˳�������FALSE
	}
	if (fullscreen)								// Ҫ����ȫ��ģʽ��?
	{
        DEVMODE dmScreenSettings;						// �豸ģʽ
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));			// ȷ���ڴ����Ϊ��
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);			// Devmode �ṹ�Ĵ�С
		dmScreenSettings.dmPelsWidth	= width;				// ��ѡ��Ļ���
		dmScreenSettings.dmPelsHeight	= height;				// ��ѡ��Ļ�߶�
		dmScreenSettings.dmBitsPerPel	= bits;					// ÿ������ѡ��ɫ�����
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
        if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
         // ��ģʽʧ�ܣ��ṩ����ѡ��˳����ڴ��������С�
			if (MessageBox(NULL,"ȫ��ģʽ�ڵ�ǰ�Կ�������ʧ�ܣ�\nʹ�ô���ģʽ��","NeHe G",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				 fullscreen=FALSE;				// ѡ�񴰿�ģʽ(Fullscreen=FALSE)
			}
			else
			{
				// ����һ���Ի��򣬸����û��������
				MessageBox(NULL,"���򽫱��ر�","����",MB_OK|MB_ICONSTOP);
				return FALSE;					//  �˳������� FALSE
			}
		}
	}
	if (fullscreen)								// �Դ���ȫ��ģʽ��?
	{
      dwExStyle=WS_EX_APPWINDOW;					// ��չ������
	  dwStyle=WS_POPUP;						// ������
	  ShowCursor(FALSE);						// �������ָ��
	}
	else
	{
      dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// ��չ������
	  dwStyle=WS_OVERLAPPEDWINDOW;					//  ������
	}
   AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// �������ڴﵽ����Ҫ��Ĵ�С
   if (!(hWnd=CreateWindowEx(	dwExStyle,"OpenG",title,WS_CLIPSIBLINGS |WS_CLIPCHILDREN |dwStyle,0, 0,WindowRect.right-WindowRect.left,
	   WindowRect.bottom-WindowRect.top,NULL,NULL,hInstance,NULL)))				
      {
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"���ܴ���һ�������豸������","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
   }
   static	PIXELFORMATDESCRIPTOR pfd=					// /pfd ���ߴ���������ϣ���Ķ�����������ʹ�õ����ظ�ʽ
	{
    	sizeof(PIXELFORMATDESCRIPTOR),					// ������ʽ�������Ĵ�С

		1,								// �汾��
		PFD_DRAW_TO_WINDOW |						// ��ʽ֧�ִ���
		PFD_SUPPORT_OPENGL |						// ��ʽ����֧��OpenGL
		PFD_DOUBLEBUFFER,						// ����֧��˫����
		PFD_TYPE_RGBA,							// ���� RGBA ��ʽ
		bits,								// ѡ��ɫ�����
		0, 0, 0, 0, 0, 0,						// ���Ե�ɫ��λ
		0,								// ��Alpha����
		0,								// ����Shift Bit
		0,								// ���ۼӻ���
		0, 0, 0, 0,							// ���Ծۼ�λ
		16,								// 16λ Z-���� (��Ȼ���)
		0,								// ���ɰ建��
		0,								// �޸�������
		PFD_MAIN_PLANE,							// ����ͼ��
		0,								// Reserved
		0, 0, 0								// ���Բ�����
	};
    if (!(hDC=GetDC(hWnd)))							// ȡ���豸��������ô?
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"���ܴ���һ����ƥ������ظ�ʽ","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))				// Windows �ҵ���Ӧ�����ظ�ʽ����?
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"�����������ظ�ʽ","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
    if(!SetPixelFormat(hDC,PixelFormat,&pfd))				// �ܹ��������ظ�ʽô?
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"�����������ظ�ʽ","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
    if (!(hRC=wglCreateContext(hDC)))					// �ܷ�ȡ����ɫ������?
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"���ܴ���OpenGL��Ⱦ������","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
    if(!wglMakeCurrent(hDC,hRC))						// ���Լ�����ɫ������
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"���ܼ��ǰ��OpenGL��Ȼ������","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
    ShowWindow(hWnd,SW_SHOW);						// ��ʾ����
	SetForegroundWindow(hWnd);						// ����������ȼ�
	SetFocus(hWnd);								// ���ü��̵Ľ������˴���
	ReSizeGLScene(width, height);						// ����͸�� GL ��Ļ
    if (!InitGL())								// ��ʼ���½���GL����
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
    return TRUE;								// �ɹ�
}


LRESULT CALLBACK WndProc(	HWND	hWnd,					// ���ڵľ��
				UINT	uMsg,					// ���ڵ���Ϣ
				WPARAM	wParam,					// ���ӵ���Ϣ����
				LPARAM	lParam)					// ���ӵ���Ϣ����
{
    switch (uMsg)								// ���Windows��Ϣ
	{
         case WM_ACTIVATE:						// ���Ӵ��ڼ�����Ϣ
		{
			if (!HIWORD(wParam))					// �����С��״̬
			{
				active=TRUE;					// �����ڼ���״̬
			}
			else
			{
				active=FALSE;					// �����ټ���
			}
			return 0;						// ������Ϣѭ��
		}
		case WM_SYSCOMMAND:						// ϵͳ�ж�����
		{
			switch (wParam)						// ���ϵͳ����
			{
				case SC_SCREENSAVE:				// ����Ҫ����?
				case SC_MONITORPOWER:				// ��ʾ��Ҫ����ڵ�ģʽ?
				return 0;					// ��ֹ����
			}
			break;							// �˳�
		}
		case WM_CLOSE:							// �յ�Close��Ϣ?
		{
			PostQuitMessage(0);					// �����˳���Ϣ
			return 0;						// ����
		}
        case WM_KEYDOWN:						// �м�����ô?
		{
			keys[wParam] = TRUE;					// ����ǣ���ΪTRUE
			return 0;						// ����
		}
		case WM_KEYUP:							// �м��ſ�ô?
		{
			keys[wParam] = FALSE;					// ����ǣ���ΪFALSE
			return 0;						// ����
		}
        case WM_SIZE:							// ����OpenGL���ڴ�С
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));		// LoWord=Width,HiWord=Height
			return 0;						// ����
		}
	}
	
// �� DefWindowProc��������δ�������Ϣ��
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

//................................main����.......................................................//
int main(	HINSTANCE	hInstance,				// ��ǰ����ʵ��
			HINSTANCE	hPrevInstance,				// ǰһ������ʵ��
			LPSTR		lpCmdLine,				// �����в���
			int		nCmdShow)				// ������ʾ״̬
{
    MSG	msg;								// Windowsx��Ϣ�ṹ
	BOOL	done=FALSE;							// �����˳�ѭ����Bool ����
// ��ʾ�û�ѡ������ģʽ
		
	if (MessageBox(NULL,"������ȫ��ģʽ������ô��", "����ȫ��ģʽ",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;						// FALSEΪ����ģʽ
	}
// ����OpenGL����
	if (!CreateGLWindow("",640,480,16,fullscreen))
	{
		return 0;							// ʧ���˳�
	}
    while(!done)								// ����ѭ��ֱ�� done=TRUE
	{
     if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))			// ����Ϣ�ڵȴ���?
		{
         if (msg.message==WM_QUIT)				// �յ��˳���Ϣ?
			{
				done=TRUE;					// �ǣ���done=TRUE
			}
			else							// ���ǣ���������Ϣ
			{
                TranslateMessage(&msg);				// ������Ϣ
		    	DispatchMessage(&msg);				// ������Ϣ
			}
		}
		else								// ���û����Ϣ
		{
            if (active)						// ���򼤻��ô?
			{
				if (keys[VK_ESCAPE])				// ESC ������ô?
				{
					done=TRUE;				// ESC �����˳��ź�
				}
				else						// �����˳���ʱ��ˢ����Ļ
				{
                 	DrawGLScene();				// ���Ƴ���
					
					SwapBuffers(hDC);			// �������� (˫����)
//................................����������...................................................................//
					if (keys['L'] && !lp)				// L ���Ѱ��²����ɿ���?
					{
                       lp=TRUE;				// lp ��Ϊ TRUE
					   light=!light;				// �л���Դ�� TRUE/FALSE
                       if (!light)				// ���û�й�Դ
							glDisable(GL_LIGHT1);		//�ص�
				       else					// ����
			    			glEnable(GL_LIGHT1);		//����
					}
					if (!keys['L'])					// L���ɿ���ô?
				   	    lp=FALSE;				// ���ǣ���lp��ΪFALSE
	//.........................���ط���...........................................//			
					if (keys['F'] && !fp)			
					{
                       fp=TRUE;				
					   TurnOn=!TurnOn;
					}
					if (!keys['F'])				
				   	    fp=FALSE;				    
     //.......................��������............................................//             
                    if (keys[VK_PRIOR])				// PageUp������?
					{
					    y-=0.1f;				// �����£�����
					}
                    if (keys[VK_NEXT])				// PageDown������ô
					{
				    	y+=0.1f;				// �����£�����
					}
                    if (keys[VK_UP])				// Up�����������ô?
					{
					    xrot-=0.1f;				// ����,y�᷽����ʱ��ת��
					}
				    if (keys[VK_DOWN])				// Down�����������ô?
					{
					    xrot+=0.1f;				// ����,y�᷽��˳ʱ��ת��
					}
				    if (keys[VK_RIGHT])				// Right�����������ô?
					{
				     	yrot+=0.1f;				// ����,x�᷽��˳ʱ��ת��
					}
				    if (keys[VK_LEFT])				// Left�����������ô?
					{
				    	yrot-=0.1f;				// ����,x�᷽����ʱ��ת��
					}
					 if (keys['A'])				// A��������ô?
					{
				     	x+=0.1f;				// ����,����
					}
				    if (keys['D'])				// D��������ô?
					{
				    	x-=0.1f;				// ����, ����
					}
					 if (keys['W'])				// W��������ô?
					{
				     	z+=0.1f;				// ����,��ǰ
					}
				    if (keys['S'])				// S��������ô?
					{
				    	z-=0.1f;				// ����, ���
					}
				}
			}
        	
			if (keys[VK_F1])					// F1��������ô?

			{

				keys[VK_F1]=FALSE;				// ���ǣ�ʹ��Ӧ��Key�����е�ֵΪ FALSE

				KillGLWindow();					// ���ٵ�ǰ�Ĵ���

				fullscreen=!fullscreen;				// �л� ȫ�� / ���� ģʽ

				// �ؽ� OpenGL ����(�޸�)

			if (!CreateGLWindow("�ƴ��꣬���ݽ�",640,480,16,fullscreen))


				{

					return 0;				// �������δ�ܴ����������˳�

				}

			}

		}

	}

KillGLWindow();								// ���ٴ���

	return (msg.wParam);							// �˳�����

}
