

static void initData_mehdi(void) {
  SDL_Surface * texSurface;
  GLfloat s = 10.0, data[] = { 
    /* 4 coordonnées de sommets */
    -s, 0.0f, -s,
    s , 0.0f, -s,
    -s, 0.0f,  s,
    s , 0.0f,  s,
    /* 4 normales */
    0.0f, 1.0f, 0.0f, 
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    /* 4 coordonnées de texture, une par sommet */
    0.0f, 0.0f, 
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
  };


  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glGenBuffers(1, &_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, _buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof data, data, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);  
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,  0, (const void *)(4 * 3 * sizeof *data));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const void *)(4 * 6 * sizeof *data));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glGenTextures(1, &_tId);
  glBindTexture(GL_TEXTURE_2D, _tId);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  if( (texSurface = SDL_LoadBMP("images/repere.bmp")) == NULL ) {
    fprintf(stderr, "Impossible d'ouvrir le fichier : %s\n", SDL_GetError());
    exit(1);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSurface->w, texSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texSurface->pixels);
  SDL_FreeSurface(texSurface);
  glBindTexture(GL_TEXTURE_2D, 0);


SDL_Surface * texSurface_sol;
 /*GLfloat s_sol = 10.0,*/GLfloat data_sol[] = { 
  /* 4 coordonnées de sommets */
  -s, 0.0f, -s,
  s , 0.0f, -s,
  -s, 0.0f,  s,
  s , 0.0f,  s,
  /* 4 normales */
  0.0f, 1.0f, 0.0f, 
  0.0f, 1.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.0f, 1.0f, 0.0f,
  /* 4 coordonnées de texture, une par sommet */
  0.0f, 0.0f, 
  1.0f, 0.0f,
  0.0f, 1.0f,
  1.0f, 1.0f
};

glGenVertexArrays(1, &_vao_sol);
glBindVertexArray(_vao_sol);

glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);
glEnableVertexAttribArray(2);

glGenBuffers(1, &_buffer_sol);
glBindBuffer(GL_ARRAY_BUFFER, _buffer_sol);
glBufferData(GL_ARRAY_BUFFER, sizeof data_sol, data_sol, GL_STATIC_DRAW);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);  
glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE,  0, (const void *)(4 * 3 * sizeof *data_sol));
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const void *)(4 * 6 * sizeof *data_sol));
glBindBuffer(GL_ARRAY_BUFFER, 0);
glBindVertexArray(0);




glGenTextures(1, &_tId_sol);
glBindTexture(GL_TEXTURE_2D, _tId_sol);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
if( (texSurface_sol = SDL_LoadBMP("images/feuilles.bmp")) == NULL ) {
  fprintf(stderr, "Impossible d'ouvrir le fichier : %s\n", SDL_GetError());
  exit(1);
 }
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texSurface_sol->w, texSurface_sol->h, 0, GL_BGR, GL_UNSIGNED_BYTE, texSurface_sol->pixels);
SDL_FreeSurface(texSurface_sol);
glBindTexture(GL_TEXTURE_2D, 0);
}
