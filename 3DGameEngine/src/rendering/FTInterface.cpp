#include "rendering/FTInterface.h"

FTInterface* FTInterface::get()
{
	static FTInterface ftInterface;
	
	return &ftInterface;
}
FTInterface::FTInterface(){}
FTInterface::~FTInterface(){}

void FTInterface::init()

{
	FTInterface *ins = FTInterface::get();

	if(FT_Init_FreeType(&ins->m_ftLib))
		std::cout<<"ERROR::FREETYPE: Could not init FreeType"<< std::endl;
	if(FT_New_Face(ins->m_ftLib,"assets/fonts/arial.ttf",0,&ins->m_ftFace))
		std::cout<<"ERROR::FREETYPE: Failed to load font"<<std::endl;
	FT_Set_Pixel_Sizes(ins->m_ftFace,0,48);

	ins->genChars();
	ins->textShader = Assets::getShader("text");
	ins->genVBO();
}

void FTInterface::renderText(std::string text, GLfloat x,GLfloat y,GLfloat scale, glm::vec3 colour)
{
	FTInterface *ins = FTInterface::get();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	ins->textShader->useProgram();
	glm::mat4 mProj = glm::ortho(0.0f,(float)SCREENWIDTH,0.0f,(float)SCREENHEIGHT);

	ins->textShader->setUniform("mProjection",mProj);
	ins->textShader->setUniform("textColour",colour);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(ins->VAO);

	std::string::const_iterator c;

	for(c=text.begin();c != text.end();c++)
	{
		Character ch = ins->m_Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		GLfloat verticies[6][4] = {
			{xpos,     ypos + h, 0.0,0.0},
			{xpos,     ypos,	 0.0,1.0},
			{xpos + w, ypos,	 1.0,1.0},
			
			{xpos,     ypos + h, 0.0,0.0},
			{xpos + w, ypos,     1.0,1.0},
			{xpos + w, ypos + h, 1.0,0.0}
		};

		glBindTexture(GL_TEXTURE_2D,ch.TextureID);
		
		glBindBuffer(GL_ARRAY_BUFFER,ins->VBO);
		glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(verticies),verticies);
		glBindBuffer(GL_ARRAY_BUFFER,0);

		glDrawArrays(GL_TRIANGLES,0,6);

		x +=(ch.Advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D,0);
}

void FTInterface::genChars()
{

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for(GLubyte c = 0; c < 128; c++)
	{
		if(FT_Load_Char(m_ftFace, c,FT_LOAD_RENDER))
		{
			std::cout<<"ERROR::FREETYPE: Failed to load glyph"<<std::endl;
			continue;
		}
		GLuint texture;
		glGenTextures(1,&texture);
		glBindTexture(GL_TEXTURE_2D,texture);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RED,
			m_ftFace->glyph->bitmap.width,
			m_ftFace->glyph->bitmap.rows,
			0,GL_RED,GL_UNSIGNED_BYTE,m_ftFace->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		Character character = {
			texture,
			glm::ivec2(m_ftFace->glyph->bitmap.width,m_ftFace->glyph->bitmap.rows),
			glm::ivec2(m_ftFace->glyph->bitmap_left,m_ftFace->glyph->bitmap_top),
			m_ftFace->glyph->advance.x};

		m_Characters.insert(std::pair<GLchar,Character>(c,character));
	}
	FT_Done_Face(m_ftFace);
	FT_Done_FreeType(m_ftLib);
}
void FTInterface::genVBO()
{
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4 * sizeof(GLfloat),0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
}
