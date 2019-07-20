#pragma comment(lib, "glaux.lib") 
#include    <windows.h>		                  // Windows的头文件
#include    <gl/glut.h>		                  // 包含OpenGL实用库
#include	<stdio.h>	                      // 标准输入/输出库的头文件
#include	<gl/glaux.h>						  // GLaux库的头文件
#include	<math.h>
#include    <stdlib.h>
#include    <time.h>
#include    <iostream.h>
//..................................定义变量区............................................................//

//.............................用于场景漫游有关变量........................................//
GLfloat		  xrot=0.0f;								// X 旋转量
GLfloat		  yrot=0.0f;								// Y 旋转量
GLfloat		  x=0.0f;								    // X 平移量
GLfloat		  y=0.0f;								    // Y 平移量
GLfloat	      z=-30.0f;								    // 深入屏幕的距离
//.............................用于键盘操作有关变量.......................................//
BOOL	      light=TRUE;									               // 光源的开/关
BOOL	      lp;									                   // L键按下了么?
BOOL	      fp;									                   // F键按下了么?
BOOL          TurnOn=FALSE;                                            // 开灯
GLUquadricObj *qobj;                   
int           fantheta=0;                                               // 风扇转过角度
HGLRC         hRC=NULL;						                        	// 窗口着色描述表句柄
HDC           hDC=NULL;							                        // OpenGL渲染描述表句柄
HWND          hWnd=NULL;							                    // 保存我们的窗口句柄
HINSTANCE     hInstance;							                    // 保存程序的实例
bool	      keys[256];								                // 保存键盘按键的数组
bool	      active=TRUE;								                // 窗口的活动标志，缺省为TRUE
bool	      fullscreen=TRUE;							                // 全屏标志缺省，缺省设定成全屏模式
//.................................光照材质有关变量......................................//
GLfloat       light_Ambient[]= { 0.1f, 0.1f, 0.1f, 1.0f }; 				// 环境光参数
GLfloat       light_Diffuse[]= { 1.0f, 1.0f, 0.45f, 1.0f };				// 漫射光参数
GLfloat       light_position[]={0.0,29.9,0.0,1.0};                     // 光源位置
GLfloat       light_position1[]={20.0,29.9,0.0,1.0};
GLfloat       mat_ambient[]={0.05f,0.05f,0.05f,1.0f};                   // 材质环境光参数
GLfloat       mat_specular[]={0.8,1.0,1.0,1.0};                         // 材质镜面光参数
GLfloat       mat_shininess[]={5.0};                                  
GLfloat       white_light[]={1.0,1.0,1.0,1.0};                          // 光
GLfloat	      no_mat[]= {0.0f, 0.0f, 0.0f, 1.0f};		                // 无材质色
GLfloat	      no_shininess[]= {0.0f};						            // 无反光
//GLfloat	      mat_diffuse[]= {1.0f, 1.0f, 1.0f, 1.0f};		            // 漫反射
//GLfloat       diffusion[]={0.521f,0.121f,0.0547f,1.0};
//......................................纹理...........................................................//
GLuint	      texture[6];								                // 7种纹理的储存空间
//.....................................时钟有关变量...................................................//
GLfloat       cx=15.5,cy=20.0,cz=-17.4,radius=5.0;
GLfloat       hradius=2.0,mradius=3.0,sradius=4.0;
GLfloat       sangle,mangle,hangle;
GLuint        second,miniter,hour;
char          timebuffer[9];
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);				    // WndProc的定义




//......................................创建纹理..........................................................//
AUX_RGBImageRec *LoadBMP(char *Filename)					           // 载入位图图象
{
	FILE *File=NULL;							                       // 文件句柄
	if (!Filename)								                       // 确保文件名已提供
	{
	  return NULL;							                           // 如果没提供，返回 NULL
	}
	File=fopen(Filename,"r");						                   // 尝试打开文件
	if (File)								                           // 文件存在么?
	{
	  fclose(File);						                           	   // 关闭句柄
	  return auxDIBImageLoad(Filename);				                   // 载入位图并返回指针
	}
	return NULL;
}								                                      // 如果载入失败，返回 NULL

	
int LoadGLTextures()							 	                  // 载入位图并转换成纹理
{
	int Status=FALSE;							                      // 状态指示器
	AUX_RGBImageRec *TextureImage[6];					              // 创建纹理的存储空间
	memset(TextureImage,0,sizeof(void *)*1);				          // 将指针设为 NULL
		                                                              // 载入位图，检查有无错误，如果位图没找到则退出
	Status=TRUE;	                                                  // 将 Status 设为 TRUE
 //...............................创建第一个纹理，地板.......................................//  
    if (TextureImage[0]=LoadBMP("4.bmp"))
	{
							                     
        glGenTextures(1, &texture[0]);// 创建纹理                                                                 
		glBindTexture(GL_TEXTURE_2D, texture[0]);                  
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//远处采用线性滤波
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);//近处采用MipMapped滤波  
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);//绑定纹理 
	}
    if (TextureImage[0])//纹理是否存在
	{
		if (TextureImage[0]->data)//纹理图像是否存在
		{
			free(TextureImage[0]->data);//释放纹理图像占用的内存
		}
		free(TextureImage[0]);//释放图像结构
	}
 //...............................创建第二个纹理，墙.......................................//
	 if (TextureImage[1]=LoadBMP("2.bmp"))
	{
	
        glGenTextures(1, &texture[1]);   //创建纹理					                                                                            
		glBindTexture(GL_TEXTURE_2D, texture[1]);                     
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//远处采用线性滤波
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);//近处采用MipMapped滤波 
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);//绑定纹理  
	}
    if (TextureImage[1])//纹理是否存在							                  
	{
		if (TextureImage[1]->data)//纹理图像是否存在					                  
		{
			free(TextureImage[1]->data);//释放纹理图像占用的内存				              
		}
		free(TextureImage[1]);//释放图像结构					 	                 
	}
 //...............................创建第五个纹理，灯.......................................//
	if (TextureImage[4]=LoadBMP("lamp.bmp"))
	{
		Status=TRUE;						
        glGenTextures(1, &texture[4]);//创建纹理				
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//远处采用线性滤波
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);//近处采用MipMapped滤波 
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[4]->sizeX, TextureImage[4]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[4]->data);//绑定纹理  
	}
    if (TextureImage[4])//纹理是否存在							
	{
		if (TextureImage[4]->data)//纹理图像是否存在					
		{
			free(TextureImage[4]->data);//释放纹理图像占用的内存			
		}
		free(TextureImage[4]);//释放图像结构						
	}
    return Status;							
}

//...................................重置OpenGL窗口大小..............................................//
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)				// 重置OpenGL窗口大小
{
	if (height==0)								                // 防止被零除
	{
		height=1;							                    // 将Height设为1
	}
	glViewport(0, 0, width, height);					        // 重置当前的视口
    glMatrixMode(GL_PROJECTION);					         	// 选择投影矩阵
	glLoadIdentity();							                // 重置投影矩阵
	                                                            // 设置视口的大小
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);
	glMatrixMode(GL_MODELVIEW);						            // 选择模型观察矩阵
	glLoadIdentity();							                // 重置模型观察矩阵
} 


//...................................初始化操作................................................//
int InitGL(GLvoid)								                // 此处开始对OpenGL进行所有设置
{
	if (!LoadGLTextures())							            // 调用纹理载入子例程
	{
		return FALSE;							                // 如果未能载入，返回FALSE
	}
	glEnable(GL_TEXTURE_2D);						            // 启用纹理映射
	glShadeModel(GL_SMOOTH);						            // 启用阴影平滑
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					    // 黑色背景
    glClearDepth(1.0f);							                // 设置深度缓存
	glEnable(GL_DEPTH_TEST);						            // 启用深度测试
	glDepthFunc(GL_LEQUAL);							            // 所作深度测试的类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// 告诉系统对透视进行修正
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,mat_ambient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
	glLightfv(GL_LIGHT1,GL_POSITION,light_position);            // 设置光源位置
	glLightfv(GL_LIGHT1,GL_AMBIENT,light_Ambient);               // 设置环境光
	glLightfv(GL_LIGHT1,GL_DIFFUSE,white_light);                 // 设置漫射光
	glLightfv(GL_LIGHT1,GL_SPECULAR,white_light);

	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);	                                     // 启用一号光源	
	glEnable(GL_COLOR_MATERIAL);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);                       //对齐像素字节函数

	return TRUE;								                 // 初始化 OK

}

//..............................................开始绘制..........................................//
int DrawGLScene(GLvoid)								             // 从这里开始进行所有的绘制
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			 // 清除屏幕和深度缓存
	glLoadIdentity();						                   	 // 重置当前的模型观察矩阵
	glTranslatef(x,y,z)	;                                        //沿X,Y,Z轴移动
    glRotatef(xrot,1.0f,0.0f,0.0f);						         // 绕X轴旋转
	glRotatef(yrot,0.0f,1.0f,0.0f);						         // 绕Y轴旋转
  
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);                  //选择材质光照
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
//.............................................房间........................................................//



 //.............................................地板........................................................//
  	glBindTexture(GL_TEXTURE_2D, texture[0]);	// 选择纹理
	glBegin(GL_QUADS); //开始绘制四边形			
   	glColor3f(1.0f, 1.0f, 1.0f);                    
	glNormal3f(0.0,1.0,0.0); //glNormal3f用于定义点的法向量		
	glTexCoord2f(0.0f, 0.0f);glVertex3f(-30.0f,  -21.0f,  72.0f);//左下
	glTexCoord2f(0.0f, 1.0f);glVertex3f(-30.0f,  -21.0f, -97.0f);//左上
	glTexCoord2f(1.0f, 1.0f);glVertex3f( 30.0f,  -21.0f, -97.0f);//右上
	glTexCoord2f(1.0f, 0.0f);glVertex3f( 30.0f,  -21.0f,  72.0f);//右下
	glEnd();
    
//.............................................天花板......................................................//
    glBindTexture(GL_TEXTURE_2D, texture[2]);				// 选择纹理
	glBegin(GL_QUADS);//开始绘制四边形 
	glColor3f(1.0f, 1.0f, 1.0f);
	glNormal3f(0.0f, -1.0f, 0.0f);//glNormal3f用于定义点的法向量
	glTexCoord2f(1.0f, 1.0f);glVertex3f(-30.0f, 29.8f, -97.0f);//右上
	glTexCoord2f(1.0f, 0.0f);glVertex3f(-30.0f, 29.8f,  72.0f);//右下
	glTexCoord2f(0.0f, 0.0f);glVertex3f( 30.0f, 29.8f,  72.0f);//左下
	glTexCoord2f(0.0f, 1.0f);glVertex3f( 30.0f, 29.8f, -97.0f);//左上				
	glEnd();


//............................................后墙面.......................................................//
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);//开始绘制四边形
	glColor3f(1.0f, 1.0f, 1.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);//glNormal3f用于定义点的法向量				
	glTexCoord2f(1.0f, 1.0f);glVertex3f(-30.0f,  30.0f, -97.0f);//右上
	glTexCoord2f(1.0f, 0.0f);glVertex3f(-30.0f, -21.0f, -97.0f);//右下
	glTexCoord2f(0.0f, 0.0f);glVertex3f(  30.0f, -21.0f, -97.0f);//左下
	glTexCoord2f(0.0f, 1.0f);glVertex3f(  30.0f,  30.0f, -97.0f);//左上


//............................................前墙面.......................................................//
	glNormal3f(0.0f, 0.0f, -1.0f);//glNormal3f用于定义点的法向量						
	glTexCoord2f(1.0f, 1.0f);glVertex3f( 30.0f,  30.0f,  72.0f);//右上
	glTexCoord2f(1.0f, 0.0f);glVertex3f( 30.0f, -21.0f,  72.0f);//右下
	glTexCoord2f(0.0f, 0.0f);glVertex3f(-30.0f, -21.0f,  72.0f);//左下
	glTexCoord2f(0.0f, 1.0f);glVertex3f(-30.0f,  30.0f,  72.0f);//左上
//............................................左墙面.......................................................//
	glNormal3f( 1.0f, 0.0f, 0.0f);//glNormal3f用于定义点的法向量						
	glTexCoord2f(1.0f, 1.0f);glVertex3f(-30.0f,  30.0f,  72.0f);//右上
	glTexCoord2f(1.0f, 0.0f);glVertex3f(-30.0f, -21.0f,  72.0f);//右下
	glTexCoord2f(0.0f, 0.0f);glVertex3f(-30.0f, -21.0f, -97.0f);//左下
	glTexCoord2f(0.0f, 1.0f);glVertex3f(-30.0f,  30.0f, -97.0f);//左上
//............................................右墙面.......................................................//
	glNormal3f(-1.0f, 0.0f, 0.0f);//glNormal3f用于定义点的法向量					
	glTexCoord2f(1.0f, 1.0f);glVertex3f( 30.0f,  30.0f, -97.0f);//右上
	glTexCoord2f(1.0f, 0.0f);glVertex3f(30.0f, -21.0f, -97.0f);//右下
	glTexCoord2f(0.0f, 0.0f);glVertex3f( 30.0f, -21.0f,  72.0f);//左下
	glTexCoord2f(0.0f, 1.0f);glVertex3f( 30.0f, 30.0f,   72.0f);//左上

	glEnd();
//.............................................床..........................................................//
	GLuint i=0,t,l;
	//一共三张上下床

//............................................第一张床......................................................//
    //..........................上床板.......................//
	glBindTexture(GL_TEXTURE_2D, texture[0]);
		glPushMatrix();//因为下面要对坐标进行变换，所以先将信息入栈保护，使整个工程的坐标不发生改变
		glColor3f(1.0,1.0,1.0);
		glTranslatef(-22.0f,-3.0f,49.5f);//平移变换
		glScalef(15.0f,0.3f,30.0f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈
    glEnd();
    //..........................下床板......................//

		glPushMatrix();//压入堆栈
		glColor3f(1.0,1.0,0.45);
		glTranslatef(-22.0f,-15.0f,49.5f);//平移变换
		glScalef(15.0f,0.3f,30.0f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈
	
   
    //...........................床的左边腿...................//

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-29.5f,-17.0f,64.5f);//平移变换
		glScalef(0.5f,33.0f,0.3f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-29.5f,-17.0f,34.5f);//平移变换
		glScalef(0.5f,33.0f,0.3f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

    //...........................床的右边腿...................//

	    glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-14.5f,-17.0f,64.5f);//平移变换
		glScalef(0.5f,33.0f,0.3f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-14.5f,-17.0f,34.5f);//平移变换
		glScalef(0.5f,33.0f,0.3f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈


		//............................床梯............................//

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-14.5f,-9.0f,36.5f);//平移变换
		glScalef(0.2f,12.0f,0.2f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-14.5f,-9.0f,40.5f);//平移变换
		glScalef(0.2f,12.0f,0.2f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		for(t=0;t<3;t++)
		{
			glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-14.5f,-15.0f+4*t,38.5);//平移变换
		glScalef(0.2f,0.2f,4.0f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}
	    //............................下边架............................//

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-22.5f,-13.0f,34.5f);//平移变换
		glScalef(15.0f,0.2f,0.2f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-22.5f,-13.0f,64.5f);//平移变换
		glScalef(15.0f,0.2f,0.2f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		for(t=0;t<3;t++)
		{
		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-27.0f+t*5,-14.0f,34.5f);//平移变换
		glScalef(0.2f,2.0f,0.2f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}

		for(t=0;t<3;t++)
		{
		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-27.0f+t*5,-14.0f,64.5f);//平移变换
		glScalef(0.2f,2.0f,0.2f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}

		//............................上边架............................//

     	glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-22.5f,-1.0f,34.5f);//平移变换
		glScalef(15.0f,0.2f,0.2f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-22.5f,-1.0f,64.5f);//平移变换
		glScalef(15.0f,0.2f,0.2f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		for(t=0;t<3;t++)
		{
		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-27.0f+t*5,-2.0f,34.5f);//平移变换
		glScalef(0.2f,2.0f,0.2f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}

		for(t=0;t<3;t++)
		{
		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(-27.0f+t*5,-2.0f,64.5f);//平移变换
		glScalef(0.2f,2.0f,0.2f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}


//............................................第二张床......................................................//
    //..........................上床板.......................//

		glPushMatrix();//因为下面要对坐标进行变换，所以先将信息入栈保护，使整个工程的坐标不发生改变
		glColor3f(1.0,1.0,0.45);
		glTranslatef(22.0f,-3.0f,49.5f);//平移变换
		glScalef(15.0f,0.3f,30.0f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈
    //..........................下床板......................//

		glPushMatrix();//压入堆栈
		glColor3f(1.0,1.0,0.45);
		glTranslatef(22.0f,-15.0f,49.5f);//平移变换
		glScalef(15.0f,0.3f,30.0f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈
   
    //...........................床的左边腿...................//

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(29.5f,-17.0f,64.5f);//平移变换
		glScalef(0.5f,33.0f,0.3f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(29.5f,-17.0f,34.5f);//平移变换
		glScalef(0.5f,33.0f,0.3f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

    //...........................床的右边腿...................//

	    glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-17.0f,64.5f);//平移变换
		glScalef(0.5f,33.0f,0.3f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-17.0f,34.5f);//平移变换
		glScalef(0.5f,33.0f,0.3f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈


		//............................床梯............................//

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-9.0f,36.5f);//平移变换
		glScalef(0.2f,12.0f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-9.0f,40.5f);//平移变换
		glScalef(0.2f,12.0f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		for(t=0;t<3;t++)
		{
			glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-15.0f+4*t,38.5);//平移变换
		glScalef(0.2f,0.2f,4.0f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}
	    //............................下边架............................//

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-13.0f,34.5f);//平移变换
		glScalef(15.0f,0.2f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-13.0f,64.5f);//平移变换
		glScalef(15.0f,0.2f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		for(t=0;t<3;t++)
		{
		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-14.0f,34.5f);//平移变换
		glScalef(0.2f,2.0f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}

		for(t=0;t<3;t++)
		{
		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-14.0f,64.5f);//平移变换
		glScalef(0.2f,2.0f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}

		//............................上边架............................//

     	glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-1.0f,34.5f);//平移变换
		glScalef(15.0f,0.2f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-1.0f,64.5f);//平移变换
		glScalef(15.0f,0.2f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		for(t=0;t<3;t++)
		{
		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-2.0f,34.5f);//平移变换
		glScalef(0.2f,2.0f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}

		for(t=0;t<3;t++)
		{
		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-2.0f,64.5f);//平移变换
		glScalef(0.2f,2.0f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}

		//............................................第三张床......................................................//
    //..........................上床板.......................//

		glPushMatrix();//因为下面要对坐标进行变换，所以先将信息入栈保护，使整个工程的坐标不发生改变
		glColor3f(1.0,1.0,0.45);
		glTranslatef(22.0f,-3.0f,4.5f);//平移变换
		glScalef(15.0f,0.3f,30.0f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈
    //..........................下床板......................//

		glPushMatrix();//压入堆栈
		glColor3f(1.0,1.0,0.45);
		glTranslatef(22.0f,-15.0f,4.5f);//平移变换
		glScalef(15.0f,0.3f,30.0f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈
   
    //...........................床的左边腿...................//

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(29.5f,-17.0f,19.5f);//平移变换
		glScalef(0.5f,33.0f,0.3f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(29.5f,-17.0f,-10.5f);//平移变换
		glScalef(0.5f,33.0f,0.3f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

    //...........................床的右边腿...................//

	    glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-17.0f,19.5f);//平移变换
		glScalef(0.5f,33.0f,0.3f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-17.0f,-10.5f);//平移变换
		glScalef(0.5f,33.0f,0.3f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈


		//............................床梯............................//

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-9.0f,17.5f);//平移变换
		glScalef(0.2f,12.0f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-9.0f,13.5f);//平移变换
		glScalef(0.2f,12.0f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		for(t=0;t<3;t++)
		{
			glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(14.5f,-15.0f+4*t,15.5);//平移变换
		glScalef(0.2f,0.2f,4.0f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}
	    //............................下边架............................//

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-13.0f,-10.5f);//平移变换
		glScalef(15.0f,0.2f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-13.0f,19.5f);//平移变换
		glScalef(15.0f,0.2f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		for(t=0;t<3;t++)
		{
		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-14.0f,-10.5f);//平移变换
		glScalef(0.2f,2.0f,0.2f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}

		for(t=0;t<3;t++)
		{
		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-14.0f,19.5f);//平移变换
		glScalef(0.2f,2.0f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}

		//............................上边架............................//

     	glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-1.0f,-10.5f);//平移变换
		glScalef(15.0f,0.2f,0.2f);//缩放变换

		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(22.5f,-1.0f,19.5f);//平移变换
		glScalef(15.0f,0.2f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		for(t=0;t<3;t++)
		{
		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-2.0f,-10.5f);//平移变换
		glScalef(0.2f,2.0f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}

		for(t=0;t<3;t++)
		{
		glPushMatrix();//压入堆栈
		glColor3f(0.0,0.0,0.0);
		glTranslatef(27.0f-t*5,-2.0f,19.5f);//平移变换
		glScalef(0.2f,2.0f,0.2f);//缩放变换
		glutSolidCube(1.0);//绘制立方体
		glPopMatrix();//弹出堆栈

		}

		//....................................书桌.............................................................//
		for(t=0;t<3;t++){
   	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-23.0f,-15.5f,27.5f-t*31);//平移变换
	glScalef(13.0f,11.0f,0.3f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	

	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-23.0f,-15.5f,-2.5f-t*31);//平移变换
	glScalef(13.0f,11.0f,0.3f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-25.5f,2.5f,-2.5f-t*31);//平移变换
	glScalef(8.0f,25.0f,0.3f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	glPushMatrix();//压入堆栈
    glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-25.5f,2.5f,12.5f-t*31);//平移变换
	glScalef(8.0f,25.0f,0.3f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-25.5f,2.5f,27.5f-t*31);//平移变换
	glScalef(8.0f,25.0f,0.3f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-25.5f,15.0f,12.5f-t*31);//平移变换
	glScalef(8.5f,0.3f,30.0f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-25.5f,1.0f,12.5f-t*31);//平移变换
	glScalef(8.5f,0.3f,30.0f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-25.5f,-2.5f,12.5f-t*31);//平移变换
	glScalef(6.0f,0.3f,30.0f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	for(l=0;l<2;l++){
	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-21.5f,8.0f,23.8f-t*31-l*15.5);//平移变换
	glScalef(0.3f,14.0f,7.4f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-21.5f,8.0f,16.2f-t*31-l*15.5);//平移变换
	glScalef(0.3f,14.0f,7.4f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-23.0f,-12.0f,21.8f-t*31-l*18.6);//平移变换
	glScalef(13.0f,0.3f,11.2f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-16.5f,-11.0f,21.8f-t*31-l*18.6);//平移变换
	glScalef(0.3f,2.0f,11.2f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-16.5f,-12.5f-l*5.2,12.5f-t*31);//平移变换
	glScalef(0.3f,5.0f,7.4f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈


	}

	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-23.0f,-15.5f,12.5f-t*31);//平移变换
	glScalef(13.0f,11.0f,7.4f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	glPushMatrix();//压入堆栈
	glColor3f(0.5f,0.25f,0.0f);
	glTranslatef(-23.0f,-10.0f,12.5f-t*31);//平移变换
	glScalef(13.5f,0.3f,30.0f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

		}

	//................................阳台................................................//
    glBindTexture(GL_TEXTURE_2D, texture[1]);
	glPushMatrix();//压入堆栈
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(18.7f,4.5f,-19.5f);//平移变换
	glScalef(22.6f,51.0f,2.0f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈
    glEnd();
	glPushMatrix();//压入堆栈
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(8.5f,4.5f,-45.8f);//平移变换
	glScalef(2.5f,51.0f,52.6f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	glPushMatrix();//压入堆栈
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(11.2f,22.0f,-71.0f);//平移变换
	glScalef(37.5f,20.0f,2.0f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈

	glPushMatrix();//压入堆栈
	glColor3f(1.0f,1.0f,1.0f);

	glTranslatef(-17.8f,4.5f,-71.0f);//平移变换
	glScalef(22.5f,51.0f,2.0f);//缩放变换
	glutSolidCube(1.0);//绘制立方体
	glPopMatrix();//弹出堆栈


	

	
//................................风扇................................................//
    GLuint m=0;
	for(;m<2;m++)//一共两排风扇
	{  
		qobj = gluNewQuadric();//初始化二次曲面并创建一个指向二次曲面的指针 
		
	   glPushMatrix();//压入堆栈，保证此处坐标变换不影响外部大环境 
       glTranslatef(0.0,-3.0,0.0+40*m);  //风扇的位置
		      if(TurnOn==TRUE)  //如果TurnOn为1
			  {
		             fantheta+=10;  //风扇转过的角度fantheta不断增加
		             if(fantheta==360)  //如果旋转角度超过360度
						 fantheta=0;   //则fantheta重新回0
	                 
			  }         
			glRotatef(fantheta,0,1,0);        //风扇旋转，方向

		//..........风扇中间的扁圆形....................//

			glPushMatrix();//压入堆栈
			glColor3f(0.7,0.7,0.7);
			glTranslatef(0.0,20.0,0.0);//平移变换
			glScalef(1,0.3,1);//缩放变换
			gluSphere(qobj,2,20,20);//绘制一个半径为2的球体
            glPopMatrix();//弹出堆栈
		//............风扇旋转120度的扇叶...............//	
		
			glPushMatrix();//压入堆栈
         	glColor3f(0.7,0.7,0.7);
			glTranslatef(0,20,0.0);//平移变换
			glScalef(1,0.05,1);//缩放变换
			glRotatef(120,0,1,0);//绕y周旋转120度
			gluCylinder(qobj,1,3,10,20,20);//绘制一个底面圆半径为1、顶面圆半径为2、高为10的圆锥体
            glPopMatrix();//弹出堆栈
		//............风扇不做旋转的扇叶.................//

			glPushMatrix();//压入堆栈
			glColor3f(0.7,0.7,0.7);
			glTranslatef(0,20,0.0);//平移变换
			glScalef(1,0.05,1);//缩放变换
			gluCylinder(qobj,1,3,10,20,20);//绘制一个底面圆半径为1、顶面圆半径为2、高为10的圆锥体

			glPopMatrix();//弹出堆栈
        //.............风扇旋转240度的扇叶...............//

			glPushMatrix();//压入堆栈
            glColor3f(0.7,0.7,0.7);
			glTranslatef(0,20,0.0);//平移变换
			glScalef(1,0.05,1);//缩放变换
			glRotatef(240,0,1,0);//绕y周旋转240度	
	    	gluCylinder(qobj,1,3,10,20,20);//绘制一个底面圆半径为1、顶面圆半径为2、高为10的圆锥体

	        glPopMatrix();//弹出堆栈
        //.............风扇的杆...........................//

			glPushMatrix();//压入堆栈
	        glTranslatef(0,33,0.0);//平移变换
	        glRotatef(90,1,0,0);//缩放变换
	        gluCylinder(qobj,0.5,0.5,13,10,10);//绘制一个半径为0.5、高为13的圆锥体
            glPopMatrix();//弹出堆栈

            glPopMatrix();//弹出堆栈

     
			}		


 //............................................灯......................................................//
	GLuint j=0;
	for(;j<2;j++)  //一共两排灯
	{
      glBindTexture(GL_TEXTURE_2D,texture[4]);//绑定纹理
	  glColor3f(1.0f,1.0f,1.0f);
	  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,light_Diffuse);//定义材质光照	
	  glBegin(GL_QUADS);//开始绘制四边形
	  glColor3f(1.0f,1.0f,1.0f);
	  glNormal3f(0.0f, -1.0f, 0.0f);
	  glTexCoord2f(0.0f, 1.0f);glVertex3f( -10.0f,  29.9f, -20.0f+j*40);//左上
	  glTexCoord2f(1.0f, 1.0f);glVertex3f(  10.0f,  29.9f, -20.0f+j*40);//右上
	  glTexCoord2f(1.0f, 0.0f);glVertex3f(  10.0f,  29.9f, -0.0f+j*40);//右下
	  glTexCoord2f(0.0f, 0.0f);glVertex3f( -10.0f,  29.9f, -0.0f+j*40);//左下

	  glEnd();
	
	}

    
//........................................时钟..........................................................//
	_strtime(timebuffer);//取系统时间到timebuffer数组中
    hour=((int)timebuffer[0]-48)*10+((int)timebuffer[1]-48);//从timebuffer数组中得到小时的值
    miniter=((int)timebuffer[3]-48)*10+((int)timebuffer[4]-48);//从timebuffer数组中得到分钟的值
    second=((int)timebuffer[6]-48)*10+((int)timebuffer[7]-48);//从timebuffer数组中得到秒钟的值
    sangle=(float)(3.1415/2-(float)second*2*3.1415/60);//算得秒针走过的角度       
    mangle=(float)3.1415/2-(float)(miniter+(float)second/60)/60*2*3.1415;//算得分针走过的角度  
    hangle=(float)3.1415/2-(float)(hour+(float)(miniter+(float)second/60)/60)*2*3.1415/12;//算得时针走过的角度
 
  //..............钟面上的时刻点............................//
    int array[13][2];//12个大点
    int arr[60][2];//60个小点
    float angle1,angle2;
    angle1=2*3.1415/12;//将圆周分成12份
    angle2=2*3.1415/60;//将圆周分成60份
    for(int q=0;q<60;q++)
	{
       arr[q][0]=cx+radius*cos(q*angle2);//求得x轴坐标
       arr[q][1]=cy+radius*sin(q*angle2);//求得y轴坐标
       glBegin(GL_POINTS);//绘制点
       glPointSize(1.0);//制定点的大小
       glVertex3f(arr[q][0]+0.4,arr[q][1]+0.4,cz);//确定点的位置
       glEnd();
	}
     for(int w=0;w<12;w++)
	 {
       array[w][0]=cx+radius*cos(w*angle1);//求得x轴坐标
       array[w][1]=cy+radius*sin(w*angle1);//求得y轴坐标
       glPointSize(1.5);//绘制点
       glBegin(GL_POINTS);//制定点的大小
       glVertex3f(array[w][0]+0.4,array[w][1]+0.4,cz);//确定点的位置
       glEnd();
	 }
  //.....................时钟的轮廓...........................//
     float x,y,d;
     float bradius=5.2;//时钟轮廓的半径
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
  //....................秒针..................................//
     float harr1[2];
     harr1[0]=cx+sradius*cos(sangle);//求得x轴坐标
     harr1[1]=cy+sradius*sin(sangle);//求得y轴坐标
     glBegin(GL_LINES);//绘制线段
     glVertex3f(harr1[0],harr1[1],cz);//线段终点
     glVertex3f(cx,cy,cz);//线段起点
     glEnd();
  //.....................分针..................................//
     float harr2[2];
     harr2[0]=cx+mradius*cos(mangle);//求得x轴坐标
     harr2[1]=cy+mradius*sin(mangle);//求得y轴坐标
     glBegin(GL_LINES);//绘制线段
     glVertex3f(harr2[0],harr2[1],cz);//线段终点
     glVertex3f(cx,cy,cz);//线段起点
     glEnd();
  //......................时针..................................//
     float harr3[2];
     harr3[0]=cx+hradius*cos(hangle);//求得x轴坐标
     harr3[1]=cy+hradius*sin(hangle);//求得y轴坐标
     glBegin(GL_LINES);//绘制线段
     glVertex3f(harr3[0],harr3[1],cz);//线段终点
     glVertex3f(cx,cy,cz);//线段起点
     glEnd();
  //...........................................................//
   return TRUE;	
}


GLvoid KillGLWindow(GLvoid)							// 正常销毁窗口
{  
	if (fullscreen)								// 我们处于全屏模式吗?
	{
		ChangeDisplaySettings(NULL,0);					// 是的话，切换回桌面
		ShowCursor(TRUE);						// 显示鼠标指针
	}
    if (hRC)								// 我们拥有OpenGL渲染描述表吗?
	{
        if (!wglMakeCurrent(NULL,NULL))					// 我们能否释放DC和RC描述表?
		{
			MessageBox(NULL,"释放DC或RC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		}
    if (!wglDeleteContext(hRC))					// 我们能否删除RC?
		{
	MessageBox(NULL,"释放RC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;							// 将RC设为 NULL
	}
    if (hDC && !ReleaseDC(hWnd,hDC))					// 我们能否释放 DC?
	{
		MessageBox(NULL,"释放DC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;							// 将 DC 设为 NULL
	}
    if (hWnd && !DestroyWindow(hWnd))					// 能否销毁窗口?
	{
		MessageBox(NULL,"释放窗口句柄失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;							// 将 hWnd 设为 NULL
	}
	if (!UnregisterClass("OpenG",hInstance))				// 能否注销类?
	{
		MessageBox(NULL,"不能注销窗口类。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;							// 将 hInstance 设为 NULL
	}
}


BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;						// 保存查找匹配的结果
    WNDCLASS	wc;							// 窗口类结构
    DWORD		dwExStyle;						// 扩展窗口风格
	DWORD		dwStyle;						// 窗口风格
    RECT WindowRect;							// 取得矩形的左上角和右下角的坐标值
	WindowRect.left=(long)0;						// 将Left   设为 0
	WindowRect.right=(long)width;						// 将Right  设为要求的宽度
	WindowRect.top=(long)0;							// 将Top    设为 0
	WindowRect.bottom=(long)height;						// 将Bottom 设为要求的高度
    fullscreen=fullscreenflag;						// 设置全局全屏标志
    hInstance		= GetModuleHandle(NULL);			// 取得我们窗口的实例
	wc.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// 移动时重画，并为窗口取得DC
	wc.lpfnWndProc		= (WNDPROC) WndProc;				// WndProc处理消息
	wc.cbClsExtra		= 0;						// 无额外窗口数据
	wc.cbWndExtra		= 0;						// 无额外窗口数据
	wc.hInstance		= hInstance;					// 设置实例
	wc.hIcon		= LoadIcon(NULL, IDI_WINLOGO);			// 装入缺省图标
	wc.hCursor		= LoadCursor(NULL, IDC_ARROW);			// 装入鼠标指针
    wc.hbrBackground	= NULL;						// GL不需要背景
	wc.lpszMenuName		= NULL;						// 不需要菜单
	wc.lpszClassName	= "OpenG";					// 设定类名字
    if (!RegisterClass(&wc))						// 尝试注册窗口类
	{

		MessageBox(NULL,"注册窗口失败","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 退出并返回FALSE
	}
	if (fullscreen)								// 要尝试全屏模式吗?
	{
        DEVMODE dmScreenSettings;						// 设备模式
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));			// 确保内存清空为零
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);			// Devmode 结构的大小
		dmScreenSettings.dmPelsWidth	= width;				// 所选屏幕宽度
		dmScreenSettings.dmPelsHeight	= height;				// 所选屏幕高度
		dmScreenSettings.dmBitsPerPel	= bits;					// 每象素所选的色彩深度
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
        if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
         // 若模式失败，提供两个选项：退出或在窗口内运行。
			if (MessageBox(NULL,"全屏模式在当前显卡上设置失败！\n使用窗口模式？","NeHe G",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				 fullscreen=FALSE;				// 选择窗口模式(Fullscreen=FALSE)
			}
			else
			{
				// 弹出一个对话框，告诉用户程序结束
				MessageBox(NULL,"程序将被关闭","错误",MB_OK|MB_ICONSTOP);
				return FALSE;					//  退出并返回 FALSE
			}
		}
	}
	if (fullscreen)								// 仍处于全屏模式吗?
	{
      dwExStyle=WS_EX_APPWINDOW;					// 扩展窗体风格
	  dwStyle=WS_POPUP;						// 窗体风格
	  ShowCursor(FALSE);						// 隐藏鼠标指针
	}
	else
	{
      dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// 扩展窗体风格
	  dwStyle=WS_OVERLAPPEDWINDOW;					//  窗体风格
	}
   AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// 调整窗口达到真正要求的大小
   if (!(hWnd=CreateWindowEx(	dwExStyle,"OpenG",title,WS_CLIPSIBLINGS |WS_CLIPCHILDREN |dwStyle,0, 0,WindowRect.right-WindowRect.left,
	   WindowRect.bottom-WindowRect.top,NULL,NULL,hInstance,NULL)))				
      {
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"不能创建一个窗口设备描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
   }
   static	PIXELFORMATDESCRIPTOR pfd=					// /pfd 告诉窗口我们所希望的东东，即窗口使用的像素格式
	{
    	sizeof(PIXELFORMATDESCRIPTOR),					// 上述格式描述符的大小

		1,								// 版本号
		PFD_DRAW_TO_WINDOW |						// 格式支持窗口
		PFD_SUPPORT_OPENGL |						// 格式必须支持OpenGL
		PFD_DOUBLEBUFFER,						// 必须支持双缓冲
		PFD_TYPE_RGBA,							// 申请 RGBA 格式
		bits,								// 选定色彩深度
		0, 0, 0, 0, 0, 0,						// 忽略的色彩位
		0,								// 无Alpha缓存
		0,								// 忽略Shift Bit
		0,								// 无累加缓存
		0, 0, 0, 0,							// 忽略聚集位
		16,								// 16位 Z-缓存 (深度缓存)
		0,								// 无蒙板缓存
		0,								// 无辅助缓存
		PFD_MAIN_PLANE,							// 主绘图层
		0,								// Reserved
		0, 0, 0								// 忽略层遮罩
	};
    if (!(hDC=GetDC(hWnd)))							// 取得设备描述表了么?
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"不能创建一种相匹配的像素格式","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}
    if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))				// Windows 找到相应的象素格式了吗?
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"不能设置像素格式","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}
    if(!SetPixelFormat(hDC,PixelFormat,&pfd))				// 能够设置象素格式么?
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"不能设置像素格式","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}
    if (!(hRC=wglCreateContext(hDC)))					// 能否取得着色描述表?
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"不能创建OpenGL渲染描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}
    if(!wglMakeCurrent(hDC,hRC))						// 尝试激活着色描述表
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"不能激活当前的OpenGL渲然描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}
    ShowWindow(hWnd,SW_SHOW);						// 显示窗口
	SetForegroundWindow(hWnd);						// 略略提高优先级
	SetFocus(hWnd);								// 设置键盘的焦点至此窗口
	ReSizeGLScene(width, height);						// 设置透视 GL 屏幕
    if (!InitGL())								// 初始化新建的GL窗口
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}
    return TRUE;								// 成功
}


LRESULT CALLBACK WndProc(	HWND	hWnd,					// 窗口的句柄
				UINT	uMsg,					// 窗口的消息
				WPARAM	wParam,					// 附加的消息内容
				LPARAM	lParam)					// 附加的消息内容
{
    switch (uMsg)								// 检查Windows消息
	{
         case WM_ACTIVATE:						// 监视窗口激活消息
		{
			if (!HIWORD(wParam))					// 检查最小化状态
			{
				active=TRUE;					// 程序处于激活状态
			}
			else
			{
				active=FALSE;					// 程序不再激活
			}
			return 0;						// 返回消息循环
		}
		case WM_SYSCOMMAND:						// 系统中断命令
		{
			switch (wParam)						// 检查系统调用
			{
				case SC_SCREENSAVE:				// 屏保要运行?
				case SC_MONITORPOWER:				// 显示器要进入节电模式?
				return 0;					// 阻止发生
			}
			break;							// 退出
		}
		case WM_CLOSE:							// 收到Close消息?
		{
			PostQuitMessage(0);					// 发出退出消息
			return 0;						// 返回
		}
        case WM_KEYDOWN:						// 有键按下么?
		{
			keys[wParam] = TRUE;					// 如果是，设为TRUE
			return 0;						// 返回
		}
		case WM_KEYUP:							// 有键放开么?
		{
			keys[wParam] = FALSE;					// 如果是，设为FALSE
			return 0;						// 返回
		}
        case WM_SIZE:							// 调整OpenGL窗口大小
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));		// LoWord=Width,HiWord=Height
			return 0;						// 返回
		}
	}
	
// 向 DefWindowProc传递所有未处理的消息。
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

//................................main函数.......................................................//
int main(	HINSTANCE	hInstance,				// 当前窗口实例
			HINSTANCE	hPrevInstance,				// 前一个窗口实例
			LPSTR		lpCmdLine,				// 命令行参数
			int		nCmdShow)				// 窗口显示状态
{
    MSG	msg;								// Windowsx消息结构
	BOOL	done=FALSE;							// 用来退出循环的Bool 变量
// 提示用户选择运行模式
		
	if (MessageBox(NULL,"你想在全屏模式下运行么？", "设置全屏模式",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;						// FALSE为窗口模式
	}
// 创建OpenGL窗口
	if (!CreateGLWindow("",640,480,16,fullscreen))
	{
		return 0;							// 失败退出
	}
    while(!done)								// 保持循环直到 done=TRUE
	{
     if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))			// 有消息在等待吗?
		{
         if (msg.message==WM_QUIT)				// 收到退出消息?
			{
				done=TRUE;					// 是，则done=TRUE
			}
			else							// 不是，处理窗口消息
			{
                TranslateMessage(&msg);				// 翻译消息
		    	DispatchMessage(&msg);				// 发送消息
			}
		}
		else								// 如果没有消息
		{
            if (active)						// 程序激活的么?
			{
				if (keys[VK_ESCAPE])				// ESC 按下了么?
				{
					done=TRUE;				// ESC 发出退出信号
				}
				else						// 不是退出的时候，刷新屏幕
				{
                 	DrawGLScene();				// 绘制场景
					
					SwapBuffers(hDC);			// 交换缓存 (双缓存)
//................................按键定义区...................................................................//
					if (keys['L'] && !lp)				// L 键已按下并且松开了?
					{
                       lp=TRUE;				// lp 设为 TRUE
					   light=!light;				// 切换光源的 TRUE/FALSE
                       if (!light)				// 如果没有光源
							glDisable(GL_LIGHT1);		//关灯
				       else					// 否则
			    			glEnable(GL_LIGHT1);		//开灯
					}
					if (!keys['L'])					// L键松开了么?
				   	    lp=FALSE;				// 若是，则将lp设为FALSE
	//.........................开关风扇...........................................//			
					if (keys['F'] && !fp)			
					{
                       fp=TRUE;				
					   TurnOn=!TurnOn;
					}
					if (!keys['F'])				
				   	    fp=FALSE;				    
     //.......................场景漫游............................................//             
                    if (keys[VK_PRIOR])				// PageUp按下了?
					{
					    y-=0.1f;				// 若按下，向下
					}
                    if (keys[VK_NEXT])				// PageDown按下了么
					{
				    	y+=0.1f;				// 若按下，向上
					}
                    if (keys[VK_UP])				// Up方向键按下了么?
					{
					    xrot-=0.1f;				// 若是,y轴方向逆时针转动
					}
				    if (keys[VK_DOWN])				// Down方向键按下了么?
					{
					    xrot+=0.1f;				// 若是,y轴方向顺时针转动
					}
				    if (keys[VK_RIGHT])				// Right方向键按下了么?
					{
				     	yrot+=0.1f;				// 若是,x轴方向顺时针转动
					}
				    if (keys[VK_LEFT])				// Left方向键按下了么?
					{
				    	yrot-=0.1f;				// 若是,x轴方向逆时针转动
					}
					 if (keys['A'])				// A键按下了么?
					{
				     	x+=0.1f;				// 若是,向左
					}
				    if (keys['D'])				// D键按下了么?
					{
				    	x-=0.1f;				// 若是, 向右
					}
					 if (keys['W'])				// W键按下了么?
					{
				     	z+=0.1f;				// 若是,向前
					}
				    if (keys['S'])				// S键按下了么?
					{
				    	z-=0.1f;				// 若是, 向后
					}
				}
			}
        	
			if (keys[VK_F1])					// F1键按下了么?

			{

				keys[VK_F1]=FALSE;				// 若是，使对应的Key数组中的值为 FALSE

				KillGLWindow();					// 销毁当前的窗口

				fullscreen=!fullscreen;				// 切换 全屏 / 窗口 模式

				// 重建 OpenGL 窗口(修改)

			if (!CreateGLWindow("黄大贞，许演杰",640,480,16,fullscreen))


				{

					return 0;				// 如果窗口未能创建，程序退出

				}

			}

		}

	}

KillGLWindow();								// 销毁窗口

	return (msg.wParam);							// 退出程序

}
