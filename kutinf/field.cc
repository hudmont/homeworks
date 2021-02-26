#include <iostream>
#include <array>
#include <SDL2/SDL.h>

#define RESY 600
#define RESX 800

using namespace std;

typedef array<array<double, 3>, 3> Stencil;

// we add a line of zeroes around the area to simplify the computation kernel
// this we won't need a series of ifs in the loop or breaking up the loop
typedef array<array<double, RESX+2>, RESY+2> Field;

inline void applyStencil(Field &current, Field &next, Stencil &stencil) {
  double sum;
  #pragma omp parallel for
  for(unsigned int y=1; y < current.size()-1 ; y++) {
    for(unsigned int x=1; x < current[y].size()-1 ; x++) {
      // TODO this is slow and a bad abstraction
      // TODO this is numerically bad
      sum = 0.0;
      sum+=current[y-1][x-1]*stencil[0][0];
      sum+=current[y-1][x+0]*stencil[0][1];
      sum+=current[y-1][x+1]*stencil[0][2];
      sum+=current[y+0][x-1]*stencil[1][0];
      sum+=current[y+0][x+0]*stencil[1][1];
      sum+=current[y+0][x+1]*stencil[1][2];
      sum+=current[y+1][x-1]*stencil[2][0];
      sum+=current[y+1][x+0]*stencil[2][1];
      sum+=current[y+1][x+1]*stencil[2][2];
      next[y][x] = sum/4.0;
    }
  }
}

Field fields[2];
int _current_field=0;

#define current fields[_current_field]
#define next fields[(_current_field+1)%2]

inline void swap_fields() {
  _current_field++; 
  _current_field%=2;
}

inline void add_points() {
  current[10][10]=250.0;
  current[59][79]=250.0;
  current[34][18]=250.0;
  current[35][18]=250.0;
  current[34][19]=250.0;
  current[35][19]=250.0;
  current[400][500]=250.0;
}

int main(void) {

  
  int maxiter = 2000;
  
  Stencil laplacian;
  laplacian[0]={0.0,   1.0, 0.0};
  laplacian[1]={1.0,   0.0, 1.0};
  laplacian[2]={0.0,   1.0, 0.0};

  // laplacian[0]={0.0,  1.0, 0.0};
  // laplacian[1]={1.0,  0.0, 1.0};
  // laplacian[2]={0.0,  1.0, 0.0};

  add_points();
  

  for(int iter=0; iter < maxiter; iter++) {
    applyStencil(current, next, laplacian);
    swap_fields();
    add_points();
  }

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event event;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(RESX, RESY, 0, &window, &renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
  SDL_RenderClear(renderer);
  
  for(unsigned int y=1; y < current.size()-1; y++){
    for(unsigned int x=1; x < current[y].size()-1; x++){
      SDL_SetRenderDrawColor(renderer, 255-(Uint8)(current[y][x]) , 255-(Uint8)(current[y][x]), 255, 0);
      SDL_RenderDrawPoint(renderer, x-1, y-1); //Renders on middle of screen.
    }
  }
  
  SDL_RenderPresent(renderer);
  while (1) {
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      break;
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  cout << current[400][500] << endl;
  cout << current[401][500] << endl;
  cout << current[402][500] << endl;
  cout << current[403][500] << endl;
  return EXIT_SUCCESS;
}

