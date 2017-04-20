#include "../includes/aleung-c_engine.hpp"

// --------------------------------------------------------------------	//
//																		//
//	Rendering UI object													//
//	Here is the code to render UI object								//
//	It is very similar to rendering text								//
//	Only, there is only one quad per object, and it is a picture.		//
//	The picture will be drawn using the same shader as the text			//
//	-> orthographic shaders												//
//																		//
// --------------------------------------------------------------------	//

void	GameEngineController::RenderUIObject(GameUIObject *obj)
{
	glBindVertexArray(obj->GetVao());
	xpos = obj->Position.x;
	ypos = obj->Position.y;

	w = obj->GetTexture().width * obj->Scale;
	h = obj->GetTexture().height * obj->Scale;
	// Creates two triangles forming a quad.
	GLfloat vertices[6][4] = {
		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
	};
	// bind character texture, then put the vertices in the object's buffer.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, obj->GetTextureID());
	glBindBuffer(GL_ARRAY_BUFFER, obj->GetVbo());
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	// then draw
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}
