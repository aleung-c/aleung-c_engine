#include "../includes/aleung-c_engine.hpp"

// --------------------------------------------------------------------	//
//																		//
//	Rendering 3D GameObject												//
//																		//
//	This is a classic opengl vertex drawing.							//
//	Faces are loaded from the .obj when you declare a new GameObject	//
//	and everything is set at this point to display the object on the	//
//	screen.																//
//																		//
// --------------------------------------------------------------------	//

void		GameEngineController::render3DGameObject(GameObject	*obj)
{
	// texture loading.
	if (obj->HasTexture() == true)
	{
		loadObjectTexture(obj);
	}
	else
		glUniform1i(glGetUniformLocation(MainShaderProgramme, "has_texture"), GL_FALSE);
	// opengl buffer loading.
	if (obj->HasModel == true)
	{
		applyMatricesToObject(obj);

		// ------ load the uvs for the object - LOCATION = 1
		glBindBuffer(GL_ARRAY_BUFFER, obj->GetFubo());
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		// ------ load vertex and draw them - LOCATION = 0
		// ----------- To display triangles from faces vertex
		glBindBuffer(GL_ARRAY_BUFFER, obj->GetFvbo());
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, obj->GetNbFaceVertices());

		// ----------- To display only points from vertex -> useful for debug.
		// glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		// glEnableVertexAttribArray(0);
		// glDrawArrays (GL_POINTS, 0, _objVertices.size());

		// ----- disable all after draw;
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}

// --------------------------------------------------------------------	//
//																		//
//	Rendering UI object													//
//																		//
//	There is only one quad per object, and it is a picture.				//
//	The picture will be drawn using the same shader as the text			//
//	-> orthographic shaders												//
//																		//
// --------------------------------------------------------------------	//

void	GameEngineController::renderGameUIObject(GameUIObject *obj)
{
	glBindVertexArray(obj->GetVao());
	xpos = obj->Position.x;
	ypos = obj->Position.y;

	w = obj->GetTexture()->width * obj->ScaleValue;
	h = obj->GetTexture()->height * obj->ScaleValue;
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

// --------------------------------------------------------------------	//
//																		//
//	Rendering Text														//
//																		//
//	Text is a bit tricky as every letter is a small quad generated		//
//	from the truetype's loaded font.									//
//																		//
// --------------------------------------------------------------------	//

void	GameEngineController::renderGameTextObject(GameTextObject *obj)
{
	glUniform3f(glGetUniformLocation(OrthoShaderProgramme, "textColor"),
		obj->Color.x, obj->Color.y, obj->Color.z);
	glBindVertexArray(obj->GetVao());
	tmp_x = obj->Position.x;
	tmp_y = obj->Position.y;
	// Iterate through all characters of the string.
	for (std::string::const_iterator c = obj->Text.begin();
		c != obj->Text.end();
		c++)
	{
		ch = Characters[*c]; // take the struct in the map.

		xpos = tmp_x + ch.Bearing.x * obj->ScaleValue;
		ypos = tmp_y - (ch.Size.y - ch.Bearing.y) * obj->ScaleValue;

		w = ch.Size.x * obj->ScaleValue;
		h = ch.Size.y * obj->ScaleValue;
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
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glBindBuffer(GL_ARRAY_BUFFER, obj->GetVbo());
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);
		// then draw
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Advance cursors for next glyph (note that advance is number of 1/64 pixels)
		tmp_x += (ch.Advance >> 6) * obj->ScaleValue; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glDisableVertexAttribArray(0);
}
