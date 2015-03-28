#ifdef unix
#define MANGLE(NAME) tinygl##NAME
#else
#define MANGLE(NAME) gl##NAME
#endif

#define glEnable MANGLE(Enable)
#define glDisable MANGLE(Disable)

#define glShadeModel MANGLE(ShadeModel)
#define glCullFace MANGLE(CullFace)
#define glPolygonMode MANGLE(PolygonMode)

#define glBegin MANGLE(Begin)
#define glEnd MANGLE(End)

#define glVertex2f MANGLE(Vertex2f)
#define glVertex2d MANGLE(Vertex2d)
#define glVertex2fv MANGLE(Vertex2fv)
#define glVertex2dv MANGLE(Vertex2dv)
#define glVertex3f MANGLE(Vertex3f)
#define glVertex3d MANGLE(Vertex3d)
#define glVertex3fv MANGLE(Vertex3fv)
#define glVertex3dv MANGLE(Vertex3dv)
#define glVertex4f MANGLE(Vertex4f)
#define glVertex4d MANGLE(Vertex4d)
#define glVertex4fv MANGLE(Vertex4fv)
#define glVertex4dv MANGLE(Vertex4dv)

#define glColor3f MANGLE(Color3f)
#define glColor3d MANGLE(Color3d)
#define glColor3fv MANGLE(Color3fv)
#define glColor3dv MANGLE(Color3dv)
#define glColor4f MANGLE(Color4f)
#define glColor4d MANGLE(Color4d)
#define glColor4fv MANGLE(Color4fv)
#define glColor4dv MANGLE(Color4dv)

#define glNormal3f MANGLE(Normal3f)
#define glNormal3d MANGLE(Normal3d)
#define glNormal3fv MANGLE(Normal3fv)
#define glNormal3dv MANGLE(Normal3dv)

#define glTexCoord1f MANGLE(TexCoord1f)
#define glTexCoord1d MANGLE(TexCoord1d)
#define glTexCoord1fv MANGLE(TexCoord1fv)
#define glTexCoord1dv MANGLE(TexCoord1dv)

#define glTexCoord2f MANGLE(TexCoord2f)
#define glTexCoord2d MANGLE(TexCoord2d)
#define glTexCoord2fv MANGLE(TexCoord2fv)
#define glTexCoord2dv MANGLE(TexCoord2dv)
#define glTexCoord3f MANGLE(TexCoord3f)
#define glTexCoord3d MANGLE(TexCoord3d)
#define glTexCoord3fv MANGLE(TexCoord3fv)
#define glTexCoord3dv MANGLE(TexCoord3dv)
#define glTexCoord4f MANGLE(TexCoord4f)
#define glTexCoord4d MANGLE(TexCoord4d)
#define glTexCoord4fv MANGLE(TexCoord4fv)
#define glTexCoord4dv MANGLE(TexCoord4dv)

#define glEdgeFlag MANGLE(EdgeFlag)

/* matrix */
#define glMatrixMode MANGLE(MatrixMode)
#define glLoadMatrixf MANGLE(LoadMatrixf)
#define glLoadIdentity MANGLE(LoadIdentity)
#define glMultMatrixf MANGLE(MultMatrixf)
#define glPushMatrix MANGLE(PushMatrix)
#define glPopMatrix MANGLE(PopMatrix)
#define glRotatef MANGLE(Rotatef)
#define glTranslatef MANGLE(Translatef)
#define glScalef MANGLE(Scalef)

#define glViewport MANGLE(Viewport)
#define glFrustum MANGLE(Frustum)

/* lists */
#define glGenLists MANGLE(GenLists)
#define glIsList MANGLE(IsList)
#define glNewList MANGLE(NewList)
#define glEndList MANGLE(EndList)
#define glCallList MANGLE(CallList)

/* clear */
#define glClear MANGLE(Clear)
#define glClearColor MANGLE(ClearColor)
#define glClearDepth MANGLE(ClearDepth)

/* selection */
#define glRenderMode MANGLE(RenderMode)
#define glSelectBuffer MANGLE(SelectBuffer)

#define glInitNames MANGLE(InitNames)
#define glPushName MANGLE(PushName)
#define glPopName MANGLE(PopName)
#define glLoadName MANGLE(LoadName)

/* textures */
#define glGenTextures MANGLE(GenTextures)
#define glDeleteTextures MANGLE(DeleteTextures)
#define glBindTexture MANGLE(BindTexture)
#define glTexImage2D MANGLE(TexImage2D)
#define glTexEnvi MANGLE(TexEnvi)
#define glTexParameteri MANGLE(TexParameteri)
#define glPixelStorei MANGLE(PixelStorei)

/* lighting */

#define glMaterialfv MANGLE(Materialfv)
#define glMaterialf MANGLE(Materialf)
#define glColorMaterial MANGLE(ColorMaterial)

#define glLightfv MANGLE(Lightfv)
#define glLightf MANGLE(Lightf)
#define glLightModeli MANGLE(LightModeli)
#define glLightModelfv MANGLE(LightModelfv)

/* misc */

#define glFlush MANGLE(Flush)
#define glHint MANGLE(Hint)
#define glGetIntegerv MANGLE(GetIntegerv)
#define glGetFloatv MANGLE(GetFloatv)
#define glFrontFace MANGLE(FrontFace)

/* opengl 1.2 arrays */
#define glEnableClientState MANGLE(EnableClientState)
#define glDisableClientState MANGLE(DisableClientState)
#define glArrayElement MANGLE(ArrayElement)
#define glVertexPointer MANGLE(VertexPointer)
#define glColorPointer MANGLE(ColorPointer)
#define glNormalPointer MANGLE(NormalPointer)
#define glTexCoordPointer MANGLE(TexCoordPointer)

/* opengl 1.2 polygon offset */
#define glPolygonOffset MANGLE(PolygonOffset)

#define glDebug MANGLE(Debug)

#define glInit MANGLE(Init)
#define glClose MANGLE(Close)

