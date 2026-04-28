#ifndef KURSACH_FUNCTIONS_H
#define KURSACH_FUNCTIONS_H
#include "raylib.h"
#include <stddef.h>
#include <stdbool.h>

#define WIDTH 1920
#define HEIGHT 1080
#define SQRT3 1.73205080756f

typedef enum {
    STATE_MENU,
    STATE_SLIDESHOW,
    STATE_GALLERY,
    STATE_MANDELBROT,
    STATE_TREE,
    STATE_CARPET,
    STATE_TRIANGLE,
    STATE_JULIA,
    STATE_CIRCLE,
    STATE_FERN,
    STATE_NEWTON,
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

typedef struct CircleParameters {
    float depth;
    int max_depth;
    Vector2 center;
    float radius;
    float red;
    float green;
    float blue;
} CircleParameters;

typedef struct FernParameters{
    float iterations;
    int max_iterations;
    float prob1;
    float prob2;
    float prob3;
    float prob4;
    Texture2D texture;
} FernParameters;

typedef struct NewtonParameters {
    float iterations;
    int max_iterations;
    float zoom;
    float offset_x;
    float offset_y;
    float red;
    float green;
    float blue;
    float gradient_r;
    float gradient_g;
    float gradient_b;
    Texture2D texture;
} NewtonParameters;

typedef struct FractalParameters {
    TreeParameters tree;
    CarpetParameters carpet;
    TriangleParameters triangle;
    MandelbrotParameters mandelbrot;
    JuliaParameters julia;
    CircleParameters circle;
    FernParameters fern;
    NewtonParameters newton;
} FractalParameters;

// Initialization
void init_fractals_parameters(FractalParameters* params);
void init_tree_parameters(TreeParameters* params);
void init_default_tree_parameters(TreeParameters* params);
void init_carpet_parameters(CarpetParameters* params);
void init_default_carpet_parameters(CarpetParameters* params);
void init_triangle_parameters(TriangleParameters* params);
void init_default_triangle_parameters(TriangleParameters* params);
void init_mandelbrot_parameters(MandelbrotParameters* params);
void init_default_mandelbrot_parameters(MandelbrotParameters* params);
void init_julia_parameters(JuliaParameters* params);
void init_default_julia_parameters(JuliaParameters* params);
void init_circle_parameters(CircleParameters* params);
void init_default_circle_parameters(CircleParameters* params);
void init_fern_parameters(FernParameters* params);
void init_default_fern_parameters(FernParameters* params);
void init_newton_parameters(NewtonParameters *params);
void init_default_newton_parameters(NewtonParameters* params);
void init_random_config(FractalParameters* params, Camera2D* cam,AppState* type);

void rewrite_tree_parameters(const TreeParameters* params);
void rewrite_carpet_parameters(const CarpetParameters* params);
void rewrite_fractal_parameters(const FractalParameters* params);
void rewrite_triangle_parameters(const TriangleParameters* params);
void rewrite_mandelbrot_parameters(const MandelbrotParameters* params);
void rewrite_julia_parameters(const JuliaParameters* params);
void rewrite_circle_parameters(const CircleParameters* params);
void rewrite_fern_parameters(const FernParameters* params);
void rewrite_newton_parameters(const NewtonParameters* params);

// UI
void handle_movement(float speed, Camera2D* cam, bool* update);
void menu_gui(AppState* state, bool* show_msg_box, bool* should_close);
void load_pics(ImageNode** head);
void gallery_gui(AppState *state, FractalParameters *params, Camera2D *cam, ImageNode **head_img,
                 bool *update);
void tree_gui(FractalParameters* params, Camera2D* cam);
void carpet_gui(FractalParameters* params, Camera2D* cam, bool* update);
void triangle_gui(FractalParameters* params, Camera2D* cam);
void mandelbrot_gui(FractalParameters* params, Camera2D* cam, bool* update);
void julia_gui(FractalParameters* params, Camera2D* cam, bool* update);
void render_fractal_gui(Camera2D* cam, FractalParameters* params, const AppState* state, bool* update);
void circle_gui(FractalParameters* params, Camera2D* cam);
void fern_gui(FractalParameters* params, Camera2D* cam, bool* update);
void newton_gui(FractalParameters* params, Camera2D* cam, bool* update);

// saving image
void save_image(AppState state, AppState random_type, const FractalParameters* params,
    const FractalParameters* random_params);

ImageNode* create_image_node(char* fract_name, char* img_name, Rectangle field, Texture2D texture, AppState state);
void load_gallery(ImageNode** head, char* fract_names[], char* img_names[], size_t size, Rectangle* img_fields);
void draw_pics(ImageNode* head);

// Дерево
Color get_color_tree(int depth, const TreeParameters* params);
void draw_tree(float x_start, float y_start, float length, float angle,
               int depth, TreeParameters* params);

// Ковёр
void draw_square(float x, float y, float length, Color color);
void draw_carpet(float x_left, float y_left, float length,
                 int depth, CarpetParameters* params);
void draw_carpet_to_image(Image* img, float x, float y, float length, int depth);
void render_carpet_to_texture(int depth, float start_length, Color color, CarpetParameters* params);

// Треугольник
Color get_color_triangle(int depth, TriangleParameters* params);
void draw_triangle_base(float x, float y, float length, Color color);
void draw_center_triangle(float x, float y, float length, Color color);
void draw_sierpinski_triangle(float x, float y, float length,
                              int depth, TriangleParameters* params);

// Множество мандельброта
int mandelbrot_iterations(float re_c, float im_c, int max_iterations);
Color get_color_mandelbrot(int iteration, int max_iterations, const MandelbrotParameters* params);
void render_mandelbrot(float zoom, float offset_x, float offset_y, int max_iterations, const MandelbrotParameters *params);
int mandelbrot_fourth_iterations(float re_c, float im_c, int max_iterations);

// Множество Жюлиа
int julia_iterations(float re_z, float im_z, float re_c, float im_c, int max_iterations);
Color get_color_julia(int iteration, int max_iterations, const JuliaParameters* params);
void render_julia(float zoom, float offset_x,
    float offset_y, int max_iterations, const JuliaParameters* params);

// Круговой фрактал
void draw_circle_fractal(float x, float y, float r, int depth, Color color);

// Папоротник Барнсли
Texture2D render_barnsley_fern(Vector2 pos, int depth, FernParameters params);

// Бассейны ньютона
void render_newton(float zoom, float offset_x, float offset_y, int max_iterations,
                        NewtonParameters *params);
int newton_iterations(float re_z, float im_z, int max_iterations, int* root_num);
Color get_color_newton(int iter, int root, const NewtonParameters *params);

void render_fractals(const Camera2D* cam, const AppState* state, FractalParameters* params, bool* update);

void clear_undo(void);
void init_undo(void);
void push(FractalParameters* params);
bool pop(FractalParameters* out);

#endif