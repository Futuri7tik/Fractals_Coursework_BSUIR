#ifndef KURSACH_FUNCTIONS_H
#define KURSACH_FUNCTIONS_H
#include "raylib.h"
#include <stddef.h>
#define WIDTH 1920
#define HEIGHT 1080

typedef enum {
    STATE_MENU,
    STATE_SLIDESHOW,
    STATE_GALLERY,
    STATE_TREE,
    STATE_CARPET,
    STATE_TRIANGLE,
    STATE_MANDELBROT,
    STATE_JULIA,
    STATE_RANDOM
} AppState;

typedef struct {
    float min;
    float max;
    float value;
} SliderParameters;

typedef struct ImageNode {
    char* fract_name;
    char* img_name;
    Rectangle field;
    Texture2D texture;
    AppState target_state;
    struct ImageNode* next;
} ImageNode;

// === Параметры фракталов ===
typedef struct {
    float depth;
    int max_depth;
    float angle;
    float angle_degrees;
    float length_factor;
} TreeParameters;

typedef struct {
    float depth;
    int max_depth;
    float start_length;
    float red;
    float green;
    float blue;
    Texture2D texture;
} CarpetParameters;

typedef struct {
    float depth;
    float x_start;
    float y_start;
    int max_depth;
    float start_length;
    float red;
    float green;
    float blue;
} TriangleParameters;

typedef struct {
    float iterations;
    int max_iterations;
    float zoom;
    float offset_x;
    float offset_y;
    float red;
    float green;
    float blue;
    Texture2D texture;
} MandelbrotParameters;

typedef struct {
    float iterations;
    int max_iterations;
    float re_c;
    float im_c;
    float zoom;
    float offset_x;
    float offset_y;
    float red;
    float green;
    float blue;
    Texture2D texture;
} JuliaParameters;

typedef struct FractalParameters {
    TreeParameters tree;
    CarpetParameters carpet;
    TriangleParameters triangle;
    MandelbrotParameters mandelbrot;
    JuliaParameters julia;
} FractalParameters;

// === Initialization
void init_fractals_parameters(FractalParameters* params);
void init_tree_parameters(TreeParameters* params);
void init_carpet_parameters(CarpetParameters* params);
void init_triangle_parameters(TriangleParameters* params);
void init_mandelbrot_parameters(MandelbrotParameters* params);
void init_julia_parameters(JuliaParameters* params);
void init_random_config(FractalParameters* params, AppState* type);

// === UI ===
void handle_movement(float speed, Camera2D* cam, bool* update);
void menu_gui(AppState* state, bool* show_msg_box, bool* should_close);
void gallery_gui(AppState *state, FractalParameters* params, Camera2D *cam, ImageNode **head_img,
                 bool *update, int *random_type, FractalParameters *random_params);
void tree_gui(FractalParameters* params, Camera2D* cam);
void carpet_gui(FractalParameters* params, Camera2D* cam, bool* update);
void triangle_gui(FractalParameters* params, Camera2D* cam);
void mandelbrot_gui(FractalParameters* params, Camera2D* cam, bool* update);
void julia_gui(FractalParameters* params, Camera2D* cam, bool* update);
void random_gui(AppState *state, FractalParameters *params, bool *update);

ImageNode* create_image_node(char* fract_name, char* img_name, Rectangle field, Texture2D texture, AppState state);
void load_gallery(ImageNode** head, char* fract_names[], char* img_names[], size_t size, Rectangle* img_fields);
void draw_pics(ImageNode* head);

// === Дерево ===
Color get_color_tree(int depth, const TreeParameters* params);
void draw_tree(float x_start, float y_start, float length, float angle,
               int depth, TreeParameters* params);

// === Ковёр ===
void draw_square(float x, float y, float length, Color color);
void draw_carpet(float x_left, float y_left, float length,
                 int depth, CarpetParameters* params);
void draw_carpet_to_image(Image* img, float x, float y, float length, int depth);
Texture2D render_carpet_to_texture(int width, int height, int depth, float start_length, Color color);

// === Треугольник ===
Color get_color_triangle(int depth, TriangleParameters* params);
void draw_triangle_base(float x, float y, float length, Color color);
void draw_center_triangle(float x, float y, float length, Color color);
void draw_sierpinski_triangle(float x, float y, float length,
                              int depth, TriangleParameters* params);

// === Множество мандельброта
int mandelbrot_iterations(float re_c, float im_c, int max_iterations);
Color get_color_mandelbrot(int iteration, int max_iterations, const MandelbrotParameters* params);
Texture2D render_mandelbrot(int width, int height, float zoom, float offset_x,
    float offset_y, int max_iterations, const MandelbrotParameters* params);
int mandelbrot_fourth_iterations(float re_c, float im_c, int max_iterations);

// === Множество Джулии
int julia_iterations(float re_z, float im_z, float re_c, float im_c, int max_iterations);
Color get_color_julia(int iteration, int max_iterations, const JuliaParameters* params);
Texture2D render_julia(int width, int height, float zoom, float offset_x,
    float offset_y, int max_iterations, const JuliaParameters* params);
#endif